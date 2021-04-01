#ifndef __TRAY_MENU_H__
#define __TRAY_MENU_H__
#include "common.h"

namespace notification_tray_icon_private
{
    class CTrayMenu
    {
    public:
        CTrayMenu(const CSCHAR *pszText);

        ~CTrayMenu();

        bool SetDisabled(bool disabled);

        bool GetDisabled();

        void SetSelectedCallback(MenuItemSelectedEventCallback callback);

    private:
        MenuItemSelectedEventCallback mSelectedCallback;
        CSCHAR *mpszText;
        bool mbDisabled;
    };
}

#endif // __TRAY_MENU_H__
