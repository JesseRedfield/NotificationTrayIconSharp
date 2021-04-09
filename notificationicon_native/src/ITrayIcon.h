#ifndef __ITRAY_ICON_H__
#define __ITRAY_ICON_H__
#include "common.h"
#include "IMenuContainer.h"
#include "ITrayMenuItem.h"

#define WM_TRAY_CALLBACK_MESSAGE (WM_USER + 1)
#define WC_TRAY_CLASS_NAME _T("tray_icon_class")

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class ITrayIcon : public IMenuContainer
    {
    public:
        ITrayIcon();

        ~ITrayIcon();

        void SetSelectedCallback(MenuItemSelectedEventCallback callback);
        
        virtual bool AddMenuItem(ITrayMenuItem *pTrayMenu);

        virtual bool RemoveMenuItem(ITrayMenuItem *pTrayMenu, bool recurse = false);

        virtual void InitializeMenu(const CSCHAR *pszIconPath) = 0;

        virtual void SetIcon(const CSCHAR *pszIconPath) = 0;

        virtual int MessageLoop(bool blocking) = 0;

        virtual void OnSelected() = 0;

    protected:
        MenuItemSelectedEventCallback _SelectedCallback;
    };
}

#endif // __ITRAY_ICON_H__
