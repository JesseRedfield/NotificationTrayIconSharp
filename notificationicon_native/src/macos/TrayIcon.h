#ifndef __CTRAY_ICON_H__
#define __CTRAY_ICON_H__
#include "../common.h"
#include "../ITrayIcon.h"
#include "TrayMenuItem.h"
#include "ButtonProxyTarget.h"

typedef void C_NSStatusItem;
typedef void C_NSImage;
typedef void C_ButtonProxy;

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon : public ITrayIcon, IButtonProxyTarget
    {
    public:
        CTrayIcon();

        ~CTrayIcon();

        void InitializeMenu(const CSCHAR *pszIconPath);

        void SetIcon(const CSCHAR *pszIconPath);

        bool AddMenuItem(ITrayMenuItem *pTrayMenu);

        bool RemoveMenuItem(ITrayMenuItem *pTrayMenu, bool recurse = false);

        void OnSelected();

        int MessageLoop(bool blocking);

    private:
        C_NSStatusItem *_pStatusItem;
        C_NSImage *_pImage;
        C_NSString *_pIconPath;
        C_NSMenu *_pMenu;
        C_ButtonProxy *_pButtonProxy;
    };
}

#endif // __ITRAY_ICON_H__
