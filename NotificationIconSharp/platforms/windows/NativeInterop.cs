using System;
using System.Runtime.InteropServices;

namespace NotificationIconSharp.Native
{
    public static partial class Interop
    {
        public const string NATIVE_LIBRARY_NAME = "notificationicon.dll";

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayIcon_Initialize(IntPtr iconHandle, [MarshalAs(UnmanagedType.LPWStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayIcon_SetTrayIcon(IntPtr iconHandle, [MarshalAs(UnmanagedType.LPWStr)] string iconPath);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr TrayIcon_Create([MarshalAs(UnmanagedType.LPWStr)] string text);

        [DllImport(NATIVE_LIBRARY_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrayMenuItem_SetText(IntPtr menuItemHandle, [MarshalAs(UnmanagedType.LPWStr)] string text);
    }
}