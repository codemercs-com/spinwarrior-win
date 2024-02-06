//
// readwrit.cpp
//
// IO-Warrior SDK library
// Read & write functions
//
#include "stdafx.h"
#include "spindev.h"

// Internal Spin-Kit functions

BOOLEAN SpiniCancelIo(HANDLE pipeHandle)
{
	HMODULE hKernel;
	BOOL(__stdcall * pCancelIo)(HANDLE hFile);

	hKernel = GetModuleHandleA("kernel32.dll");
	if (hKernel == NULL)
		return FALSE;
	pCancelIo = (BOOL(__stdcall*)(HANDLE)) GetProcAddress(hKernel, "CancelIo");
	if (pCancelIo == NULL)
		return FALSE;

	(*pCancelIo)(pipeHandle);

	return TRUE;
}

void __stdcall SpiniDummyCompletion(DWORD errorCode, DWORD numBytes, OVERLAPPED* ovlp)
{
}

// Low-level read-write stuff

BOOLEAN SpiniInsertBuffer(PSPINKIT_DEVICE_INTERNAL spinDev, PSPINKIT_MAX_REPORT report, DWORD size)
{
	DWORD rc;
	BOOLEAN bOk = TRUE;

	// Copy data to buffer
	// lock data
	rc = WaitForSingleObject(spinDev->bufMutex, INFINITE);
	// Save last read value
	spinDev->lastReport = *report;

	// Check if buffer is overflowing
	if (spinDev->numBuf == SPIN_BUFFER_SIZE)
	{
		bOk = FALSE;
		// Loose 1st value in buffer
		if (spinDev->bufStart == spinDev->bufEnd)
			spinDev->bufStart = SPINKIT_BUFFER_POS(spinDev->bufStart + 1);
		else
		{
			spinDev->bufStart = SPINKIT_BUFFER_POS(spinDev->bufStart + 1);
			spinDev->bufEnd = SPINKIT_BUFFER_POS(spinDev->bufEnd + 1);
		}
		// Save new value at the end of buffer
		spinDev->buffer[spinDev->bufEnd] = *report;
	}
	else
	{
		// Insert value at the end of buffer
		spinDev->buffer[spinDev->bufEnd] = *report;
		// Set new buffer end position
		spinDev->bufEnd = SPINKIT_BUFFER_POS(spinDev->bufEnd + 1);
		spinDev->numBuf++;
	}

	ReleaseMutex(spinDev->bufMutex);
	return bOk;
}

BOOLEAN SpiniGetBuffer(PSPINKIT_DEVICE_INTERNAL spinDev, PSPINKIT_MAX_REPORT preport)
{
	// Fail if buffer is empty
	if (spinDev->numBuf == 0)
		return FALSE;
	// Acquire mutex and get value from buffer
	WaitForSingleObject(spinDev->bufMutex, INFINITE);
	// Copy buffered value
	*preport = spinDev->buffer[spinDev->bufStart];
	// Set new buffer start
	spinDev->bufStart = SPINKIT_BUFFER_POS(spinDev->bufStart + 1);
	// Decrement number of buffered values
	spinDev->numBuf--;
	// Reset start & end positions
	if (spinDev->numBuf == 0)
		spinDev->bufStart = spinDev->bufEnd = 0;
	// Release mutex and exit
	ReleaseMutex(spinDev->bufMutex);
	return TRUE;
}

BOOLEAN SpiniReadReport(PSPINKIT_DEVICE_INTERNAL spinDev, PSPINKIT_MAX_REPORT preport)
{
	DWORD rc;

	if (!SpiniGetBuffer(spinDev, preport))
	{
		ResetEvent(spinDev->readEvent);
		// Synchronize with thread
		rc = WaitForSingleObject(spinDev->readEvent, spinDev->ioTimeout);
		ResetEvent(spinDev->readEvent);
		// Timeout expired, return
		if (rc != WAIT_OBJECT_0)
		{
			// Set error code
			SetLastError(ERROR_TIMEOUT);
			return FALSE;
		}
		return SpiniGetBuffer(spinDev, preport);
	}
	return TRUE;
}

void SpiniSetData(PSPINKIT_DEVICE_INTERNAL spinDev, PSPINKIT_MAX_REPORT preport, PSPINKIT_DATA SpinData)
{
	int i;

	WaitForSingleObject(spinDev->msgMutex, INFINITE);
	memset(SpinData, 0, sizeof(SPINKIT_DATA));
	SpinData->Device = (SPINKIT_HANDLE)spinDev;
	switch (spinDev->HidInfo.Attributes.ProductID)
	{
	case SPINKIT_PRODUCT_ID24R4:
	case SPINKIT_PRODUCT_ID28R4:
	//case SPINKIT_PRODUCT_ITRAIN:
		for (i = 0; i < 4; i++)
			SpinData->Spins[i] = preport->ReportR4.Spins[i];
		for (i = 0; i < 7; i++)
			SpinData->Buttons[i] = (preport->ReportR4.Buttons >> i) & 0x1;
		break;
	case SPINKIT_PRODUCT_ID24R6:
	case SPINKIT_PRODUCT_ID28R6:
		for (i = 0; i < 6; i++)
			SpinData->Spins[i] = preport->ReportR6.Spins[i];
		for (i = 0; i < 3; i++)
			SpinData->Buttons[i] = (preport->ReportR6.Buttons >> i) & 0x1;
		break;
	case SPINKIT_PRODUCT_ID24A3:
	case SPINKIT_PRODUCT_ID28A3:
		for (i = 0; i < 3; i++)
			SpinData->Spins[i] = (int)(preport->ReportA3.Spins[i]);
		for (i = 0; i < 6; i++)
			SpinData->Buttons[i] = (preport->ReportA3.Buttons >> i) & 0x1;
		break;

	case SPINKIT_PRODUCT_MAZERO:
		for (i = 0; i < 4; i++)
			SpinData->Spins[i] = preport->ReportItrain.Spins[i];
		//for (i = 0; i < 7; i++)
		//    SpinData->Buttons[i] = (preport->ReportItrain->Buttons >> i) & 0x1;
		break;

	}
	ReleaseMutex(spinDev->msgMutex);
}

