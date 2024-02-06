//
// openclos.cpp
// IO-Warrior SDK library
//
// IOW open & close functions
//
#include "stdafx.h"
#include "spindev.h"

// global internal variables
ULONG spinNumDevices = 0;
HANDLE spinPrivateHeap;
SPINKIT_DEVICE_INTERNAL spinDevices[SPINKIT_MAX_DEVICES];

// Here go internal functions

void SpiniClear(void)
{
	memset(spinDevices, 0, sizeof(spinDevices));
	spinNumDevices = 0;
}

// Get device_interface_detail_data for given device
PSP_DEVICE_INTERFACE_DETAIL_DATA_A SpiniGetDeviceInterfaceDetails(HDEVINFO devInfo, PSP_DEVICE_INTERFACE_DATA devInterfaceData, PSP_DEVINFO_DATA devData)
{
	PSP_DEVICE_INTERFACE_DETAIL_DATA_A devDetails;
	ULONG len, req_len;

	// Get length
	(*pSetupDiGetDeviceInterfaceDetailA)(devInfo, devInterfaceData, NULL, 0, &len, devData);
	if (len == 0)
		return NULL;
	// Allocate device details
	devDetails = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)HeapAlloc(spinPrivateHeap, 0, len);
	if (devDetails != NULL)
	{
		// Get details
		devDetails->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		(*pSetupDiGetDeviceInterfaceDetailA)(devInfo, devInterfaceData, devDetails, len, &req_len, devData);
	}
	return devDetails;
}

// Terminate thread and clean up after it
void SpiniTerminateThread(PSPINKIT_DEVICE_INTERNAL spinDev)
{
	ULONG rc;

	// Check if thread has been started
	if (spinDev->ioThread != NULL)
	{
		// Tell thread to terminate
		SetEvent(spinDev->endEvent);
		// Wait for thread to terminate
		rc = WaitForSingleObject(spinDev->threadTermEvent, INFINITE);
		// Wait for thread to terminate
		WaitForSingleObject(spinDev->ioThread, INFINITE);
		// Close thread handle
		CloseHandle(spinDev->ioThread);
	}
	// Close all events & mutexes
	if (spinDev->readEvent != NULL)
		CloseHandle(spinDev->readEvent);
	if (spinDev->endEvent != NULL)
		CloseHandle(spinDev->endEvent);
	if (spinDev->threadTermEvent != NULL)
		CloseHandle(spinDev->threadTermEvent);
	if (spinDev->bufMutex != NULL)
		CloseHandle(spinDev->bufMutex);
	if (spinDev->readMutex != NULL)
		CloseHandle(spinDev->readMutex);
	if (spinDev->msgMutex != NULL)
		CloseHandle(spinDev->msgMutex);
}

// Fire up async read thread
BOOLEAN SpiniFireUpThread(PSPINKIT_DEVICE_INTERNAL spinDev)
{
	DWORD threadId;

	// Create synchronization events
	// Read complete event
	spinDev->readEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	// thread termination event
	spinDev->endEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// thread has been terminated event
	spinDev->threadTermEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// Create mutexes
	spinDev->bufMutex = CreateMutex(NULL, FALSE, NULL);
	spinDev->readMutex = CreateMutex(NULL, FALSE, NULL);
	spinDev->msgMutex = CreateMutex(NULL, FALSE, NULL);
	// Create thread in suspended state
	spinDev->ioThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&SpinKitReadThread, spinDev, CREATE_SUSPENDED, &threadId);
	if ((spinDev->readEvent == NULL) || (spinDev->endEvent == NULL) || (spinDev->threadTermEvent == NULL) ||
		(spinDev->bufMutex == NULL) || (spinDev->readMutex == NULL) || (spinDev->msgMutex == NULL) ||
		(spinDev->ioThread == NULL))
	{
		SpiniTerminateThread(spinDev);
		return FALSE;
	}
	else
	{
		memset(&spinDev->lastReport, 0, sizeof(SPINKIT_REPORT_R6));
		ResumeThread(spinDev->ioThread);
		return TRUE;
	}
}

// Return TRUE if device represented by spinHandle is SpinKit device
// FALSE otherwise
BOOL SpiniIsSpinDevice(HANDLE spinHandle, PHIDD_ATTRIBUTES phidAttr)
{
	BOOL bOK;

	// Set up structure for call
	phidAttr->Size = sizeof(HIDD_ATTRIBUTES);
	// Get HID device attributes
	bOK = (*pHidD_GetAttributes)(spinHandle, phidAttr);

	if (bOK)
		// Compare vendorID & productID with SpinWarrior's
		// NOTE: we don't care about VersionNumber yet
		bOK = ((phidAttr->VendorID == SPINKIT_VENDOR_ID) &&
				(
				(phidAttr->ProductID == SPINKIT_PRODUCT_ID24R4) ||
				(phidAttr->ProductID == SPINKIT_PRODUCT_ID24R6) ||
				(phidAttr->ProductID == SPINKIT_PRODUCT_ID24A3) ||
				(phidAttr->ProductID == SPINKIT_PRODUCT_ID28R4) ||
				(phidAttr->ProductID == SPINKIT_PRODUCT_ID28R6) ||
				(phidAttr->ProductID == SPINKIT_PRODUCT_ID28A3) ||
				(phidAttr->ProductID == SPINKIT_PRODUCT_MAZERO)
				)
			 );

	return bOK;
}

void SpiniCloseOneDevice(PSPINKIT_DEVICE_INTERNAL spinDev)
{
	SpiniTerminateThread(spinDev);
	if (spinDev->ReadHandle != NULL)
		CloseHandle(spinDev->ReadHandle);
	if (spinDev->HidInfo.DetailData != NULL)
		HeapFree(spinPrivateHeap, 0, spinDev->HidInfo.DetailData);
	// Poison cookie
	spinDev->cookie = 0xf00fbeef;
}

