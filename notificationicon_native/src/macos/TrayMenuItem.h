#ifndef __CTray_MENU_ITEM_H__
#define __CTray_MENU_ITEM_H__
#include "../common.h"
#include "../ITrayMenuItem.h"
#include "ButtonProxyTarget.h"

typedef void C_NSString;
typedef void C_NSMenu;
typedef void C_NSMenuItem;
typedef void C_ButtonProxy;

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

        bool AddMenuItem(ITrayMenuItem *pTrayMenuItem);

        bool RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse = false);

        void OnSelected();

        C_NSMenuItem *GetNSMenuItem();

    private:
        C_NSString *_pTitle;
        C_NSMenu *_pMenu;
        C_NSMenuItem *_pMenuItem;
        C_ButtonProxy *_pButtonProxy;
    };
}

#endif // __CTray_MENU_ITEM_H__
