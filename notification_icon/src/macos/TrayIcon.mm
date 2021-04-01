#include "TrayIcon.h"

namespace notification_tray_icon_private
{
    CTrayIcon::CTrayIcon() : ITrayIcon()
    {
        _pStatusItem = NULL;
        _pImage = NULL;
        _pIconPath = NULL;
        _pButtonProxy = [[ButtonProxy alloc] initWithOwner: this];
    }

    CTrayIcon::~CTrayIcon()
    {
         if(_pStatusItem != NULL)
        {
            [[NSStatusBar systemStatusBar] removeStatusItem: _pStatusItem];
        }
        OBJC_SAFE_RELEASE(_pStatusItem)
        OBJC_SAFE_RELEASE(_pImage)
        OBJC_SAFE_RELEASE(_pIconPath)
    }

    void CTrayIcon::InitializeMenu(CSCHAR *pszIconPath)
    {
    _pStatusItem = [[NSStatusBar systemStatusBar] statusItemWithLength: NSSquareStatusItemLength];
    
    [[_pStatusItem button] setImage: _pImage];
    [[_pStatusItem button] setEnabled: true];
    [[_pStatusItem button] setTarget: _pButtonProxy];
    [[_pStatusItem button] setAction: @selector(leftClick:)];
    [_pStatusItem retain];
    SetIcon(pszIconPath);
    }

    void CTrayIcon::SetIcon(CSCHAR *pszIconPath)
    {
        OBJC_SAFE_RELEASE(_pImage)
        OBJC_SAFE_RELEASE(_pIconPath)

        if(pszIconPath == NULL) return;
    
        _pIconPath = [[NSString alloc] initWithCString: pszIconPath encoding: NSUTF8StringEncoding];
        _pImage = [[NSImage alloc] initWithContentsOfFile: _pIconPath];
        
        [[_pStatusItem button] setImage: _pImage];
    }

    bool CTrayIcon::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if(ITrayIcon::AddMenuItem(pTrayMenu))
        {
            [_pStatusItem setMenu: _pMenu];
            [_pMenu addItem: pTrayMenu->GetNSMenuItem()];

            return true;
        }
        
        return false;
    }

    bool CTrayIcon::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if(IMenuContainer::AddMenuItem(pTrayMenu))
        {
            [mpStatusItem setMenu: mpMenu];
            [mpMenu addItem: pTrayMenu->GetNSMenuItem()];

            return true;
        }
        
        return false;
    }

    int CTrayIcon::MessageLoop(bool blocking)
    {

    }
}
