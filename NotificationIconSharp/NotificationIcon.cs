using System;
using System.Collections.Generic;
using System.Text;
using NotificationIconSharp.Native;
using static NotificationIconSharp.Native.Interop;

namespace NotificationIconSharp
{
    /// <summary>
    /// Notification Icon Selected Delegate
    /// </summary>
    /// <param name="icon">The icon instance that was selected.</param>
    public delegate void NotificationIconSelectedCallback(NotificationIcon icon);

    public class NotificationIcon : IDisposable
    {
        private bool _disposedValue = false;
        private IntPtr _iconPtr = IntPtr.Zero;
        private MenuItemSelectedEventCallback _callbackDelegate = null;

        /// <summary>
        /// Public event, subscribe to it to respond to the notification icon
        /// being clicked. NOTE, if there are menu items, the click opens the
        /// menu and no event is fired, this is an OS level implementation
        /// detail, and can't be changed.
        /// </summary>
        public event NotificationIconSelectedCallback NotificationIconSelected;

        /// <summary>
        /// Publicly accessible list of current menu items.
        /// </summary>
        public List<NotificationMenuItem> MenuItems = new List<NotificationMenuItem>();

        /// <summary>
        /// Notification Tray Icon Constructor, supplying a path to the Icon will
        /// set it.  Passing null and calling Set Icon allows you to set one later.
        /// OSX Aceepts: PNGs, and JPGs, Recommended size 20x20px or smaller
        /// Windows Accepts: ICO icon files, recommended size, something that scales nice.
        /// </summary>
        /// <param name="iconPath"></param>
        public NotificationIcon(string iconPath)
        {
            _iconPtr = TrayIconCreate();
            if (_iconPtr == null) return;

            /// Native code will not hold a reference to the delegate
            /// don't think you're smart and remove this, the delegate will
            /// get garbage collected if you do, and then you'll get intermittent
            /// crashes :-(
            _callbackDelegate = HandleMenuItemSelected;

            TrayIconInitialize(_iconPtr, iconPath);
            TrayIconSetSelectedCallback(_iconPtr, _callbackDelegate);
        }

        public void SetIcon(string iconPath)
        {

        }

        /// <summary>
        /// Pump an OS specific message loop if not used in a GUI application
        /// already pumping one.
        /// </summary>
        /// <param name="blocking">block until next message</param>
        /// <returns>status code</returns>
        public int DoMessageLoop(bool blocking)
        {
            if (_iconPtr == IntPtr.Zero) return -1;

            return TrayIconPumpMessageLoop(_iconPtr, blocking);
        }

        /// <summary>
        /// Adds a menu item to the tray icon menu. Creates a menu if none
        /// is present.
        /// </summary>
        /// <param name="menuItem">Menu item to add to the icon menu.</param>
        /// <returns>true success, false otherwise</returns>
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

        /// <summary>
        /// Removes the requested menu item from the menu.
        /// </summary>
        /// <param name="menuItem">Menu Item to remove</param>
        /// <returns>true if item was found and removed</returns>
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

        private void HandleMenuItemSelected(IntPtr itemPtr)
        {
            NotificationIconSelected?.Invoke(this);
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
