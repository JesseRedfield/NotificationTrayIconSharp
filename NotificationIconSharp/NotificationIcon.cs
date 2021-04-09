using System;
using System.Collections.Generic;
using System.Text;
using static NotificationIconSharp.Native.Interop;

namespace NotificationIconSharp
{
    public delegate void NotificationIconSelectedCallback(NotificationIcon icon);

    public class NotificationIcon : IDisposable
    {
        private bool _disposedValue = false;
        private IntPtr _iconPtr = IntPtr.Zero;

        public event NotificationIconSelectedCallback NotificationIconSelected;

        public List<NotificationMenuItem> MenuItems = new List<NotificationMenuItem>();

        public NotificationIcon(string iconPath)
        {
            _iconPtr = TrayIconCreate();
            if (_iconPtr == null) return;

            TrayIconInitialize(_iconPtr, iconPath);
            TrayIconSetSelectedCallback(_iconPtr, HandleMenuItemSelected);
        }

        public int DoMessageLoop(bool blocking)
        {
            if (_iconPtr == IntPtr.Zero) return -1;

            return TrayIconPumpMessageLoop(_iconPtr, blocking);
        }

        private void HandleMenuItemSelected(IntPtr itemPtr)
        {
            NotificationIconSelected?.Invoke(this);
        }

        public bool AddMenuItem(NotificationMenuItem menuItem)
        {
            if (_iconPtr == IntPtr.Zero) return false;

            if(TrayIconAddMenuItem(_iconPtr, menuItem.GetHandle()))
            {
                MenuItems.Add(menuItem);
                return true;
            }

            return false;
        }

        public bool RemoveMenuItem(NotificationMenuItem menuItem)
        {
            if (_iconPtr == IntPtr.Zero) return false;

            if(TrayIconRemoveMenuItem(_iconPtr, menuItem.GetHandle()))
            {
                MenuItems.Remove(menuItem);
                return true;
            }
            
            return false;
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    MenuItems = null;
                    NotificationIconSelected = null;
                }

                if(_iconPtr != IntPtr.Zero)
                {
                    TrayIconDestroy(_iconPtr);
                    _iconPtr = IntPtr.Zero;
                }

                _disposedValue = true;
            }
        }

        ~NotificationIcon()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
