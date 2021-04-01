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

    void CTrayIcon::InitializeMenu(const CSCHAR *pszIconPath)
    {
    _pStatusItem = [[NSStatusBar systemStatusBar] statusItemWithLength: NSSquareStatusItemLength];
    
    [[_pStatusItem button] setImage: _pImage]; 
    [[_pStatusItem button] setEnabled: true];
    [[_pStatusItem button] setTarget: _pButtonProxy];
    [[_pStatusItem button] setAction: @selector(leftClick:)];
    //[_pStatusItem retain];
    SetIcon(pszIconPath);
    }

    void CTrayIcon::SetIcon(const CSCHAR *pszIconPath)
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
        if(ITrayIcon::AddMenuItem(pTrayMenuItem))
        {
            if(_pMenu == NULL)
                _pMenu = [NSMenu alloc];
            
            [_pStatusItem setMenu: _pMenu];
            [_pMenu addItem: ((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem()];

            return true;
        }
        
        return false;
    }

    bool CTrayIcon::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if(ITrayIcon::RemoveMenuItem(pTrayMenuItem, recurse))
        {
            [_pMenu removeItem: ((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem()];

            if([_pMenu numberOfItems] == 0)
                [_pStatusItem setMenu: NULL];
            
            return true;
        }
        
        return false;
    }

    void CTrayIcon::OnSelected()
    {
        if (_SelectedCallback != NULL)
            _SelectedCallback(this);
    }

    int CTrayIcon::MessageLoop(bool blocking)
    {
        return 0;
    }
}