BOOLEAN SpiniKitEnumDevices(void)
{
	HANDLE spinHandle;
	GUID hidGuid;
	HDEVINFO devInfo;
	SP_INTERFACE_DEVICE_DATA devInterfaceData;
	SP_DEVINFO_DATA devData;
	BOOL bEnum = TRUE, bFound = FALSE;
	PSPINKIT_DEVICE_INTERNAL spinDev;
	ULONG i;
	PSP_DEVICE_INTERFACE_DETAIL_DATA_A devDetails;
	PHIDP_PREPARSED_DATA prepData;
	HIDD_ATTRIBUTES hidAttr;
	HIDP_CAPS hidCaps;
	wchar_t Buffer[64];

	(*pHidD_GetHidGuid)(&hidGuid);
	// Start HID device enumeration
	devInfo = (*pSetupDiGetClassDevsA)(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	// Check if error
	if (devInfo == INVALID_HANDLE_VALUE)
		return FALSE;
	// Set up DEVICE_DATA for enumeration
	devInterfaceData.cbSize = sizeof(devInterfaceData);
	// Init
	i = -1;
	// Enumerate IOW devices
	while (bEnum)
	{
		i++;
		// Enumerate device
		bEnum = (*pSetupDiEnumDeviceInterfaces)(devInfo, NULL, &hidGuid, i, &devInterfaceData);
		// Exit enumeration loop
		if (!bEnum)
			break;

		devData.cbSize = sizeof(devData);
		devDetails = SpiniGetDeviceInterfaceDetails(devInfo, &devInterfaceData, &devData);

		// Open device for HID access
		spinHandle = CreateFileA(devDetails->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (spinHandle == INVALID_HANDLE_VALUE)
			spinHandle = CreateFileA(devDetails->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		// Check if we have handle
		if (spinHandle == INVALID_HANDLE_VALUE)
		{
			HeapFree(spinPrivateHeap, 0, devDetails);
			continue;
		}

		// OK, we opened device, check if it's not SpinWarrior
		// and continue loop in such case
		if (!SpiniIsSpinDevice(spinHandle, &hidAttr))
		{
			// Close handle and find next device
			CloseHandle(spinHandle);
			HeapFree(spinPrivateHeap, 0, devDetails);
			continue;
		}

		// We found SpinWarrior
		bFound = TRUE;
		spinDev = &spinDevices[spinNumDevices];
		spinNumDevices++;
		(*pHidD_GetPreparsedData)(spinHandle, &prepData);
		(*pHidP_GetCaps)(prepData, &hidCaps);
		(*pHidD_FreePreparsedData)(prepData);
		spinDev->HidInfo.Attributes = hidAttr;
		spinDev->HidInfo.Caps = hidCaps;
		spinDev->HidInfo.DetailData = devDetails;
		memset(spinDev->serialNumber, 0, sizeof(spinDev->serialNumber));

		if ((*pHidD_GetSerialNumberString)(spinHandle, spinDev->serialNumber, sizeof(spinDev->serialNumber)))
		{
			// fix a bug in HID.DLL
			(*pHidD_GetIndexedString)(spinHandle, 0, Buffer, sizeof(Buffer));
			if (wcscmp(spinDev->serialNumber, Buffer) == 0)
				memset(spinDev->serialNumber, 0, sizeof(spinDev->serialNumber));
		}

		// Set timeouts
		spinDev->ioTimeout = INFINITE;
		CloseHandle(spinHandle);
	}
	(*pSetupDiDestroyDeviceInfoList)(devInfo);
	return bFound;
}

SPINKIT_HANDLE SPINKIT_API SpinKitOpenDevice(void)
{
	ULONG i;
	SPINKIT_HANDLE ret;
	DWORD err;

	SpiniClear();
	if (!SpinKitLoadLibraries())
	{
		SetLastError(ERROR_DEV_NOT_EXIST);
		return NULL;
	}
	err = ERROR_SUCCESS;
	ret = NULL;
	spinPrivateHeap = HeapCreate(HEAP_NO_SERIALIZE, 4096, 0);
	if ((spinPrivateHeap != NULL) && SpiniKitEnumDevices())
	{
		for (i = 0; i < spinNumDevices; i++)
		{
			// Set cookie
			spinDevices[i].cookie = SPINKIT_DEVICE_COOKIE;
			spinDevices[i].ReadHandle = CreateFileA(spinDevices[i].HidInfo.DetailData->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			(*pHidD_SetNumInputBuffers)(spinDevices[i].ReadHandle, 128);
			if (!SpiniFireUpThread(&spinDevices[i]))
			{
				SpinKitCloseDevice(&spinDevices[0]);
				err = ERROR_DEV_NOT_EXIST;
				ret = NULL;
				break;
			}
		}
		ret = &spinDevices[0];
	}
	else
	{
		if (spinPrivateHeap != NULL)
			HeapDestroy(spinPrivateHeap);
		spinPrivateHeap = NULL;
		err = ERROR_DEV_NOT_EXIST;
	}
	// the libraries are not needed anymore
	SpinKitUnloadLibraries();
	SetLastError(err);
	return ret;
}

void SPINKIT_API SpinKitCloseDevice(SPINKIT_HANDLE spinHandle)
{
	ULONG i;

	// Close all devices
	for (i = 0; i < spinNumDevices; i++)
		SpiniCloseOneDevice(&spinDevices[i]);
	
	SpiniClear();

	if (spinPrivateHeap != NULL)
		HeapDestroy(spinPrivateHeap);

	spinPrivateHeap = NULL;
	SetLastError(ERROR_SUCCESS);
}
