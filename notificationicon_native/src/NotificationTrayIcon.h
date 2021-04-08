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
        EXPORT CTrayIcon *TrayIcon_Create();
        EXPORT void TrayIcon_Initialize(CTrayIcon *pTrayIcon, const CSCHAR *pszTrayIconPath);
        EXPORT void TrayIcon_SetTrayIcon(CTrayIcon *pTrayIcon, const CSCHAR *pszTrayIconPath);
        EXPORT void TrayIcon_SetSelectedCallback(CTrayIcon *pTrayIcon, MenuItemSelectedEventCallback menuItemSelectedEventCallback);
        EXPORT bool TrayIcon_AddMenuItem(CTrayIcon *pTrayIcon, CTrayMenuItem *pMenuItem);
        EXPORT bool TrayIcon_RemoveMenuItem(CTrayIcon *pTrayIcon, CTrayMenuItem *pMenuItem, bool recurse = false);
        EXPORT int TrayIcon_PumpMessageLoop(CTrayIcon *pTrayIcon, bool bBlocking);
        EXPORT void TrayIcon_Destroy(CTrayIcon *pTrayIcon);

        EXPORT CTrayMenuItem *TrayMenuItem_Create(const CSCHAR *pszText);
        EXPORT void TrayMenuItem_SetText(CTrayMenuItem *pTrayMenuItem, const CSCHAR *pszText);
        EXPORT void TrayMenuItem_SetSelectedCallback(CTrayMenuItem *pTrayMenuItem, MenuItemSelectedEventCallback menuItemSelectedEventCallback);
        EXPORT bool TrayMenuItem_AddMenuItem(CTrayMenuItem *pTargetMenuItem, CTrayMenuItem *pMenuItemToAdd);
        EXPORT bool TrayMenuItem_RemoveMenuItem(CTrayMenuItem *pTargetMenuItem, CTrayMenuItem *pMenuItemToRemove, bool recurse = false);
        EXPORT bool TrayMenuItem_GetDisabled(CTrayMenuItem *pTrayMenuItem);
        EXPORT void TrayMenuItem_SetDisabled(CTrayMenuItem *pTrayMenuItem, bool disabled);
        EXPORT bool TrayMenuItem_GetChecked(CTrayMenuItem *pTrayMenuItem);
        EXPORT void TrayMenuItem_SetChecked(CTrayMenuItem *pTrayMenuItem, bool checked);
        EXPORT void TrayMenuItem_Destroy(CTrayMenuItem *pTrayIcon);
    }
#ifdef __cplusplus
}
#endif

#endif // __NOTIFICATION_TRAY_ICON_API_H__
