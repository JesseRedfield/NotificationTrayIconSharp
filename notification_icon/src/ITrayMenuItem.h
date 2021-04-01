#ifndef __ITRAY_MENU_ITEM_H__
#define __ITRAY_MENU_ITEM_H__
#include "common.h"
#include "IMenuContainer.h"

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class ITrayIcon;

    class ITrayMenuItem : public IMenuContainer
    {
    public:
        ITrayMenuItem(CSCHAR *pszText);

        ~ITrayMenuItem();

        uint32_t GetUUID();

        void SetOwner(ITrayIcon *pOwner);

        void SetText(CSCHAR *pszText);

        bool GetDisabled();

        void SetDisabled(bool disabled);

        bool GetChecked();

        void SetChecked(bool checked);

        bool AddMenuItem(ITrayMenuItem *pTrayMenuItem);

        bool RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse = false);

        void OnSelected();

        void SetSelectedCallback(MenuItemSelectedEventCallback callback);

    protected:
        bool _bDisabled;
        bool _bChecked;
        uint32_t _UUID;
        ITrayIcon *_pOwner;
        MenuItemSelectedEventCallback _SelectedCallback;

    };
}

#endif // __ITRAY_MENU_ITEM_H__
