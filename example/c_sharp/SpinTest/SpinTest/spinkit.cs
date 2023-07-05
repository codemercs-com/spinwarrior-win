using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace SpinKit
{
    public static class Defines
    {
        // IO-Warrior vendor & product IDs
        public const uint SPINKIT_VENDOR_ID = 0x07c0;
        public const uint SPINKIT_VID = SPINKIT_VENDOR_ID;

        // SpinWarrior24R4
        public const uint SPINKIT_PRODUCT_ID24R4 = 0x1200;
        public const uint SPINKIT_PID24R4 = SPINKIT_PRODUCT_ID24R4;

        // SpinWarrior24R6
        public const uint SPINKIT_PRODUCT_ID24R6 = 0x1201;
        public const uint SPINKIT_PID24R6 = SPINKIT_PRODUCT_ID24R6;

        // SpinWarrior24A3
        public const uint SPINKIT_PRODUCT_ID24A3 = 0x1202;
        public const uint SPINKIT_PID24A3 = SPINKIT_PRODUCT_ID24A3;

        // SpinWarrior28R4
        public const uint SPINKIT_PRODUCT_ID28R4 = 0x1209;
        public const uint SPINKIT_PID28R4 = SPINKIT_PRODUCT_ID28R4;

        // SpinWarrior28R6
        public const uint SPINKIT_PRODUCT_ID28R6 = 0x120A;
        public const uint SPINKIT_PID28R6 = SPINKIT_PRODUCT_ID28R6;

        // SpinWarrior28A3
        public const uint SPINKIT_PRODUCT_ID28A3 = 0x1208;
        public const uint SPINKIT_PID28A3 = SPINKIT_PRODUCT_ID28A3;
		
		// iTrain
        public const uint SPINKIT_PRODUCT_MAZERO = 0x1281;
        public const uint SPINKIT_PIDIMAZERO = SPINKIT_PRODUCT_MAZERO;

        // Max number of SpinWarriors supported
        public const uint SPINKIT_MAX_DEVICES = 16;
		
		//Length of serial number
		public const uint SPINKIT_SN_LEN = 8;
    }

    public static class Functions
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct SPINKIT_DATA
        {
            public IntPtr spinHandle;
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 6, ArraySubType = UnmanagedType.I4)] public Int32[] encoder;
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 7, ArraySubType = UnmanagedType.I4)] public Boolean[] buttons;
        }

        //Import functions from iowkit.dll (win32)
        [DllImport("spinkit.dll")] public static extern IntPtr SpinKitOpenDevice();
        [DllImport("spinkit.dll")] public static extern void SpinKitCloseDevice(IntPtr spinHandle);
        [DllImport("spinkit.dll")] public static extern bool SpinKitRead(IntPtr spinHandle, byte[] buffer);
        [DllImport("spinkit.dll")] public static extern bool SpinKitReadNonBlocking(IntPtr spinHandle, ref SPINKIT_DATA data);
        [DllImport("spinkit.dll")] public static extern UInt32 SpinKitGetNumDevs();
        [DllImport("spinkit.dll")] public static extern IntPtr SpinKitGetDeviceHandle(UInt32 numDevice);
        [DllImport("spinkit.dll")] public static extern UInt32 SpinKitGetProductId(IntPtr spinHandle);
        [DllImport("spinkit.dll")] public static extern UInt32 SpinKitGetRevision(IntPtr spinHandle);
        [DllImport("spinkit.dll", CharSet = CharSet.Unicode)] public static extern bool SpinKitGetSerialNumber(IntPtr spinHandle, StringBuilder SnTarget);
        [DllImport("spinkit.dll")] public static extern bool SpinKitSetTimeout(IntPtr spinHandle, UInt32 timeout);
        [DllImport("spinkit.dll")] public static extern string SpinKitVersion();

        //Import GetLastError (win32)
        [DllImport("kernel32.dll")] public static extern uint GetLastError();
    }
}