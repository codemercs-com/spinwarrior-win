#ifndef _SPIN_DEVICE_
#define _SPIN_DEVICE_

#include "spinkit.h"
#include "spinkitextra.h"

// Config Manager API
typedef DWORD CONFIGRET;
typedef DWORD DEVNODE, DEVINST;
typedef DEVNODE* PDEVNODE, * PDEVINST;
extern CONFIGRET(__stdcall* pCM_Get_Parent)(PDEVINST pdnDevInst, DEVINST dnDevInst, ULONG ulFlags);

// HID function pointers
extern void(__stdcall* pHidD_GetHidGuid)(LPGUID HidGuid);
extern BOOLEAN(__stdcall* pHidD_GetAttributes)(HANDLE HidDeviceObject, PHIDD_ATTRIBUTES Attributes);
extern BOOLEAN(__stdcall* pHidD_GetPreparsedData)(HANDLE dev, PHIDP_PREPARSED_DATA* prepData);
extern NTSTATUS(__stdcall* pHidP_GetCaps)(PHIDP_PREPARSED_DATA prepData, PHIDP_CAPS caps);
extern BOOLEAN(__stdcall* pHidD_FreePreparsedData)(PHIDP_PREPARSED_DATA prepData);
extern BOOLEAN(__stdcall* pHidD_GetNumInputBuffers)(HANDLE HidDeviceObject, PULONG NumberBuffers);
extern BOOLEAN(__stdcall* pHidD_SetNumInputBuffers)(HANDLE HidDeviceObject, ULONG NumberBuffers);
extern BOOLEAN(__stdcall* pHidD_GetSerialNumberString)(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
extern BOOLEAN(__stdcall* pHidD_GetIndexedString)(HANDLE HidDeviceObject, ULONG StringIndex, PVOID Buffer, ULONG BufferLength);

// Setup API
extern BOOL(__stdcall* pSetupDiGetDeviceInterfaceDetailA)(HDEVINFO DeviceInfoSet, PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,
	PSP_DEVICE_INTERFACE_DETAIL_DATA_A DeviceInterfaceDetailData, DWORD DeviceInterfaceDetailDataSize,
	PDWORD RequiredSize, PSP_DEVINFO_DATA DeviceInfoData);
extern HDEVINFO(__stdcall* pSetupDiGetClassDevsA)(const GUID* ClassGuid, PCSTR Enumerator, HWND hwndParent, DWORD Flags);
extern BOOL(__stdcall* pSetupDiEnumDeviceInterfaces)(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData,
	const GUID* InterfaceClassGuid, DWORD MemberIndex, PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);
extern BOOL(__stdcall* pSetupDiDestroyDeviceInfoList)(HDEVINFO DeviceInfoSet);

#define SPINKIT_DEVICE_COOKIE  'aWPS'

// last 8 values are buffered
#define SPIN_BUFFER_SIZE     8
// Determine buffer position
#define SPINKIT_BUFFER_POS(pos)  ((pos) % SPIN_BUFFER_SIZE)
// IOW KIT serial number string length
#define SPINKIT_SN_LEN       8

// Don't forget to pack it!
#pragma pack(push, 1)

typedef struct _SPINKIT_REPORT_R4
{
	BYTE ReportID;
	unsigned char Spins[4];
	BYTE Buttons;
}
SPINKIT_REPORT_R4, * PSPINKIT_REPORT_R4;


typedef struct _SPINKIT_REPORT_ITRAIN
{
	BYTE ReportID;
	BYTE Spins[4];
	//BYTE Buttons;
}
SPINKIT_REPORT_ITRAIN, * PSPINKIT_REPORT_ITRAIN;


typedef struct _SPINKIT_REPORT_R6
{
	BYTE ReportID;
	signed char Spins[6];
	BYTE Buttons;
}
SPINKIT_REPORT_R6, * PSPINKIT_REPORT_R6;

typedef struct _SPINKIT_REPORT_A3
{
	BYTE ReportID;
	short int Spins[3];
	BYTE Buttons;
}
SPINKIT_REPORT_A3, * PSPINKIT_REPORT_A3;

typedef union _SPINKIT_MAX_REPORT
{
	SPINKIT_REPORT_R6 ReportR6;
	SPINKIT_REPORT_R4 ReportR4;
	SPINKIT_REPORT_A3 ReportA3;
	SPINKIT_REPORT_ITRAIN ReportItrain;
}
SPINKIT_MAX_REPORT, * PSPINKIT_MAX_REPORT;

#pragma pack(pop)

typedef struct _SPINKIT_HID_INFO
{
	HIDP_CAPS Caps;
	HIDD_ATTRIBUTES Attributes;
	PSP_DEVICE_INTERFACE_DETAIL_DATA_A DetailData;
}
SPINKIT_HID_INFO;

typedef struct _SPINKIT_DEVICE_INTERNAL
{
	DWORD cookie;         // Cookie
	ULONG refCount;       // Reference count
	ULONG ioTimeout;      // I/O read timeout

	// Async I/O thread handle
	// currently it does only reads in async mode
	// to keep up 32-bits value in sync with IOW
	HANDLE ioThread;
	HANDLE readEvent;               // Read complete event
	HANDLE endEvent;                // Terminate event
	HANDLE threadTermEvent;         // Thread has been terminated event
	HANDLE bufMutex;                // Mutex protecting buffer from changing
	HANDLE readMutex;               // Mutex serializing reads
	HANDLE msgMutex;

	HANDLE ReadHandle;
	SPINKIT_HID_INFO HidInfo;
	WCHAR serialNumber[SPINKIT_SN_LEN + 1];  // Serial number string

	SPINKIT_MAX_REPORT lastReport;   // Last report read from IO endpoint

	SPINKIT_MAX_REPORT buffer[SPIN_BUFFER_SIZE];  // reports read from IO endpoint
	DWORD bufStart;                              // Start & end buffer positions
	DWORD bufEnd;
	DWORD numBuf;                                // Number of buffers used
}
SPINKIT_DEVICE_INTERNAL, * PSPINKIT_DEVICE_INTERNAL;

// iowkit.cpp internal data
extern ULONG spinNumDevices;
extern SPINKIT_DEVICE_INTERNAL spinDevices[];

// iowkit.cpp internal functions
BOOLEAN SpinKitLoadLibraries(void);
void SpinKitUnloadLibraries(void);
DWORD __stdcall SpinKitReadThread(PSPINKIT_DEVICE_INTERNAL spinDev);

#endif // _SPIN_DEVICE_
