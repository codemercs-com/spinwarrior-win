//
// Spinkit library V1.5 include file
//

// Spin Library definitions
#ifndef _SPINKIT_H_
#define _SPINKIT_H_

#ifdef _WIN32

#define SPINKIT_API __stdcall

#include "spinkitextra.h"

#else

#define SPINKIT_API

/*
 * Windows specific types and defines
 */
typedef unsigned int     ULONG;
typedef unsigned short   USHORT;
typedef unsigned short   WORD;
typedef unsigned char    UCHAR;
typedef unsigned char    BYTE;
typedef char *           PCHAR;
typedef unsigned short * PWCHAR;
typedef unsigned char    BOOLEAN;
typedef unsigned int     DWORD;
typedef DWORD *          PDWORD;
typedef void *           PVOID;
typedef DWORD            HANDLE;
typedef DWORD            BOOL;

#define FALSE 0
#define TRUE 1

#endif // _WIN32

// SpinWarrior vendor & product ID
#define SPINKIT_VENDOR_ID         0x07c0
#define SPINKIT_VID               SPINKIT_VENDOR_ID

#define SPINKIT_PRODUCT_ID24R4    0x1200
#define SPINKIT_PID24R4           SPINKIT_PRODUCT_ID24R4

#define SPINKIT_PRODUCT_ID24R6    0x1201
#define SPINKIT_PID24R6           SPINKIT_PRODUCT_ID24R6

#define SPINKIT_PRODUCT_ID24A3    0x1202
#define SPINKIT_PID24A3           SPINKIT_PRODUCT_ID24A3

//Spinwarrior28 product ID
#define SPINKIT_PRODUCT_ID28R4    0x1209
#define SPINKIT_PID28R4           SPINKIT_PRODUCT_ID28R4

#define SPINKIT_PRODUCT_ID28R6    0x120A
#define SPINKIT_PID28R6           SPINKIT_PRODUCT_ID28R6

#define SPINKIT_PRODUCT_ID28A3    0x1208
#define SPINKIT_PID28A3           SPINKIT_PRODUCT_ID28A3

#define SPINKIT_PRODUCT_MAZERO    0x1281
#define SPINKIT_PIDMAZERO         SPINKIT_PRODUCT_MAZERO


// Max number of SpinWarriors supported
#define SPINKIT_MAX_DEVICES  16

// Opaque SpinWarrior handle
typedef PVOID SPINKIT_HANDLE;

// Don't forget to pack it!
#pragma pack(push, 1)

typedef struct _SPINKIT_DATA
 {
  SPINKIT_HANDLE Device;
  int Spins[6];
  BOOL Buttons[7];
 }
  SPINKIT_DATA, *PSPINKIT_DATA;

#define SPINKIT_DATA_SIZE sizeof(SPINKIT_DATA)

#pragma pack(pop)

// Function prototypes

#ifdef  __cplusplus
extern "C" {
#endif // __cplusplus

SPINKIT_HANDLE SPINKIT_API SpinKitOpenDevice(void);
void SPINKIT_API SpinKitCloseDevice(SPINKIT_HANDLE spinHandle);
BOOL SPINKIT_API SpinKitRead(SPINKIT_HANDLE spinHandle, PSPINKIT_DATA SpinData);
BOOL SPINKIT_API SpinKitReadNonBlocking(SPINKIT_HANDLE spinHandle, PSPINKIT_DATA SpinData);
ULONG SPINKIT_API SpinKitGetNumDevs(void);
SPINKIT_HANDLE SPINKIT_API SpinKitGetDeviceHandle(ULONG numDevice);
ULONG SPINKIT_API SpinKitGetProductId(SPINKIT_HANDLE spinHandle);
ULONG SPINKIT_API SpinKitGetRevision(SPINKIT_HANDLE spinHandle);
BOOL SPINKIT_API SpinKitGetSerialNumber(SPINKIT_HANDLE spinHandle, PWCHAR serialNumber);
BOOL SPINKIT_API SpinKitSetTimeout(SPINKIT_HANDLE spinHandle, ULONG timeout);
PCHAR SPINKIT_API SpinKitVersion(void);

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _SPINKIT_H_
