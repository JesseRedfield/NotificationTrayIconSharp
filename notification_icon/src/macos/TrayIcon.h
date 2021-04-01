#ifndef __CTRAY_ICON_H__
#define __CTRAY_ICON_H__
#include "../common.h"
#include "../ITrayIcon.h"
#include "TrayMenuItem.h"
#import "ButonProxyTarget.hpp"
#import <Cocoa/Cocoa.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSImage.h>
#import <AppKit/NSStatusBar.h>
#import <AppKit/NSStatusBarButton.h>
#import <AppKit/NSStatusItem.h>

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon : public ITrayIcon, IButtonProxyTarget
    {
    public:
        CTrayIcon();

        ~CTrayIcon();

        void InitializeMenu(CSCHAR *pszIconPath);

        void SetIcon(CSCHAR *pszIconPath);

        bool AddMenuItem(ITrayMenuItem *pTrayMenu);

        bool RemoveMenuItem(ITrayMenuItem *pTrayMenu);

        void RebuildMenu();

        int MessageLoop(bool blocking);

    private:
        NSStatusItem *_pStatusItem;
        NSImage *_pImage;
        NSString *_pIconPath;
        ButtonProxy *_pButtonProxy;
    };
}

#endif // __ITRAY_ICON_H__
