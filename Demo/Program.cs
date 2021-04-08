using System;
using NotificationIconSharp.Native;

namespace Demo
{
    class Program
    {
        static void Main(string[] args)
        {           
            IntPtr trayIcon = IntPtr.Zero;

            trayIcon = Interop.TrayIcon_Create();
            Interop.TrayIcon_Initialize(trayIcon, @"C:\dev\NotificationTrayIconSharp\icon.ico");

            while (true)
            {
                Interop.TrayIcon_PumpMessageLoop(trayIcon, false);
            }
        }
    }
}
