#ifndef __CTray_MENU_ITEM_H__
#define __CTray_MENU_ITEM_H__
#include "../common.h"
#include "../ITrayMenuItem.h"
#include <windows.h>
#include <shellapi.h>

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon;

    class CTrayMenuItem : public ITrayMenuItem
    {
    public:
        CTrayMenuItem(CSCHAR *pszText);

        ~CTrayMenuItem();

        void SetText(CSCHAR *pszText);

        void SetDisabled(bool disabled);

        void SetChecked(bool checked);

        bool AddMenuItem(CTrayMenuItem *pTrayMenuItem);

        bool RemoveMenuItem(CTrayMenuItem *pTrayMenuItem, bool recurse = false);

        void RebuildMenu();

        void RebuildMenu(HMENU hMenu);

        void ConstructMenuItemInfo();

    private:
        MENUITEMINFOW mhMenuItem;
        HMENU mhMenu;
        CSCHAR *mpText;
    };
}

#endif // __CTray_MENU_ITEM_H__
