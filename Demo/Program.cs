using System;
using System.Threading.Tasks;
using AppKit;
using NotificationIconSharp.Native;

namespace Demo
{
    class Program
    {
        static void Main(string[] args)
        {
            //Console.WriteLine("Hello World!");
            //NSApplication.Main(args);
            //NSApplication.Main(args);


            
            IntPtr trayIcon = IntPtr.Zero;
            //Task.Run(() =>
            //{
                trayIcon = Interop.TrayIcon_Create();
                Interop.TrayIcon_Initialize(trayIcon, "/Users/jesser/dev/NotificationTrayIconSharp/icon.jpg");
            //});

            while (true)
            {
                Interop.TrayIcon_PumpMessageLoop(trayIcon, false);
            }
        }
    }
}
