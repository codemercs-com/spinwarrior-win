// SpinKit.cpp : Defines the entry point for the DLL application.
//

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//
// IO-Warrior SDK library
//
#include "stdafx.h"
#include "spindev.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
 {
  // We always succeed
  return TRUE;
 }

// HID stuff
// HID.DLL library handle
HMODULE hHid = NULL;
HMODULE hCfgMgr = NULL;
HMODULE hSetupApi = NULL;

// HID function pointers
BOOLEAN (__stdcall *pHidD_GetAttributes)(IN HANDLE HidDeviceObject, OUT PHIDD_ATTRIBUTES Attributes);
void (__stdcall *pHidD_GetHidGuid)(LPGUID HidGuid);
BOOLEAN (__stdcall *pHidD_GetPreparsedData)(HANDLE dev, PHIDP_PREPARSED_DATA *prepData);
NTSTATUS (__stdcall *pHidP_GetCaps)(PHIDP_PREPARSED_DATA prepData, PHIDP_CAPS caps);
BOOLEAN (__stdcall *pHidD_FreePreparsedData)(PHIDP_PREPARSED_DATA prepData);
BOOLEAN (__stdcall *pHidD_GetNumInputBuffers)(HANDLE HidDeviceObject, PULONG NumberBuffers);
BOOLEAN (__stdcall *pHidD_SetNumInputBuffers)(HANDLE HidDeviceObject, ULONG NumberBuffers);
BOOLEAN (__stdcall *pHidD_GetSerialNumberString)(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength);
BOOLEAN (__stdcall *pHidD_GetIndexedString)(HANDLE HidDeviceObject, ULONG StringIndex, PVOID Buffer, ULONG BufferLength);

// Config Manager function pointers
CONFIGRET (__stdcall *pCM_Get_Parent)(PDEVINST pdnDevInst, DEVINST dnDevInst, ULONG ulFlags);

//Setup API function pointers
BOOL (__stdcall *pSetupDiGetDeviceInterfaceDetailA)(HDEVINFO DeviceInfoSet, PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,
  PSP_DEVICE_INTERFACE_DETAIL_DATA_A DeviceInterfaceDetailData, DWORD DeviceInterfaceDetailDataSize,
  PDWORD RequiredSize, PSP_DEVINFO_DATA DeviceInfoData);
HDEVINFO (__stdcall *pSetupDiGetClassDevsA)(const GUID *ClassGuid, PCSTR Enumerator, HWND hwndParent, DWORD Flags);
BOOL (__stdcall *pSetupDiEnumDeviceInterfaces)(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData,
  const GUID *InterfaceClassGuid, DWORD MemberIndex, PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);
BOOL (__stdcall *pSetupDiDestroyDeviceInfoList)(HDEVINFO DeviceInfoSet);

