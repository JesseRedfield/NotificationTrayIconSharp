#include "TrayMenuItem.h"
#include "TrayIcon.h"

namespace notification_tray_icon_private
{
    CTrayMenuItem::CTrayMenuItem(const CSCHAR *pszText) : ITrayMenuItem(pszText)
    {
        _pTitle = NULL;
        _pButtonProxy = [[ButtonProxy alloc] initWithOwner: this];
        _pMenuItem = [NSMenuItem alloc];
        [_pMenuItem setEnabled: true];
        [_pMenuItem setTarget: _pButtonProxy];
        [_pMenuItem setAction: @selector(leftClick:)];

        SetText(pszText);
    }

    CTrayMenuItem::~CTrayMenuItem()
    {
        OBJC_SAFE_RELEASE(_pTitle)
        OBJC_SAFE_RELEASE(_pMenuItem)
        OBJC_SAFE_RELEASE(_pButtonProxy)
    }

    void CTrayMenuItem::SetText(const CSCHAR *pszText)
    {
        OBJC_SAFE_RELEASE(_pTitle)

        if(pszText == NULL)
            _pTitle = @"";
        else
            _pTitle = [[NSString alloc] initWithCString: pszText encoding: NSUTF8StringEncoding];
        
        [_pMenuItem setTitle: _pTitle];
    }

    void CTrayMenuItem::SetChecked(bool checked)
    {
        ITrayMenuItem::SetChecked(checked);
        [_pMenuItem setState: checked ? 1 : 0];
    }

    void CTrayMenuItem::SetDisabled(bool disabled)
    {
        ITrayMenuItem::SetDisabled(disabled);
        [_pMenuItem setEnabled: disabled ? false : true];
    }

    bool CTrayMenuItem::AddMenuItem(CTrayMenuItem *pTrayMenuItem)
    {
        if(ITrayMenuItem::AddMenuItem(pTrayMenuItem))
        {
            if(_pMenu == NULL)
                _pMenu = [NSMenu alloc]; 
            
            [_pMenuItem setSubmenu: _pMenu];
            [_pMenu addItem: ((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem()];

            return true;
        }
        
        return false;
    }

    bool CTrayMenuItem::RemoveMenuItem(CTrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if(ITrayMenuItem::RemoveMenuItem(pTrayMenuItem, recurse))
        {
            [_pMenu removeItem: ((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem()];

            if([_pMenu numberOfItems] == 0)
                [_pMenuItem setSubmenu: NULL];
            
            return true;
        }
        
        return false;
    }

    void CTrayMenuItem::OnSelected()
    {
        if(_bDisabled || !_MenuItems.empty()) return;
        
        if (_SelectedCallback != NULL)
            _SelectedCallback(this);
    }

    NSMenuItem *CTrayMenuItem::GetNSMenuItem()
    {
        return _pMenuItem;
    }
}
