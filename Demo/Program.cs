using System;
using NotificationIconSharp;

namespace Demo
{
    class Program
    {
        //SET AN ICON PATH HERE, BMP/ICO on WINDOWS, PNG/JPG on OSX
        const string icon_path = "PATH/TO/YOUR/ICON";

        static bool destroy = false;
        static void Main(string[] args)
        {           
            var trayIcon = new NotificationIcon(icon_path);
            trayIcon.NotificationIconSelected += TrayIcon_NotificationIconSelected;

            while (true)
            {
                trayIcon?.DoMessageLoop(true);

                if(destroy)
                {
                    trayIcon?.Dispose();
                    trayIcon = null;
                }
            }
        }

        private static void TrayIcon_NotificationIconSelected(NotificationIcon icon)
        {
            if (icon.MenuItems.Count > 0) return;

            var checkMenuItem = new NotificationMenuItem("Set Check On Clicked");
            checkMenuItem.NotificationMenuItemSelected += CheckMenuItem_NotificationMenuItemSelected;

            var disableMenuItem = new NotificationMenuItem("Disable On Clicked");
            disableMenuItem.NotificationMenuItemSelected += DisableMenuItem_NotificationMenuItemSelected;

            var setTextMenuItem = new NotificationMenuItem("Set Text On Click");
            setTextMenuItem.NotificationMenuItemSelected += SetTextMenuItem_NotificationMenuItemSelected;

            var subMenuMenuItem = new NotificationMenuItem("Create Sub Menu On Click");
            subMenuMenuItem.NotificationMenuItemSelected += SubMenuMenuItem_NotificationMenuItemSelected;

            icon.AddMenuItem(checkMenuItem);
            icon.AddMenuItem(disableMenuItem);
            icon.AddMenuItem(setTextMenuItem);
            icon.AddMenuItem(subMenuMenuItem);
        }

        private static void CheckMenuItem_NotificationMenuItemSelected(NotificationMenuItem menuItem)
        {
            menuItem.Checked = true;
        }

        private static void DisableMenuItem_NotificationMenuItemSelected(NotificationMenuItem menuItem)
        {
            menuItem.Disabled = !menuItem.Disabled;
        }

        private static void SetTextMenuItem_NotificationMenuItemSelected(NotificationMenuItem menuItem)
        {
            menuItem.Text = "Hello World!";
        }

        private static void SubMenuMenuItem_NotificationMenuItemSelected(NotificationMenuItem menuItem)
        {
            if (menuItem.MenuItems.Count > 0) return;

            var destroyMenuItem = new NotificationMenuItem("Destroy Menu");
            destroyMenuItem.NotificationMenuItemSelected += DestroyMenuItem_NotificationMenuItemSelected;
            menuItem.AddMenuItem(destroyMenuItem);
        }

        private static void DestroyMenuItem_NotificationMenuItemSelected(NotificationMenuItem icon)
        {
            destroy = true;
        }
    }
}
