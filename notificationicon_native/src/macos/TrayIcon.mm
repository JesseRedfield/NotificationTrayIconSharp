#include "TrayIcon.h"
#import "ButtonProxy.h"
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSImage.h>
#import <AppKit/NSStatusBar.h>
#import <AppKit/NSStatusBarButton.h>
#import <AppKit/NSStatusItem.h>

OBJC_INCLUDE_BUTTONPROXY(CTrayIconProxy)

namespace notification_tray_icon_private
{
    NSApplication *pApp;

    CTrayIcon::CTrayIcon() : ITrayIcon()
    {
        pApp = [NSApplication sharedApplication];
        _pStatusItem = NULL;
        _pImage = NULL;
        _pIconPath = NULL;
        CTrayIconProxy *pButtonProxy = [[CTrayIconProxy alloc] initWithOwner: this];
        _pButtonProxy = (void*)CFBridgingRetain(pButtonProxy);
    }

    CTrayIcon::~CTrayIcon()
    {
        if(_pStatusItem != NULL)
        {
            NSStatusItem *pStatusItem = (__bridge NSStatusItem*)_pStatusItem;
            [[NSStatusBar systemStatusBar] removeStatusItem: pStatusItem];
        }
        OBJC_SAFE_RELEASE(_pStatusItem)
        OBJC_SAFE_RELEASE(_pImage)
        OBJC_SAFE_RELEASE(_pIconPath)
    }

    void CTrayIcon::InitializeMenu(const CSCHAR *pszIconPath)
    {
        NSStatusItem *pStatusItem = [[NSStatusBar systemStatusBar] statusItemWithLength: NSSquareStatusItemLength];
        CTrayIconProxy *pButtonProxy = (__bridge CTrayIconProxy*)_pButtonProxy;
            
        [[pStatusItem button] setEnabled: true];
        [[pStatusItem button] setTarget: pButtonProxy];
        [[pStatusItem button] setAction: @selector(leftClick:)];
        _pStatusItem = (void*)CFBridgingRetain(pStatusItem);
        
        SetIcon(pszIconPath);
    }

    void CTrayIcon::SetIcon(const CSCHAR *pszIconPath)
    {
        OBJC_SAFE_RELEASE(_pImage)
        OBJC_SAFE_RELEASE(_pIconPath)

        if(pszIconPath == NULL) return;
    
        NSStatusItem *pStatusItem = (__bridge NSStatusItem*)_pStatusItem;
        
        NSString *pIconPath = [[NSString alloc] initWithCString: pszIconPath encoding: NSUTF8StringEncoding];
        _pIconPath = (void*)CFBridgingRetain(pIconPath);
        
        NSImage *pImage = [[NSImage alloc] initWithContentsOfFile: pIconPath];
        _pImage = (void*)CFBridgingRetain(pImage);
        
        [[pStatusItem button] setImage: pImage];
    }

    bool CTrayIcon::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if(ITrayIcon::AddMenuItem(pTrayMenuItem))
        {
            NSMenu *pMenu;
            NSStatusItem* pStatusItem = (__bridge NSStatusItem*)_pStatusItem;
            
            if(_pMenu == NULL)
                pMenu = [NSMenu alloc];
            else
                pMenu = (__bridge NSMenu*)_pMenu;
            
            NSMenuItem* pItem = (__bridge NSMenuItem*)((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem();
                        
            [pStatusItem setMenu: pMenu];
            [pMenu addItem: pItem];

            return true;
        }
        
        return false;
    }

    bool CTrayIcon::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if(ITrayIcon::RemoveMenuItem(pTrayMenuItem, recurse))
        {
            NSMenu* pMenu = (__bridge NSMenu*)_pMenu;
            NSMenuItem* pItem = (__bridge NSMenuItem*)((CTrayMenuItem*)pTrayMenuItem)->GetNSMenuItem();
            NSStatusItem* pStatusItem = (__bridge NSStatusItem*)_pStatusItem;
            
            [pMenu removeItem: pItem];

            if([pMenu numberOfItems] == 0)
                [pStatusItem setMenu: NULL];
            
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
    {}
        NSDate *pUntil = blocking ? [NSDate distantFuture] : [NSDate distantPast];
        NSString *pRunLoopMode = @"kCFRunLoopDefaultMode";
        NSEvent *pEvent = [pApp nextEventMatchingMask:ULONG_MAX untilDate:pUntil inMode:pRunLoopMode dequeue:true];
           
        if (pEvent) {
            [pApp sendEvent: pEvent];
        }
            return 0;
    }
}
