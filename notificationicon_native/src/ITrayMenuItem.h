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
        ITrayMenuItem(const CSCHAR *pszText);

        ~ITrayMenuItem();

        uint32_t GetUUID();

        void SetOwner(ITrayIcon *pOwner);

        void SetSelectedCallback(MenuItemSelectedEventCallback callback);
        
        virtual void SetText(const CSCHAR *pszText) = 0;

        virtual bool GetDisabled();

        virtual void SetDisabled(bool disabled);

        virtual bool GetChecked();

        virtual void SetChecked(bool checked);

        virtual bool AddMenuItem(ITrayMenuItem *pTrayMenuItem);

        virtual bool RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse = false);

        virtual void OnSelected() = 0;

    protected:
        bool _bDisabled;
        bool _bChecked;
        uint32_t _UUID;
        ITrayIcon *_pOwner;
        MenuItemSelectedEventCallback _SelectedCallback;
    };
}

#endif // __ITRAY_MENU_ITEM_H__
