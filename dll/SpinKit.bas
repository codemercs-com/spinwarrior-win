Attribute VB_Name = "SpinKit"
' Spinkit library V1.5 include file

' SpinWarrior vendor & product ID
Public Const SPINKIT_VENDOR_ID As Long = &H7C0
Public Const SPINKIT_VID As Long = SPINKIT_VENDOR_ID

Public Const SPINKIT_PRODUCT_ID24R4 As Long = &H1200
Public Const SPINKIT_PID24R4 As Long = SPINKIT_PRODUCT_ID24R4

Public Const SPINKIT_PRODUCT_ID24R6 As Long = &H1201
Public Const SPINKIT_PID24R6 As Long = SPINKIT_PRODUCT_ID24R6

Public Const SPINKIT_PRODUCT_ID24A3 As Long = &H1202
Public Const SPINKIT_PID24A3 As Long = SPINKIT_PRODUCT_ID24A3

Public Const SPINKIT_PRODUCT_ID28R4 As Long = &H1209
Public Const SPINKIT_PID28R4 As Long = SPINKIT_PRODUCT_ID28R4

Public Const SPINKIT_PRODUCT_ID28R6 As Long = &H120A
Public Const SPINKIT_PID28R6 As Long = SPINKIT_PRODUCT_ID28R6

Public Const SPINKIT_PRODUCT_ID28A3 As Long = &H1208
Public Const SPINKIT_PID28A3 As Long = SPINKIT_PRODUCT_ID28A3

Public Const SPINKIT_PRODUCT_MAZERO As Long = &H1281
Public Const SPINKIT_PIDIMAZERO As Long = SPINKIT_PRODUCT_MAZERO


' Max number of SpinWarriors supported
Public Const SPINKIT_MAX_DEVICES As Long = 16

Public Type SPINKIT_DATA
  Device As Long
  Spins(5) As Long
  Buttons(6) As Long
End Type

Public Declare Function SpinKitOpenDevice Lib "spinkit" () As Long
Public Declare Sub SpinKitCloseDevice Lib "spinkit" (ByVal spinHandle As Long)

Public Declare Function SpinKitRead Lib "spinkit" (ByVal spinHandle As Long, ByRef SpinData As SPINKIT_DATA) As Long
Public Declare Function SpinKitReadNonBlocking Lib "spinkit" (ByVal spinHandle As Long, ByRef SpinData As SPINKIT_DATA) As Long
Public Declare Function SpinKitGetNumDevs Lib "spinkit" () As Long
Public Declare Function SpinKitGetDeviceHandle Lib "spinkit" (ByVal numDevice As Long) As Long
Public Declare Function SpinKitGetProductId Lib "spinkit" (ByVal spinHandle As Long) As Long
Public Declare Function SpinKitGetRevision Lib "spinkit" (ByVal spinHandle As Long) As Long
Public Declare Function SpinKitGetSerialNumber Lib "spinkit" (ByVal spinHandle As Long, ByRef serialNumber As Byte) As Long
Public Declare Function SpinKitSetTimeout Lib "spinkit" (ByVal spinHandle As Long, ByVal timeout As Long) As Long
'PCHAR SpinKitVersion(void);
