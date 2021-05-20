using System;
using System.Collections.Generic;
using NotificationIconSharp.Native;
using static NotificationIconSharp.Native.Interop;

namespace NotificationIconSharp
{
    public delegate void NotificationMenuItemCallback(NotificationMenuItem  menuItem);

    public class NotificationMenuItem : IDisposable
    {
        private string _text;
        private bool _disposedValue;
        private IntPtr _menuPtr = IntPtr.Zero;

        public event NotificationMenuItemCallback NotificationMenuItemSelected;
        private MenuItemSelectedEventCallback _callbackDelegate = null;
        public List<NotificationMenuItem> MenuItems = new List<NotificationMenuItem>();

        public NotificationMenuItem(string text)
        {
            _menuPtr = TrayMenuItemCreate(text);
            if (_menuPtr == IntPtr.Zero) return;
            _text = text;
            _callbackDelegate = HandleMenuItemSelected;
            TrayMenuItemSetSelectedCallback(_menuPtr, _callbackDelegate);
        }

        private void HandleMenuItemSelected(IntPtr itemPtr)
        {
            NotificationMenuItemSelected?.Invoke(this);
        }

        public string Text
        {
            get
            {
                if (_menuPtr == IntPtr.Zero) return string.Empty;
                return _text;
            }

            set
            {
                if (_menuPtr == IntPtr.Zero) return;

                TrayMenuItemSetText(_menuPtr, value);
                _text = value;
            }
        }

        public bool Disabled
        {
            get
            {
                if (_menuPtr == IntPtr.Zero) return false;

                return TrayMenuItemGetDisabled(_menuPtr);
            }

            set
            {
                if (_menuPtr == IntPtr.Zero) return;

                TrayMenuItemSetDisabled(_menuPtr, value);
            }
        }

        public bool Checked
        {
            get
            {
                if (_menuPtr == IntPtr.Zero) return false;

                return TrayMenuItemGetChecked(_menuPtr);
            }

            set
            {
                if (_menuPtr == IntPtr.Zero) return;

                TrayMenuItemSetChecked(_menuPtr, value);
            }
        }

        public bool AddMenuItem(NotificationMenuItem menuItem)
        {
            if (_menuPtr == IntPtr.Zero) return false;

            if (TrayMenuItemAddMenuItem(_menuPtr, menuItem.GetHandle()))
            {
                MenuItems.Add(menuItem);
                return true;
            }

            return false;
        }

        public bool RemoveMenuItem(NotificationMenuItem menuItem)
        {
            if (_menuPtr == IntPtr.Zero) return false;

            if (TrayMenuItemRemoveMenuItem(_menuPtr, menuItem.GetHandle()))
            {
                MenuItems.Remove(menuItem);
                return true;
            }

            return false;
        }

        internal IntPtr GetHandle()
        {
            return _menuPtr;
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    MenuItems = null;
                    NotificationMenuItemSelected = null;
                }

                if (_menuPtr != IntPtr.Zero)
                {
                    TrayMenuItemDestroy(_menuPtr);
                    _menuPtr = IntPtr.Zero;
                }
                _disposedValue = true;
            }
        }

        ~NotificationMenuItem()
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