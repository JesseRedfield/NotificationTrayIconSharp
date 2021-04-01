#ifndef __NOTIFICATION_TRAY_ICON_API_H__
#define __NOTIFICATION_TRAY_ICON_API_H__

#include "common.h"
#include <stdlib.h>

#ifdef OS_WINDOWS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace notification_tray_icon_private
{
    class CTrayIcon;
    class CTrayMenuItem;
}

using namespace notification_tray_icon_private;

#ifdef __cplusplus
extern "C"
{
#endif
    namespace notification_tray_icon
    {
        CTrayIcon *TrayIcon_Create();
        void TrayIcon_Initialize(CTrayIcon *pTrayIcon, const CSCHAR *pszTrayIconPath);
        void TrayIcon_SetTrayIcon(CTrayIcon *pTrayIcon, CCHAR *pszTrayIconPath);
        void TrayIcon_SetSelectedCallback(CTrayIcon *pTrayIcon, MenuItemSelectedEventCallback menuItemSelectedEventCallback);
        bool TrayIcon_AddMenuItem(CTrayIcon *pTrayIcon);
        bool TrayIcon_RemoveMenuItem(CTrayIcon *pTrayIcon, bool recurse = false);
        int TrayIcon_PumpMessageLoop(CTrayIcon *pTrayIcon, bool bBlocking);
        void TrayIcon_Destroy(CTrayIcon *pTrayIcon);

        CTrayMenuItem TrayMenuItem_Create(CCHAR *pszText);
        void TrayMenuItem_SetText(CTrayMenuItem pTrayMenuItem, CCHAR *pszText);
        void TrayMenuItem_SetSelectedCallback(CTrayMenuItem *pTrayMenuItem, MenuItemSelectedEventCallback menuItemSelectedEventCallback);
        bool TrayMenuItem_AddMenuItem(CTrayMenuItem *pTargetMenuItem, CTrayMenuItem *pMenuItemToAdd);
        bool TrayMenuItem_RemoveMenuItem(CTrayMenuItem *pTargetMenuItem, CTrayMenuItem *pMenuItemToRemove, bool recurse);
        bool TrayMenuItem_GetDisabled(CTrayMenuItem *pTrayMenuItem);
        void TrayMenuItem_SetDisabled(CTrayMenuItem *pTrayMenuItem, bool disabled);
        bool TrayMenuItem_GetChecked(CTrayMenuItem *pTrayMenuItem);
        void TrayMenuItem_SetChecked(CTrayMenuItem *pTrayMenuItem, bool checked);
        void TrayMenuItem_Destroy(CTrayMenuItem *pTrayIcon);
    }
#ifdef __cplusplus
}
#endif

#endif // __NOTIFICATION_TRAY_ICON_API_H__