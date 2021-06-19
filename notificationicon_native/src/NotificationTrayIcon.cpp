#include "NotificationTrayIcon.h"

#ifdef OS_WIN32
#include "windows/TrayMenuItem.h"
#include "windows/TrayIcon.h"
#include "windows/Toast.h"
#endif
#ifdef OS_OSX
#include "macos/TrayMenuItem.h"
#include "macos/TrayIcon.h"
#include "macos/Toast.h"
#endif

using namespace notification_tray_icon_private;

namespace notification_tray_icon
{
    CTrayIcon *TrayIcon_Create()
    {
        CTrayIcon *pNewIcon = new CTrayIcon();
        return pNewIcon;
    }

    void TrayIcon_Initialize(CTrayIcon *pTrayIcon, const CSCHAR *pszTrayIconPath)
    {
        if (pTrayIcon == NULL)
            return;

        pTrayIcon->InitializeMenu(pszTrayIconPath);
    }

    void TrayIcon_SetTrayIcon(CTrayIcon *pTrayIcon, const CSCHAR *pszTrayIconPath)
    {
        if (pTrayIcon == NULL)
            return;

        pTrayIcon->SetIcon(pszTrayIconPath);
    }

    void TrayIcon_SetSelectedCallback(CTrayIcon *pTrayIcon, MenuItemSelectedEventCallback menuItemSelectedEventCallback)
    {
        if (pTrayIcon == NULL)
            return;

        pTrayIcon->SetSelectedCallback(menuItemSelectedEventCallback);
    }

    bool TrayIcon_AddMenuItem(CTrayIcon *pTrayIcon, CTrayMenuItem *pTrayMenuItem)
    {
        if (pTrayIcon == NULL || pTrayMenuItem == NULL)
            return false;

        return pTrayIcon->AddMenuItem(pTrayMenuItem);
    }

    bool TrayIcon_RemoveMenuItem(CTrayIcon *pTrayIcon, CTrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if (pTrayIcon == NULL)
            return false;

        return pTrayIcon->RemoveMenuItem(pTrayMenuItem, recurse);
    }

    int TrayIcon_PumpMessageLoop(CTrayIcon *pTrayIcon, bool bBlocking)
    {
        if (pTrayIcon == NULL)
            return -1;

        return pTrayIcon->MessageLoop(bBlocking);
    }

    void TrayIcon_Destroy(CTrayIcon *pTrayIcon)
    {
        if (pTrayIcon != NULL)
            delete pTrayIcon;
    }

    CTrayMenuItem *TrayMenuItem_Create(const CSCHAR *pszText)
    {
        return new CTrayMenuItem(pszText);
    }

    void TrayMenuItem_SetText(CTrayMenuItem *pTrayMenuItem, const CSCHAR *pszText)
    {
        if (pTrayMenuItem == NULL)
            return;

        pTrayMenuItem->SetText(pszText);
    }

    void TrayMenuItem_SetSelectedCallback(CTrayMenuItem *pTrayMenuItem, MenuItemSelectedEventCallback menuItemSelectedEventCallback)
    {
        if (pTrayMenuItem == NULL)
            return;

        pTrayMenuItem->SetSelectedCallback(menuItemSelectedEventCallback);
    }

    bool TrayMenuItem_AddMenuItem(CTrayMenuItem *pTargetMenuItem, CTrayMenuItem *pMenuItemToAdd)
    {
        if (pTargetMenuItem == NULL || pMenuItemToAdd == NULL)
            return false;

        return pTargetMenuItem->AddMenuItem(pMenuItemToAdd);
    }

    bool TrayMenuItem_RemoveMenuItem(CTrayMenuItem *pTargetMenuItem, CTrayMenuItem *pMenuItemToRemove, bool recurse)
    {
        if (pTargetMenuItem == NULL || pMenuItemToRemove == NULL)
            return false;

        return pTargetMenuItem->RemoveMenuItem(pMenuItemToRemove, recurse);
    }

    bool TrayMenuItem_GetDisabled(CTrayMenuItem *pTrayMenuItem)
    {
        if (pTrayMenuItem == NULL)
            return false;

        return pTrayMenuItem->GetDisabled();
    }

    void TrayMenuItem_SetDisabled(CTrayMenuItem *pTrayMenuItem, bool disabled)
    {
        if (pTrayMenuItem == NULL)
            return;

        pTrayMenuItem->SetDisabled(disabled);
    }

    bool TrayMenuItem_GetChecked(CTrayMenuItem *pTrayMenuItem)
    {
        if (pTrayMenuItem == NULL)
            return false;

        return pTrayMenuItem->GetChecked();
    }

    void TrayMenuItem_SetChecked(CTrayMenuItem *pTrayMenuItem, bool checked)
    {
        if (pTrayMenuItem == NULL)
            return;

        pTrayMenuItem->SetChecked(checked);
    }

    void TrayMenuItem_Destroy(CTrayMenuItem *pTrayMenuItem)
    {
        if (pTrayMenuItem != NULL)
            delete pTrayMenuItem;
    }

    EXPORT void Toast_Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName, const CSCHAR *pszIconPath)
    {
        Toast::Initialize(pszAppId, pszDisplayName, pszIconPath);
    }

    EXPORT void Toast_SendNotification(const CSCHAR *title, const CSCHAR *text, const CSCHAR *pszNotificationId)
    {
        Toast::SendNotification(title, text, pszNotificationId);
    }

    EXPORT void Toast_UnInitialize()
    {
        Toast::UnInitialize();
    }
}
