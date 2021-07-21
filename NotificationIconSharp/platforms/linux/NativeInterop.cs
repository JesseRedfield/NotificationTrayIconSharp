using System;
using System.Runtime.InteropServices;

namespace NotificationIconSharp.Native
{
    internal class LinInterop : INativeInterop
    {
        public const string NATIVE_LIBRARY_NAME = "not_impl.so";
        public IntPtr TrayIconCreate()
        {
            return TrayIcon_Create();
        }

        public void TrayIconInitialize(IntPtr iconHandle, string iconPath)
        {
            TrayIcon_Initialize(iconHandle, iconPath);
        }

        public void TrayIconSetTrayIcon(IntPtr iconHandle, string iconPath)
        {
            TrayIcon_SetTrayIcon(iconHandle, iconPath);
        }

        public void TrayIconSetSelectedCallback(IntPtr iconHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback)
        {
            TrayIcon_SetSelectedCallback(iconHandle, menuItemSelectedEventCallback);
        }

        public bool TrayIconAddMenuItem(IntPtr iconHandle, IntPtr menuItemHandle)
        {
            return TrayIcon_AddMenuItem(iconHandle, menuItemHandle);
        }

        public bool TrayIconRemoveMenuItem(IntPtr iconHandle, IntPtr menuItemHandle)
        {
            return TrayIcon_RemoveMenuItem(iconHandle, menuItemHandle);
        }

        public int TrayIconPumpMessageLoop(IntPtr iconHandle, bool blocking)
        {
            return TrayIcon_PumpMessageLoop(iconHandle, blocking);
        }

        public void TrayIconDestroy(IntPtr iconHandle)
        {
            TrayIcon_Destroy(iconHandle);
        }

        public IntPtr TrayMenuItemCreate(string text)
        {
            return TrayMenuItem_Create(text);
        }

        public void TrayMenuItemSetText(IntPtr menuItemHandle, string text)
        {
            TrayMenuItem_SetText(menuItemHandle, text);
        }

        public void TrayMenuItemSetSelectedCallback(IntPtr menuItemHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback)
        {
            TrayMenuItem_SetSelectedCallback(menuItemHandle, menuItemSelectedEventCallback);
        }

        public bool TrayMenuItemAddMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle)
        {
            return TrayMenuItem_AddMenuItem(targetMenuItemHandle, itemToAddHandle);
        }

        public bool TrayMenuItemRemoveMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle)
        {
            return TrayMenuItem_RemoveMenuItem(targetMenuItemHandle, itemToAddHandle);
        }

        public bool TrayMenuItemGetDisabled(IntPtr menuItemHandle)
        {
            return TrayMenuItem_GetDisabled(menuItemHandle);
        }

        public void TrayMenuItemSetDisabled(IntPtr menuItemHandle, bool bDisabled)
        {
            TrayMenuItem_SetDisabled(menuItemHandle, bDisabled);
        }

        public bool TrayMenuItemGetChecked(IntPtr menuItemHandle)
        {
            return TrayMenuItem_GetChecked(menuItemHandle);
        }

        public void TrayMenuItemSetChecked(IntPtr menuItemHandle, bool bChecked)
        {
            TrayMenuItem_SetChecked(menuItemHandle, bChecked);
        }

        public void TrayMenuItemDestroy(IntPtr menuItemHandle)
        {
            TrayMenuItem_Destroy(menuItemHandle);
        }

        public void ToastInitialize(string appId, string displayName, string iconPath)
        {
            Toast_Initialize(appId, displayName, iconPath);

            _callbackDelegate = LinInterop_notificationCallbackEvent;
            Toast_SetSelectedCallback(_callbackDelegate);
        }

        public void ToastSendNotification(string title, string text, string id, string contentImage = null)
        {
            Toast_SendNotification(title, text, id);
        }

        public event NotificationSelectedEventCallback NotificationIconSelectedEvent;

        private void LinInterop_notificationCallbackEvent([MarshalAs(UnmanagedType.LPStr)] string notificationId)
        {
            NotificationIconSelectedEvent?.Invoke(notificationId);
        }

        public void ToastUnInitialize()
        {
            Toast_UnInitialize();
        }

        private Toast_NotificationSelectedEventCallback _callbackDelegate = null;
        internal delegate void Toast_NotificationSelectedEventCallback([MarshalAs(UnmanagedType.LPStr)] string pMenuItem);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayIcon_Initialize(IntPtr iconHandle, [MarshalAs(UnmanagedType.LPStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayIcon_SetTrayIcon(IntPtr iconHandle, [MarshalAs(UnmanagedType.LPStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TrayIcon_Create();

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayIcon_SetSelectedCallback(IntPtr iconHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TrayIcon_AddMenuItem(IntPtr iconHandle, IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TrayIcon_RemoveMenuItem(IntPtr iconHandle, IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern int TrayIcon_PumpMessageLoop(IntPtr iconHandle, [MarshalAs(UnmanagedType.I1)] bool blocking);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayIcon_Destroy(IntPtr iconHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TrayMenuItem_Create([MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayMenuItem_SetText(IntPtr menuItemHandle, [MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayMenuItem_SetSelectedCallback(IntPtr menuItemHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TrayMenuItem_AddMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TrayMenuItem_RemoveMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TrayMenuItem_GetDisabled(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayMenuItem_SetDisabled(IntPtr menuItemHandle, [MarshalAs(UnmanagedType.I1)] bool bDisabled);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TrayMenuItem_GetChecked(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayMenuItem_SetChecked(IntPtr menuItemHandle, [MarshalAs(UnmanagedType.I1)] bool bChecked);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void TrayMenuItem_Destroy(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Toast_Initialize([MarshalAs(UnmanagedType.LPStr)] string appId, [MarshalAs(UnmanagedType.LPStr)] string displayName, [MarshalAs(UnmanagedType.LPStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Toast_SendNotification([MarshalAs(UnmanagedType.LPStr)] string title, [MarshalAs(UnmanagedType.LPStr)] string text, [MarshalAs(UnmanagedType.LPStr)] string id);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Toast_SetSelectedCallback(Toast_NotificationSelectedEventCallback notificationSelectedCallback);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Toast_UnInitialize();
    }
}