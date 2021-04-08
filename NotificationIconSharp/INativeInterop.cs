
using System;
using System.Runtime.InteropServices;

namespace NotificationIconSharp.Native
{
    internal delegate void MenuItemSelectedEventCallback(IntPtr pMenuItem);

    internal interface INativeInterop
    {
        IntPtr TrayIconCreate();

        void TrayIconInitialize(IntPtr iconHandle, string iconPath);

        void TrayIconSetTrayIcon(IntPtr iconHandle, string iconPath);

        void TrayIconSetSelectedCallback(IntPtr iconHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback);

        bool TrayIconAddMenuItem(IntPtr iconHandle, IntPtr menuItemHandle);

        bool TrayIconRemoveMenuItem(IntPtr iconHandle, IntPtr menuItemHandle);

        int TrayIconPumpMessageLoop(IntPtr iconHandle, bool blocking);

        void TrayIconDestroy(IntPtr iconHandle);

        IntPtr TrayMenuItemCreate(string text);

        void TrayMenuItemSetText(IntPtr menuItemHandle, string text);

        void TrayMenuItemSetSelectedCallback(IntPtr menuItemHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback);

        bool TrayMenuItemAddMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle);

        bool TrayMenuItemRemoveMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle);

        bool TrayMenuItemGetDisabled(IntPtr menuItemHandle);

        void TrayMenuItemSetDisabled(IntPtr menuItemHandle, bool bDisabled);

        bool TrayMenuItemGetChecked(IntPtr menuItemHandle);

        void TrayMenuItemSetChecked(IntPtr menuItemHandle, bool bChecked);

        void TrayMenuItemDestroy(IntPtr menuItemHandle);
    }
}