// SpinKitRead(): read value from device
BOOL SPINKIT_API SpinKitRead(SPINKIT_HANDLE spinHandle, PSPINKIT_DATA SpinData)
{
	PSPINKIT_DEVICE_INTERNAL spinDev = (PSPINKIT_DEVICE_INTERNAL)spinHandle;
	HANDLE pipeHandle;
	OVERLAPPED ovlp;
	SPINKIT_MAX_REPORT report;
	DWORD err;

	if (spinDev == NULL || (spinDev->cookie != SPINKIT_DEVICE_COOKIE) || SpinData == NULL)
		return FALSE;

	// Get pipe handle
	pipeHandle = spinDev->ReadHandle;
	// Pipe doesn't exist, fail
	if ((pipeHandle == INVALID_HANDLE_VALUE))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	ZeroMemory(&ovlp, sizeof(ovlp));
	// Read file asynchronously
	// NOTE: we check if we read from first pipe here and sync with
	// thread in that case
	WaitForSingleObject(spinDev->readMutex, INFINITE);

	if (SpiniReadReport(spinDev, &report))
		SpiniSetData(spinDev, &report, SpinData);

	err = GetLastError();
	ReleaseMutex(spinDev->readMutex);
	SetLastError(err);
	return TRUE;
}

DWORD __stdcall SpinKitReadThread(PSPINKIT_DEVICE_INTERNAL spinDev)
{
	// Handle
	SPINKIT_MAX_REPORT report;
	HANDLE handle;
	// Set up ovlp
	OVERLAPPED ovlp;
	// SleepEx return value
	DWORD rc;
	DWORD size;

	// Set up overlapped structure
	ZeroMemory(&ovlp, sizeof(ovlp));
	// Get zero pipe handle
	handle = spinDev->ReadHandle;

	// Fire up reading
	for (;;)
	{
		// Read asynchronously
		memset(&report, 0, sizeof(SPINKIT_MAX_REPORT));
		switch (spinDev->HidInfo.Attributes.ProductID)
		{
		case SPINKIT_PRODUCT_ID24R6:
		case SPINKIT_PRODUCT_ID28R6:
			ReadFileEx(handle, &report.ReportR6, sizeof(report.ReportR6), &ovlp, &SpiniDummyCompletion);
			break;
		case SPINKIT_PRODUCT_ID24R4:
		case SPINKIT_PRODUCT_ID28R4:
		//case SPINKIT_PRODUCT_ITRAIN:
			ReadFileEx(handle, &report.ReportR4, sizeof(report.ReportR4), &ovlp, &SpiniDummyCompletion);
			break;
		//case SPINKIT_PRODUCT_ID24A3:
		case SPINKIT_PRODUCT_MAZERO:
			ReadFileEx(handle, &report.ReportItrain, sizeof(report.ReportItrain), &ovlp, &SpiniDummyCompletion);
			break;
		default:
			ReadFileEx(handle, &report.ReportA3, sizeof(report.ReportA3), &ovlp, &SpiniDummyCompletion);
			break;
		}

		// Perform async read
		for (;;)
		{
			// Wait for either I/O completion or for
			// "terminate thread" event
			rc = WaitForSingleObjectEx(spinDev->endEvent, 250, TRUE);
			// OK, we read something
			if (rc == WAIT_IO_COMPLETION)
			{
				size = 0;
				GetOverlappedResult(handle, &ovlp, &size, TRUE);
				SpiniInsertBuffer(spinDev, &report, size);
				// OK, set event
				SetEvent(spinDev->readEvent);
				break;
			}
			else
				// OK, we gotta terminate thread
				// AND tell the program it's safe to close everything
				if (rc == WAIT_OBJECT_0)
				{
					// Don't forget to cancel all I/O
					SpiniCancelIo(handle);
					// Signal that thread is now gone
					SetEvent(spinDev->threadTermEvent);
					// Goodbye, cruel world!
					ExitThread(0);
				}
		}
	}
}

// SpinKitReadNonBlocking(): get last 32-bits value from device
BOOL SPINKIT_API SpinKitReadNonBlocking(SPINKIT_HANDLE spinHandle, PSPINKIT_DATA SpinData)
{
	PSPINKIT_DEVICE_INTERNAL spinDev = (PSPINKIT_DEVICE_INTERNAL)spinHandle;
	BOOL ret;
	SPINKIT_MAX_REPORT report;

	if (spinDev == NULL || (spinDev->cookie != SPINKIT_DEVICE_COOKIE) || SpinData == NULL)
		return FALSE;

	ret = SpiniGetBuffer(spinDev, &report);
	if (ret)
		SpiniSetData(spinDev, &report, SpinData);
	else
		SpiniSetData(spinDev, &spinDev->lastReport, SpinData);
	return ret;
}
