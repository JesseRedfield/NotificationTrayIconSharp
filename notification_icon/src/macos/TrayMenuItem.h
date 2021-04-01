#ifndef __CTray_MENU_ITEM_H__
#define __CTray_MENU_ITEM_H__
#include "../common.h"
#include "../ITrayMenuItem.h"
#import "ButtonProxy.hpp"
#import "ButtonProxyTarget.hpp"
#import <Cocoa/Cocoa.h>
#import <AppKit/NSMenu.h>
#import <AppKit/NSMenuItem.h>

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon;

    class CTrayMenuItem : public ITrayMenuItem, IButtonProxyTarget
    {
    public:
        CTrayMenuItem(const CSCHAR *pszText);

        ~CTrayMenuItem();

        void SetText(const CSCHAR *pszText);

        void SetDisabled(bool disabled);

        void SetChecked(bool checked);

        bool AddMenuItem(CTrayMenuItem *pTrayMenuItem);

        bool RemoveMenuItem(CTrayMenuItem *pTrayMenuItem, bool recurse = false);

        void OnSelected();

        NSMenuItem *GetNSMenuItem();

    private:
        NSString *_pTitle;
        NSMenu *_pMenu;
        NSMenuItem *_pMenuItem;
        ButtonProxy *_pButtonProxy;
    };
}

#endif // __CTray_MENU_ITEM_H__
