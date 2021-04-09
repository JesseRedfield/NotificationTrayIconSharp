#include "TrayMenuItem.h"
#include "TrayIcon.h"
#import  "ButtonProxy.h"
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSMenuItem.h>

OBJC_INCLUDE_BUTTONPROXY(CTrayMenuItemProxy)

namespace notification_tray_icon_private
{
     CTrayMenuItem::CTrayMenuItem(const CSCHAR *pszText) : ITrayMenuItem(pszText)
    {
        _pTitle = NULL;
        _pMenu = NULL;
        
        CTrayMenuItemProxy *pButtonProxy = [[CTrayMenuItemProxy alloc] initWithOwner: this];
        _pButtonProxy = (void*)CFBridgingRetain(pButtonProxy);

        NSMenuItem* pMenuItem = [NSMenuItem alloc];
        [pMenuItem setEnabled: true];
        [pMenuItem setTarget: pButtonProxy];
        [pMenuItem setAction: @selector(leftClick:)];
        _pMenuItem = (void*)CFBridgingRetain(pMenuItem);

        SetText(pszText);
    }

    CTrayMenuItem::~CTrayMenuItem()
    {
        OBJC_SAFE_RELEASE(_pTitle)
        OBJC_SAFE_RELEASE(_pMenu)
        OBJC_SAFE_RELEASE(_pMenuItem)
        OBJC_SAFE_RELEASE(_pButtonProxy)
    }

    void CTrayMenuItem::SetText(const CSCHAR *pszText)
    {
        OBJC_SAFE_RELEASE(_pTitle)

        NSMenuItem* pMenuItem = (__bridge NSMenuItem*)_pMenuItem;
        
        NSString* pTitle;
        if(pszText == NULL)
            pTitle = @"";
        else
            pTitle = [[NSString alloc] initWithCString: pszText encoding: NSUTF8StringEncoding];
        
        _pTitle = (void*)CFBridgingRetain(pTitle);
        [pMenuItem setTitle: pTitle];
    }

    void CTrayMenuItem::SetChecked(bool checked)
    {
        NSMenuItem* pMenuItem = (__bridge NSMenuItem*)_pMenuItem;
        
        ITrayMenuItem::SetChecked(checked);
        [pMenuItem setState: checked ? 1 : 0];
    }

    void CTrayMenuItem::SetDisabled(bool disabled)
    {
        NSMenuItem* pMenuItem = (__bridge NSMenuItem*)_pMenuItem;
        
        ITrayMenuItem::SetDisabled(disabled);
        [pMenuItem setEnabled: disabled ? false : true];
    }

    bool CTrayMenuItem::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if(ITrayMenuItem::AddMenuItem(pTrayMenuItem))
        {
            NSMenu *pMenu;
            NSMenuItem* pMenuItem = (__bridge NSMenuItem*)_pMenuItem;
            
            if(_pMenu == NULL)
            {
                pMenu = [NSMenu alloc];
                _pMenu = (void*)CFBridgingRetain(pMenu);
            }
            else
            {
                pMenu = (__bridge NSMenu*)_pMenu;
            }

            NSMenuItem* pItem = (__bridge NSMenuItem*)((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem();
            
            [pMenuItem setSubmenu: pMenu];
            [pMenu addItem: pItem];

            return true;
        }
        
        return false;
    }

    bool CTrayMenuItem::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if(ITrayMenuItem::RemoveMenuItem(pTrayMenuItem, false))
        {
            NSMenuItem* pItem = (__bridge NSMenuItem*)((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem();
            NSMenuItem* pMenuItem = (__bridge NSMenuItem*)_pMenuItem;
            
            if(_pMenu != NULL) 
            {
                NSMenu* pMenu = (__bridge NSMenu*)_pMenu;
                [pMenu removeItem: pItem];

                if([pMenu numberOfItems] == 0)
                    [pMenuItem setSubmenu: NULL];
            }

            return true;
        }
        
        if(recurse)
        {
            return ITrayMenuItem::RemoveMenuItem(pTrayMenuItem, true);
        }
        
        return false;
    }

    void CTrayMenuItem::OnSelected()
    {
        if(_bDisabled || !_MenuItems.empty()) return;
        
        if (_SelectedCallback != NULL)
            _SelectedCallback(this);
    }

    void *CTrayMenuItem::GetNSMenuItem()
    {
        return _pMenuItem;
    }
}
