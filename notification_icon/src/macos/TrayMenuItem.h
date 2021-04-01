#ifndef __CTray_MENU_ITEM_H__
#define __CTray_MENU_ITEM_H__
#include "../common.h"
#include "../ITrayMenuItem.h"
#import "ButonProxyTarget.hpp"
#import <AppKit/NSMenuItem.h>

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon;

    class CTrayMenuItem : public ITrayMenuItem, IButtonProxyTarget
    {
    public:
        CTrayMenuItem(CSCHAR *pszText);

        ~CTrayMenuItem();

        void SetText(CSCHAR *pszText);

        void SetDisabled(bool disabled);

        void SetChecked(bool checked);

        bool AddMenuItem(CTrayMenuItem *pTrayMenuItem);

        bool RemoveMenuItem(CTrayMenuItem *pTrayMenuItem, bool recurse = false);

    private:
        NSString *_pTitle;
        NSMenuItem *_pMenuItem;
        ButtonProxy *_pButtonProxy;
    };
}

#endif // __CTray_MENU_ITEM_H__
