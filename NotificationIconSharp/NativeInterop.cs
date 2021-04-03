
using System;
using System.Net;
using System.Runtime.InteropServices;

namespace NotificationIconSharp.Native
{
    public static partial class Interop
    {
        public const string NATIVE_LIBRARY_NAME = "libnotificationicon.dylib";

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr TrayIcon_Create();

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayIcon_Initialize(IntPtr iconHandle, [MarshalAs(UnmanagedType.LPStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayIcon_SetTrayIcon(IntPtr iconHandle, [MarshalAs(UnmanagedType.LPStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayIcon_SetSelectedCallback(IntPtr iconHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool TrayIcon_AddMenuItem(IntPtr iconHandle, IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool TrayIcon_RemoveMenuItem(IntPtr iconHandle, IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int TrayIcon_PumpMessageLoop(IntPtr iconHandle, bool blocking);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayIcon_Destroy(IntPtr iconHandle);




        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr TrayIcon_Create([MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayMenuItem_SetText(IntPtr menuItemHandle, [MarshalAs(UnmanagedType.LPStr)] string text);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayMenuItem_SetSelectedCallback(IntPtr menuItemHandle, MenuItemSelectedEventCallback menuItemSelectedEventCallback);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool TrayMenuItem_AddMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool TrayMenuItem_RemoveMenuItem(IntPtr targetMenuItemHandle, IntPtr itemToAddHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool TrayMenuItem_GetDisabled(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayMenuItem_SetDisabled(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool TrayMenuItem_GetChecked(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayMenuItem_SetChecked(IntPtr menuItemHandle);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayMenuItem_Destroy(IntPtr menuItemHandle);

        public delegate void MenuItemSelectedEventCallback(IntPtr pMenuItem);
    }
}