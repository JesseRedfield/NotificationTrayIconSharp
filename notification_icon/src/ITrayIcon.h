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

        void OnSelected();

        void SetSelectedCallback(MenuItemSelectedEventCallback callback);

        virtual void InitializeMenu(CSCHAR *pszIconPath) = 0;

        virtual void SetIcon(CSCHAR *pszIconPath) = 0;

        virtual bool AddMenuItem(ITrayMenuItem *pTrayMenu) = 0;

        virtual bool RemoveMenuItem(ITrayMenuItem *pTrayMenu) = 0;

        virtual int MessageLoop(bool blocking) = 0;

    private:
        MenuItemSelectedEventCallback _SelectedCallback;
    };
}

#endif // __ITRAY_ICON_H__
