//
// Spinkit library V1.5 extras include file
//

// Spin Library definitions
#ifndef _SPINKIT_EXTRA_H_
#define _SPINKIT_EXTRA_H_

#ifdef _WIN32

// Function prototypes

#ifdef  __cplusplus
extern "C" {
#endif // __cplusplus

void SPINKIT_API CMSendVirtualKeyEx(WORD Vk, BOOL KeyUp);
void SPINKIT_API CMSendScanCodeEx(WORD Scan, BOOL KeyUp);
void SPINKIT_API CMSendVirtualKey(WORD Vk);
void SPINKIT_API CMSendScanCode(WORD Scan);
void SPINKIT_API CMSendVirtualKeySequence(WORD *VkSeq, int Count);
void SPINKIT_API CMSendScanCodeSequence(WORD *ScanSeq, int Count);
void SPINKIT_API CMSendVirtualKeySequenceEx(WORD *VkSeq, BOOL *KeyUp, int Count);
void SPINKIT_API CMSendScanCodeSequenceEx(WORD *ScanSeq, BOOL *KeyUp, int Count);
void SPINKIT_API CMSendString(PCHAR Str);
char SPINKIT_API CMSendSpinChar(int Steps);

#endif // _WIN32

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _SPINKIT_EXTRA_H_
