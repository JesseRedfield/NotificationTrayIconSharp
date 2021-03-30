#ifndef __TRAY_ICON_H__
#define __TRAY_ICON_H__
#include "NativeTypes.h"
#include "TrayMenu.h"
#include <vector>

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon
    {
    public:
        CTrayIcon(const CSCHAR *pszIconPath);

        ~CTrayIcon();

        void SetSelectedCallback(MenuItemSelectedEventCallback callback);

    protected:
        CSCHAR *mpszIconPath;
        std::vector<CTrayMenu *> mMenuItems;
        MenuItemSelectedEventCallback mSelectedCallback;
    };
}

#endif // __TRAY_ICON_H__
