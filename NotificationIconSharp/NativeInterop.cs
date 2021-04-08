
using System;
using System.Runtime.InteropServices;

namespace NotificationIconSharp.Native
{
    internal static class Interop
    {
        private static INativeInterop _impl;

        private static void Initialize()
        {
            if (_impl != null) return;

            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                _impl = new WinInterop();
                return;
            }
            if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            {
                _impl = new OsxInterop();
                return;
            }

            //Not currently implemented
            throw new PlatformNotSupportedException();
        }

        public static IntPtr TrayIconCreate()
        {
            Initialize();
            return _impl.TrayIconCreate();
        }

        public static void TrayIconInitialize(IntPtr iconHandle, string iconPath)
        {
            Initialize();
            _impl.TrayIconInitialize(iconHandle, iconPath);
        }

        public static void TrayIconSetTrayIcon(IntPtr iconHandle, string iconPath)
        {
            Initialize();
            _impl.TrayIconSetTrayIcon(iconHandle, iconPath);
        }

        public static void TrayIconSetSelectedCallback(IntPtr iconHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback)
        {
            Initialize();
            _impl.TrayIconSetSelectedCallback(iconHandle, menuItemSelectedEventCallback);
        }

        public static bool TrayIconAddMenuItem(IntPtr iconHandle, IntPtr menuItemHandle)
        {
            Initialize();
            return _impl.TrayIconAddMenuItem(iconHandle, menuItemHandle);
        }

        public static bool TrayIconRemoveMenuItem(IntPtr iconHandle, IntPtr menuItemHandle)
        {
            Initialize();
            return _impl.TrayIconRemoveMenuItem(iconHandle, menuItemHandle);
        }

        public static int TrayIconPumpMessageLoop(IntPtr iconHandle, bool blocking)
        {
            Initialize();
            return _impl.TrayIconPumpMessageLoop(iconHandle, blocking);
        }

        public static void TrayIconDestroy(IntPtr iconHandle)
        {
            Initialize();
            _impl.TrayIconDestroy(iconHandle);
        }

        public static IntPtr TrayMenuItemCreate(string text)
        {
            Initialize();
            return _impl.TrayMenuItemCreate(text);
        }

        public static void TrayMenuItemSetText(IntPtr menuItemHandle, string text)
        {
            Initialize();
            _impl.TrayMenuItemSetText(menuItemHandle, text);
        }

        public static void TrayMenuItemSetSelectedCallback(IntPtr menuItemHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback)
        {
            Initialize();
            _impl.TrayMenuItemSetSelectedCallback(menuItemHandle, menuItemSelectedEventCallback);

        }

        public static bool TrayMenuItemAddMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle)
        {
            Initialize();
            return _impl.TrayMenuItemAddMenuItem(targetMenuItemHandle, itemToAddHandle);
        }

        public static bool TrayMenuItemRemoveMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle)
        {
            Initialize();
            return _impl.TrayMenuItemRemoveMenuItem(targetMenuItemHandle, itemToAddHandle);

        }

        public static bool TrayMenuItemGetDisabled(IntPtr menuItemHandle)
        {
            Initialize();
            return _impl.TrayMenuItemGetDisabled(menuItemHandle);
        }

        public static void TrayMenuItemSetDisabled(IntPtr menuItemHandle, bool bDisabled)
        {
            Initialize();
            _impl.TrayMenuItemSetDisabled(menuItemHandle, bDisabled);
        }

        public static bool TrayMenuItemGetChecked(IntPtr menuItemHandle)
        {
            Initialize();
            return _impl.TrayMenuItemGetChecked(menuItemHandle);
        }

        public static void TrayMenuItemSetChecked(IntPtr menuItemHandle, bool bChecked)
        {
            Initialize();
            _impl.TrayMenuItemSetChecked(menuItemHandle, bChecked);
        }

        public static void TrayMenuItemDestroy(IntPtr menuItemHandle)
        {
            Initialize();
            _impl.TrayMenuItemDestroy(menuItemHandle);
        }
    }
}