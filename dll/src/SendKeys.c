//
// SendKeys.cpp
// IO-Warrior SDK library
//
// send key and mouse events
//

#include "stdafx.h"
#include "spindev.h"

void SPINKIT_API CMSendVirtualKeyEx(WORD Vk, BOOL KeyUp)
 {
  WORD Scan;
  DWORD dwFlags;

  switch(Vk)
   {
    case VK_LBUTTON:
      if (KeyUp)
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
      else
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      break;
    case VK_RBUTTON:
      if (KeyUp)
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
      else
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
      break;
    case VK_MBUTTON:
      if (KeyUp)
        mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
      else
        mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
      break;
    default:
      dwFlags = (KeyUp ? KEYEVENTF_KEYUP : 0);
      Scan = MapVirtualKey(Vk, 0);
      if (Scan & 0x100)
       {
        dwFlags |= KEYEVENTF_EXTENDEDKEY;
        Scan &= 0xFF;
       }
      if (Scan)
        keybd_event((BYTE) Vk, (BYTE) Scan, dwFlags, 0);
   }
 }

void SPINKIT_API CMSendScanCodeEx(WORD Scan, BOOL KeyUp)
 {
  WORD Vk;
  DWORD dwFlags;

  Scan &= 0x1FF;
  if (Scan)
   {
    Vk = MapVirtualKey(Scan, 3);
    if (Vk == 0)
      Vk = MapVirtualKey(Scan, 1);
    if (Vk != 0)
     {
      dwFlags = (KeyUp ? KEYEVENTF_KEYUP : 0);
      if (Scan & 0x100)
        {
         dwFlags |= KEYEVENTF_EXTENDEDKEY;
         Scan &= 0xFF;
        }
      keybd_event((BYTE) Vk, (BYTE) Scan, dwFlags, 0);
     }
   }
 }

void SPINKIT_API CMSendVirtualKey(WORD Vk)
 {
  CMSendVirtualKeyEx(Vk, FALSE);
  CMSendVirtualKeyEx(Vk, TRUE);
 }

void SPINKIT_API CMSendScanCode(WORD Scan)
 {
  CMSendScanCodeEx(Scan, FALSE);
  CMSendScanCodeEx(Scan, TRUE);
 }

void SPINKIT_API CMSendVirtualKeySequence(WORD *VkSeq, int Count)
 {
  int i;

  for (i = 0; i < Count; i++)
    CMSendVirtualKey(*VkSeq++);
 }

void SPINKIT_API CMSendScanCodeSequence(WORD *ScanSeq, int Count)
 {
  int i;

  for (i = 0; i < Count; i++)
    CMSendScanCode(*ScanSeq++);
 }

void SPINKIT_API CMSendVirtualKeySequenceEx(WORD *VkSeq, BOOL *KeyUp, int Count)
 {
  int i;

  if (VkSeq != NULL)
    for (i = 0; i < Count; i++)
      if (KeyUp != NULL)
        CMSendVirtualKeyEx(*VkSeq++, *KeyUp++);
      else
        CMSendVirtualKeyEx(*VkSeq++, FALSE);
 }

void SPINKIT_API CMSendScanCodeSequenceEx(WORD *ScanSeq, BOOL *KeyUp, int Count)
 {
  int i;

  if (ScanSeq != NULL)
    for (i = 0; i < Count; i++)
      if (KeyUp != NULL)
        CMSendScanCodeEx(*ScanSeq++, *KeyUp++);
      else
        CMSendScanCodeEx(*ScanSeq++, FALSE);
 }

void SPINKIT_API CMSendString(PCHAR Str)
 {
  short int N;

  while (*Str)
   {
    N = VkKeyScan(*Str++);
    if (N != -1)
     {
      if (N & 0x0100)
        CMSendVirtualKeyEx(VK_SHIFT, FALSE);
      if (N & 0x0200)
        CMSendVirtualKeyEx(VK_CONTROL, FALSE);
      if (N & 0x0400)
        CMSendVirtualKeyEx(VK_MENU, FALSE);
      CMSendVirtualKeyEx(N & 0xFF, FALSE);
      CMSendVirtualKeyEx(N & 0xFF, TRUE);
      if (N & 0x0400)
        CMSendVirtualKeyEx(VK_MENU, TRUE);
      if (N & 0x0200)
        CMSendVirtualKeyEx(VK_CONTROL, TRUE);
      if (N & 0x0100)
        CMSendVirtualKeyEx(VK_SHIFT, TRUE);
     }
   }
 }

char SPINKIT_API CMSendSpinChar(int Steps)
 {
  static int CurrChar = 0;
  static char SendStr[3] = "\b ";

  if (Steps == 0)
   {
    CurrChar = 0;
    SendStr[1] = (char) (CurrChar + 32);
    CMSendString(SendStr + 1);
   }
  else
   {
    CurrChar += Steps;
    while (CurrChar < 0)
      CurrChar += 126 - 32;
    while (CurrChar > 126 - 32)
      CurrChar-= 126 - 32;
    SendStr[1] = (char) (CurrChar + 32);
    CMSendString(SendStr);
   };
  return (SendStr[1]);
 }
