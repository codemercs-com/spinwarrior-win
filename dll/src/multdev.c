//
// multdev.cpp
// IO-Warrior SDK library
//
// multiple devices support functions
//

#include "stdafx.h"
#include "spindev.h"

static char KitVersion[] = "SpinWarrior Kit V1.6";

PCHAR SPINKIT_API SpinKitVersion(void)
{
	return KitVersion;
}

// Return number of devices
ULONG SPINKIT_API SpinKitGetNumDevs(void)
{
	return spinNumDevices;
}

// Get Nth device handle
SPINKIT_HANDLE SPINKIT_API SpinKitGetDeviceHandle(ULONG numDevice)
{
	if ((numDevice < 1) || (numDevice > spinNumDevices))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return NULL;
	}
	else
	{
		SetLastError(ERROR_SUCCESS);
		return &spinDevices[numDevice - 1];
	}
}

ULONG SPINKIT_API SpinKitGetProductId(SPINKIT_HANDLE spinHandle)
{
	PSPINKIT_DEVICE_INTERNAL spinDev = (PSPINKIT_DEVICE_INTERNAL)spinHandle;

	if (spinDev == NULL || (spinDev->cookie != SPINKIT_DEVICE_COOKIE))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	else
	{
		SetLastError(ERROR_SUCCESS);
		// both device parts have the same HID info
		return spinDev->HidInfo.Attributes.ProductID;
	}
}

ULONG SPINKIT_API SpinKitGetRevision(SPINKIT_HANDLE spinHandle)
{
	PSPINKIT_DEVICE_INTERNAL spinDev = (PSPINKIT_DEVICE_INTERNAL)spinHandle;

	if (spinDev == NULL || (spinDev->cookie != SPINKIT_DEVICE_COOKIE))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	else
	{
		SetLastError(ERROR_SUCCESS);
		// both device parts have the same HID info
		return spinDev->HidInfo.Attributes.VersionNumber;
	}
}

BOOL SPINKIT_API SpinKitGetSerialNumber(SPINKIT_HANDLE spinHandle, PWCHAR serialNumber)
{
	PSPINKIT_DEVICE_INTERNAL spinDev = (PSPINKIT_DEVICE_INTERNAL)spinHandle;

	if (spinDev == NULL || (spinDev->cookie != SPINKIT_DEVICE_COOKIE) || serialNumber == NULL)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	else
	{
		SetLastError(ERROR_SUCCESS);
		memcpy(serialNumber, spinDev->serialNumber, sizeof(spinDev->serialNumber));
		return serialNumber[0] != 0;
	}
}

BOOL SPINKIT_API SpinKitSetTimeout(SPINKIT_HANDLE spinHandle, ULONG timeout)
{
	PSPINKIT_DEVICE_INTERNAL spinDev = (PSPINKIT_DEVICE_INTERNAL)spinHandle;

	if (spinDev == NULL || (spinDev->cookie != SPINKIT_DEVICE_COOKIE))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	else
	{
		SetLastError(ERROR_SUCCESS);
		spinDev->ioTimeout = timeout;
		return TRUE;
	}
}