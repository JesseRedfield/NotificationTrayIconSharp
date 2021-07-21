using System;
using System.Collections.Generic;
using NotificationIconSharp.Native;
using static NotificationIconSharp.Native.Interop;

namespace NotificationIconSharp
{
    public delegate void NotificationSelectedEventCallback(string notificationId);

    public class NotificationManager : IDisposable
    {
        //singleton reference to call dispose when the app cleans up
        private static NotificationManager _instance;

        private NotificationManager()
        {

        }

        public static void Initialize(string appId, string displayName, string iconPath = null)
        {
            _instance = new NotificationManager();
            ToastInitialize(appId, displayName, iconPath);
        }

        public static event NotificationSelectedEventCallback NotificationIconSelectedEvent
        {
            add { Interop.NotificationIconSelectedEvent += value; }
            remove { Interop.NotificationIconSelectedEvent -= value; }
        }

        public static void SendNotification(string title, string text, string id, string contentImage = null)
        {
            if (_instance == null)
                throw new Exception("Notification Manager is not Initialized,call the static initialize class before using.");

            ToastSendNotification(title, text, id, contentImage);
        }

        public void Dispose()
        {
            ToastUnInitialize();
        }
    }
}