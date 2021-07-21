#ifndef __CTOAST_H__
#define __CTOAST_H__
#include "../common.h"

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    typedef void C_NSUserNotification;

    class Toast
    {
    public:
        static void Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName = NULL, const CSCHAR *pszIconPath = NULL);

        static void SendNotification(const CSCHAR *pszTitle, const CSCHAR *pszText, const CSCHAR *pszNotificationId, const CSCHAR *pszIconPath = NULL);

        static void UnInitialize();

        static void HandleCallback(C_NSUserNotification *pUserNotification);
        
        static void SetSelectedCallback(NotificationSelectedEventCallback callback);
        
    private:
        static NotificationSelectedEventCallback NotificationSelectedCallback;
        static bool Initialized;
    };
}

#endif // __CTOAST_H__
