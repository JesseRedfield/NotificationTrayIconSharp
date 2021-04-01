#include "TrayMenuItem.h"
#include "TrayIcon.h"

namespace notification_tray_icon_private
{
    CTrayMenuItem::CTrayMenuItem(CSCHAR *pszText) : ITrayMenuItem(pszText)
    {
        mpTitle = NULL;
        mpButtonProxy = [[ButtonProxy alloc] initWithOwner: this];
        mpMenuItem = [NSMenuItem alloc];
        [mpMenuItem setEnabled: true];
        [mpMenuItem setTarget: mpButtonProxy];
        [mpMenuItem setAction: @selector(leftClick:)];

        SetText(pszText);
    }

    CTrayMenuItem::~CTrayMenuItem()
    {
        OBJC_SAFE_RELEASE(mpTitle)
        OBJC_SAFE_RELEASE(mpMenuItem)
        OBJC_SAFE_RELEASE(mpButtonProxy)
    }

    void CTrayMenuItem::SetText(CSCHAR *pszText)
    {
        OBJC_SAFE_RELEASE(mpTitle)

        if(pszText == NULL)
            mpTitle = [[NSString alloc] initWithString: @""];
        else
            mpTitle = [[NSString alloc] initWithCString: pszText encoding: NSUTF8StringEncoding];
        
        [mpMenuItem setTitle: mpTitle];
    }

    void CTrayMenuItem::SetChecked(bool checked)
    {
        ITrayMenuItem::SetChecked(checked);
        [mpMenuItem setState: mbChecked ? 1 : 0];
    }

    void CTrayMenuItem::SetDisabled(bool disabled)
    {
        ITrayMenuItem::SetDisabled(disabled);
        [mpMenuItem setEnabled: !disabled];
    }

    bool CTrayMenuItem::AddMenuItem(CTrayMenuItem *pTrayMenuItem)
    {
        if(ITrayMenuItem::AddMenuItem(pTrayMenu))
        {
            [mpMenuItem setSubmenu: mpMenu];
            [mpMenu addItem: pTrayMenu->GetNSMenuItem()];

            return true;
        }
        
        return false;
    }

    bool CTrayMenuItem::RemoveMenuItem(CTrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if(ITrayMenuItem::RemoveMenuItem(pTrayMenu))
        {
            [mpMenuItem setSubmenu: mpMenu];

            return true;
        }
        
        return false;
    }
}