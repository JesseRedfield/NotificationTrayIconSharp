#include "../TrayMenu.h"
#include "../common.h"
#include <cstdlib>
#include <string.h>

namespace notification_tray_icon_private
{

    CTrayMenu::CTrayMenu(const CSCHAR* pszText)
    {
        size_t size = CSTRLEN(pszText) + 1;
        mpszText = new CSCHAR[size];
        CSTRCPY(mpszText, pszText, size);
    }

    CTrayMenu::~CTrayMenu()
    {
        delete mpszText;
    }

    bool CTrayMenu::SetDisabled(bool disabled)
    {
        mbDisabled = disabled;
        return mbDisabled;
    }

    bool CTrayMenu::GetDisabled()
    {
        return mbDisabled;
    }

//    bool CTrayMenu::SetChecked(bool checked)
//    {
//        mbChecked = checked;
//        return mbChecked;
//    }
//
//    bool CTrayMenu::GetChecked()
//    {
//        return mbChecked;
//    }
    
    
    void CTrayMenu::SetSelectedCallback(MenuItemSelectedEventCallback callback)
    {
        mSelectedCallback = callback;
    }

}