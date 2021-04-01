#ifndef __TRAY_ICON_MAC_H__
#define __TRAY_ICON_MAC_H__
#include "../common.h"
#include "../TrayIcon.h"
#import <AppKit/NSMenu.h>
#import <AppKit/NSImage.h>
#import <AppKit/NSStatusBar.h>
#import <AppKit/NSStatusBarButton.h>
#import <AppKit/NSStatusItem.h>

namespace notification_tray_icon_private
{
    class CTrayIconMac : public CTrayIcon
    {
    public:
        void InitializeMenu();

    private:
        NSMenu *pMenu;
        NSStatusItem *pStatusItem;
    };
}

#endif // __TRAY_ICON_H__
