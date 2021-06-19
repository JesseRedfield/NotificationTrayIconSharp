#ifndef __CTOAST_H__
#define __CTOAST_H__
#include "../common.h"
#include <Windows.h>
#include <string>

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class Toast
    {
    public:
        static void Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName, const CSCHAR *pszIconPath = NULL);

        static void SendNotification(const CSCHAR *pszTitle, const CSCHAR *pszText, const CSCHAR *pszNotificationId, const CSCHAR *pszIconPath = NULL);

        static void UnInitialize();
    };
}

#endif // __CTOAST_H__
