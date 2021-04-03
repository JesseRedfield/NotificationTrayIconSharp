using System;

using AppKit;
using Foundation;
using NotificationIconSharp.Native;

namespace OSXDemo
{
    public partial class ViewController : NSViewController
    {
        public ViewController(IntPtr handle) : base(handle)
        {
        }

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();

            // Do any additional setup after loading the view.
            IntPtr trayIcon = IntPtr.Zero;
            trayIcon = Interop.TrayIcon_Create();
            Interop.TrayIcon_Initialize(trayIcon, "/Users/jesser/dev/NotificationTrayIconSharp/icon.jpg");
        }

        public override NSObject RepresentedObject
        {
            get
            {
                return base.RepresentedObject;
            }
            set
            {
                base.RepresentedObject = value;
                // Update the view, if already loaded.
            }
        }
    }
}
