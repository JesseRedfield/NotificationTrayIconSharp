#include "TrayIcon.h"
#include "NativeTypes.h"

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    CTrayIcon::CTrayIcon(const CSCHAR *pszText)
    {
        size_t size = CSTRLEN(pszText) + 1;
        mpszIconPath = new CSCHAR[size];
        CSTRCPY(mpszIconPath, pszText, size);
    }

    CTrayIcon::~CTrayIcon()
    {
        delete mpszIconPath;
    }

    void CTrayIcon::SetSelectedCallback(MenuItemSelectedEventCallback callback)
    {
        mSelectedCallback = callback;
    }
}
