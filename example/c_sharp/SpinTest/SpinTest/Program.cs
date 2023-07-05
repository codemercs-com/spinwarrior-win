/*
 * Simple demo to read data from the SpinWarrior family in C#.
 */

using System.Text;
using static SpinKit.Functions;
using static SpinKit.Defines;

IntPtr m_Handle = SpinKit.Functions.SpinKitOpenDevice();
SPINKIT_DATA m_Data = new SPINKIT_DATA();
short[] serial = new short[9];
uint pid = 0;
uint encoder_count = 0;
uint switch_count = 0;

//Get first device and HANDLE found.
if (m_Handle != IntPtr.Zero)
{
    if(SpinKitGetNumDevs() == 1)
    {
        Console.WriteLine("Single SpinWarrior Found");

        SpinKitSetTimeout(m_Handle, 1000); //Timeout, if you use Read instead of ReadNonBlocking
        pid = SpinKitGetProductId(m_Handle);

        switch(pid)
        {
            case SPINKIT_PID24A3: //Obsolete device
                Console.Write("SpinWarrior24 A3 found. SN: ");
                encoder_count = 3;
                switch_count = 6;
                break;
            case SPINKIT_PID24R4: //Obsolete device
                Console.Write("SpinWarrior24 R4 found. SN: ");
                encoder_count = 4;
                switch_count = 7;
                break;
            case SPINKIT_PID24R6: //Obsolete device
                Console.Write("SpinWarrior24 R6 found. SN: ");
                encoder_count = 6;
                switch_count = 3;
                break;
            case SPINKIT_PID28A3:
                Console.Write("SpinWarrior28 A3 found. SN: ");
                encoder_count = 3;
                switch_count = 6;
                break;
            case SPINKIT_PID28R4:
                Console.Write("SpinWarrior28 R4 found. SN: ");
                encoder_count = 4;
                switch_count = 7;
                break;
            case SPINKIT_PID28R6:
                Console.Write("SpinWarrior28 R6 found. SN: ");
                encoder_count = 6;
                switch_count = 3;
                break;
        }

        StringBuilder sn = new StringBuilder();
        SpinKitGetSerialNumber(m_Handle, sn);
        Console.WriteLine(sn);

        //Read 100x the SpinWarrior
        for (int i = 0; i < 100; i++)
        {
            //Read data non blocking from SpinWarrior
            SpinKitReadNonBlocking(m_Handle, ref m_Data); 

            Console.Write("Spin Data: ");

            //Get encoder
            for (int j = 0; j < encoder_count; j++)
            {
                Console.Write(m_Data.encoder[j]);
                Console.Write(" | ");
            }

            //Get buttons as bool array
            for (int j = 0; j < switch_count; j++)
            {
                if (m_Data.buttons[j] == true)
                    Console.Write("1");
                else
                    Console.Write("0");
            }

            Console.WriteLine("");
            Thread.Sleep(100);
        }
    }
    else
    {
        Console.WriteLine("Multiple SpinWarrior Found. For this demo please connect only one device");
    }
}
else
{
    Console.WriteLine("No SpinWarrior found");
}

//Don't forget to close the spinkit.dll
SpinKit.Functions.SpinKitCloseDevice(m_Handle);