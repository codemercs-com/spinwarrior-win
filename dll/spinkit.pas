unit spinkit;

interface

uses
  Windows;

const
  // SpinWarrior vendor & product ID
  SPINKIT_VENDOR_ID      = $07c0;
  SPINKIT_VID            = SPINKIT_VENDOR_ID;

  SPINKIT_PRODUCT_ID24R4 = $1200;
  SPINKIT_PID24R4        = SPINKIT_PRODUCT_ID24R4;

  SPINKIT_PRODUCT_ID24R6 = $1201;
  SPINKIT_PID24R6        = SPINKIT_PRODUCT_ID24R6;

  SPINKIT_PRODUCT_ID24A3 = $1202;
  SPINKIT_PID24A3        = SPINKIT_PRODUCT_ID24A3;
  
  SPINKIT_PRODUCT_ID28R4 = $1209;
  SPINKIT_PID28R4        = SPINKIT_PRODUCT_ID28R4;

  SPINKIT_PRODUCT_ID28R6 = $120A;
  SPINKIT_PID28R6        = SPINKIT_PRODUCT_ID28R6;

  SPINKIT_PRODUCT_ID28A3 = $1208;
  SPINKIT_PID28A3        = SPINKIT_PRODUCT_ID28A3;
  
  SPINKIT_PRODUCT_IDMAZERO = $1281;
  SPINKIT_PIDMAZERO        = SPINKIT_PRODUCT_IDMAZERO;

  // Max number of SpinWarriors supported
  SPINKIT_MAX_DEVICES  = 16;

type
  // Opaque SpinWarrior handle
  SPINKIT_HANDLE = Pointer;

  PSPINKIT_DATA = ^SPINKIT_DATA;
  SPINKIT_DATA = packed record
    Device: SPINKIT_HANDLE;
    Spins: array [0..5] of Integer;
    Buttons: array [0..6] of BOOL;
  end;

const
  SPINKIT_DATA_SIZE = SizeOf(SPINKIT_DATA);

function SpinKitOpenDevice: SPINKIT_HANDLE; stdcall;
procedure SpinKitCloseDevice(spinHandle: SPINKIT_HANDLE); stdcall;
function SpinKitRead(spinHandle: SPINKIT_HANDLE; var SpinData: SPINKIT_DATA): BOOL; stdcall;
function SpinKitReadNonBlocking(spinHandle: SPINKIT_HANDLE; var SpinData: SPINKIT_DATA): BOOL; stdcall;
function SpinKitGetNumDevs: ULONG; stdcall;
function SpinKitGetDeviceHandle(numDevice: ULONG): SPINKIT_HANDLE; stdcall;
function SpinKitGetProductId(spinHandle: SPINKIT_HANDLE): ULONG; stdcall;
function SpinKitGetRevision(spinHandle: SPINKIT_HANDLE): ULONG; stdcall;
function SpinKitGetThreadHandle(spinHandle: SPINKIT_HANDLE): THandle; stdcall;
function SpinKitGetSerialNumber(spinHandle: SPINKIT_HANDLE; serialNumber: PWideChar): BOOL; stdcall;
function SpinKitSetTimeout(spinHandle: SPINKIT_HANDLE; timeout: ULONG): BOOL; stdcall;
function SpinKitVersion: PChar; stdcall;

procedure CMSendVirtualKeyEx(Vk: WORD; KeyUp: BOOL); stdcall;
procedure CMSendScanCodeEx(Scan: WORD; KeyUp: BOOL); stdcall;
procedure CMSendVirtualKey(Vk: WORD); stdcall;
procedure CMSendScanCode(Scan: WORD); stdcall;
procedure CMSendVirtualKeySequence(VkSeq: PWORD; Count: Integer); stdcall;
procedure CMSendScanCodeSequence(ScanSeq: PWORD; Count: Integer); stdcall;
procedure CMSendVirtualKeySequenceEx(VkSeq: PWORD; KeyUp: PBOOL; Count: Integer); stdcall;
procedure CMSendScanCodeSequenceEx(ScanSeq: PWORD; KeyUp: PBOOL; Count: Integer); stdcall;
procedure CMSendString(Str: PChar); stdcall;
function CMSendSpinChar(Steps: Integer): Char; stdcall;

implementation

const
  IOWKITDllName = 'spinkit.dll';

function SpinKitOpenDevice; external IOWKITDllName name 'SpinKitOpenDevice';
procedure SpinKitCloseDevice; external IOWKITDllName name 'SpinKitCloseDevice';
function SpinKitRead; external IOWKITDllName name 'SpinKitRead';
function SpinKitReadNonBlocking; external IOWKITDllName name 'SpinKitReadNonBlocking';
function SpinKitGetNumDevs; external IOWKITDllName name 'SpinKitGetNumDevs';
function SpinKitGetDeviceHandle; external IOWKITDllName name 'SpinKitGetDeviceHandle';
function SpinKitGetProductId; external IOWKITDllName name 'SpinKitGetProductId';
function SpinKitGetRevision; external IOWKITDllName name 'SpinKitGetRevision';
function SpinKitGetThreadHandle; external IOWKITDllName name 'SpinKitGetThreadHandle';
function SpinKitGetSerialNumber; external IOWKITDllName name 'SpinKitGetSerialNumber';
function SpinKitSetTimeout; external IOWKITDllName name 'SpinKitSetTimeout';
function SpinKitVersion; external IOWKITDllName name 'SpinKitVersion';

procedure CMSendVirtualKeyEx; external IOWKITDllName name 'CMSendVirtualKeyEx';
procedure CMSendScanCodeEx; external IOWKITDllName name 'CMSendScanCodeEx';
procedure CMSendVirtualKey; external IOWKITDllName name 'CMSendVirtualKey';
procedure CMSendScanCode; external IOWKITDllName name 'CMSendScanCode';
procedure CMSendVirtualKeySequence; external IOWKITDllName name 'CMSendVirtualKeySequence';
procedure CMSendScanCodeSequence; external IOWKITDllName name 'CMSendScanCodeSequence';
procedure CMSendVirtualKeySequenceEx; external IOWKITDllName name 'CMSendVirtualKeySequenceEx';
procedure CMSendScanCodeSequenceEx; external IOWKITDllName name 'CMSendScanCodeSequenceEx';
procedure CMSendString; external IOWKITDllName name 'CMSendString';
function CMSendSpinChar; external IOWKITDllName name 'CMSendSpinChar';

end.
