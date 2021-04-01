#ifndef __CTRAY_ICON_H__
#define __CTRAY_ICON_H__
#include "../common.h"
#include "../ITrayIcon.h"
#include "TrayMenuItem.h"
#include <windows.h>
#include <shellapi.h>
#include <tchar.h>

#define WM_TRAY_CALLBACK_MESSAGE (WM_USER + 1)
#define WC_TRAY_CLASS_NAME _T("tray_icon_class")

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    class CTrayIcon : public ITrayIcon
    {
    public:
        CTrayIcon();

        ~CTrayIcon();

        void InitializeMenu(CSCHAR *pszIconPath);

        void SetIcon(CSCHAR *pszIconPath);

        bool AddMenuItem(ITrayMenuItem *pTrayMenu);

        bool RemoveMenuItem(ITrayMenuItem* pTrayMenu);

        void RebuildMenu();

        int MessageLoop(bool blocking);

    private:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WndProc_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WndProc_TrayCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, CTrayIcon *pThis);
        static LRESULT CALLBACK WndProc_Command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, CTrayIcon *pThis);

    private:
        bool _bInitialized;
        CSCHAR *_pIconPathStr;
        WNDCLASSEX _WndClassEx;
        NOTIFYICONDATA _NotifyIconData;
        HWND mhWnd;
        HMENU mhMenu;
    };
}

#endif // __ITRAY_ICON_H__