// Load DLLs and lookup needed functions
BOOLEAN SpinKitLoadLibraries(void)
 {
  hHid = LoadLibraryA("hid.dll");
  hCfgMgr = LoadLibraryA("cfgmgr32.dll");
  hSetupApi = LoadLibraryA("setupapi.dll");
  if (hHid == NULL || hCfgMgr == NULL || hSetupApi == NULL)
   {
    SpinKitUnloadLibraries();
    return FALSE;
  }
  // Look up all needed functions
  pHidD_GetAttributes = (BOOLEAN (__stdcall *)(HANDLE, PHIDD_ATTRIBUTES)) GetProcAddress(hHid, "HidD_GetAttributes");
  pHidD_GetHidGuid = (void (__stdcall *)(LPGUID)) GetProcAddress(hHid, "HidD_GetHidGuid");
  pHidD_GetPreparsedData = (BOOLEAN (__stdcall *)(HANDLE, PHIDP_PREPARSED_DATA *)) GetProcAddress(hHid, "HidD_GetPreparsedData");
  pHidP_GetCaps = (DWORD (__stdcall *)(PHIDP_PREPARSED_DATA, PHIDP_CAPS)) GetProcAddress(hHid, "HidP_GetCaps");
  pHidD_FreePreparsedData = (BOOLEAN (__stdcall *)(PHIDP_PREPARSED_DATA)) GetProcAddress(hHid, "HidD_FreePreparsedData");
  pHidD_GetNumInputBuffers = (BOOLEAN (__stdcall *)(HANDLE, PULONG)) GetProcAddress(hHid, "HidD_GetNumInputBuffers");
  pHidD_SetNumInputBuffers = (BOOLEAN (__stdcall *)(HANDLE, ULONG)) GetProcAddress(hHid, "HidD_SetNumInputBuffers");
  pHidD_GetSerialNumberString = (BOOLEAN (__stdcall *)(HANDLE, PVOID, ULONG)) GetProcAddress(hHid, "HidD_GetSerialNumberString");
  pHidD_GetIndexedString = (BOOLEAN (__stdcall *)(HANDLE, ULONG, PVOID, ULONG)) GetProcAddress(hHid, "HidD_GetIndexedString");

  pCM_Get_Parent = (CONFIGRET (__stdcall *)(PDEVINST, DEVINST, ULONG)) GetProcAddress(hCfgMgr, "CM_Get_Parent");

  pSetupDiGetDeviceInterfaceDetailA = (BOOL (__stdcall *)(HDEVINFO, PSP_DEVICE_INTERFACE_DATA,
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A, DWORD, PDWORD, PSP_DEVINFO_DATA)) GetProcAddress(hSetupApi, "SetupDiGetDeviceInterfaceDetailA");
  pSetupDiGetClassDevsA = (HDEVINFO (__stdcall *)(const GUID *, PCSTR, HWND, DWORD)) GetProcAddress(hSetupApi, "SetupDiGetClassDevsA");
  pSetupDiEnumDeviceInterfaces = (BOOL (__stdcall *)(HDEVINFO, PSP_DEVINFO_DATA, const GUID *, DWORD, PSP_DEVICE_INTERFACE_DATA))
    GetProcAddress(hSetupApi, "SetupDiEnumDeviceInterfaces");
  pSetupDiDestroyDeviceInfoList = (BOOL (__stdcall *)(HDEVINFO)) GetProcAddress(hSetupApi, "SetupDiDestroyDeviceInfoList");

  // Fail if we can't look up these functions
  if (!pHidD_GetAttributes || !pHidD_GetHidGuid || !pHidD_GetSerialNumberString ||
    !pHidD_GetPreparsedData || !pHidP_GetCaps || !pHidD_FreePreparsedData || !pHidD_SetNumInputBuffers ||
    !pHidD_GetNumInputBuffers || !pHidD_GetIndexedString ||
    !pCM_Get_Parent ||
    !pSetupDiGetDeviceInterfaceDetailA || !pSetupDiGetClassDevsA || !pSetupDiEnumDeviceInterfaces || !pSetupDiDestroyDeviceInfoList)
   {
    SpinKitUnloadLibraries();
    return FALSE;
   }
  // OK, we loaded it
  return TRUE;
 }

void SpinKitUnloadLibraries(void)
 {
  // Unload HID.DLL
  if (hHid)
    FreeLibrary(hHid);
  hHid = NULL;
  // Zero function pointers
  pHidD_GetAttributes = NULL;
  pHidD_GetHidGuid = NULL;
  pHidD_GetPreparsedData = NULL;
  pHidP_GetCaps = NULL;
  pHidD_FreePreparsedData = NULL;
  pHidD_GetNumInputBuffers = NULL;
  pHidD_SetNumInputBuffers = NULL;
  pHidD_GetSerialNumberString = NULL;
  pHidD_GetIndexedString = NULL;
  // Unload CFGMGR32.DLL
  if (hCfgMgr)
    FreeLibrary(hCfgMgr);
  hCfgMgr = NULL;
  pCM_Get_Parent = NULL;
  // Unload SETUPAPI.DLL
  if (hSetupApi)
    FreeLibrary(hSetupApi);
  hSetupApi = NULL;
  pSetupDiGetDeviceInterfaceDetailA = NULL;
  pSetupDiGetClassDevsA = NULL;
  pSetupDiEnumDeviceInterfaces = NULL;
  pSetupDiDestroyDeviceInfoList = NULL;
 }

#ifdef _MANAGED
#pragma managed(pop)
#endif