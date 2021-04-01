#include "TrayIcon.h"

namespace notification_tray_icon_private
{
    CTrayIcon::CTrayIcon() : ITrayIcon()
    {
        mhWnd = NULL;
        mhMenu = NULL;
        _bInitialized = false;
        _pIconPathStr = NULL;
        memset(&_WndClassEx, 0, sizeof(WNDCLASSEX));
        memset(&_NotifyIconData, 0, sizeof(NOTIFYICONDATA));
    }

    CTrayIcon::~CTrayIcon()
    {
        if (mhMenu != NULL)
            DestroyMenu(mhMenu);
        mhMenu = NULL;

        if (mhWnd != NULL)
            DestroyWindow(mhWnd);
        mhWnd = NULL;

        if (_bInitialized)
        {
            Shell_NotifyIcon(NIM_DELETE, &_NotifyIconData);

            if (_NotifyIconData.hIcon != NULL)
                DestroyIcon(_NotifyIconData.hIcon);

            UnregisterClass(WC_TRAY_CLASS_NAME, GetModuleHandle(NULL));
        }

        SAFE_DELETE(_pIconPathStr);
    }

    void CTrayIcon::InitializeMenu(const CSCHAR *pszIconPath)
    {
        WNDCLASS wc;
        HINSTANCE hInstance = GetModuleHandle(NULL);

        if (!GetClassInfo(hInstance, WC_TRAY_CLASS_NAME, &wc))
        {
            memset(&_WndClassEx, 0, sizeof(WNDCLASSEX));
            _WndClassEx.cbSize = sizeof(WNDCLASSEX);
            _WndClassEx.lpfnWndProc = WndProc;
            _WndClassEx.hInstance = GetModuleHandle(NULL);
            _WndClassEx.lpszClassName = WC_TRAY_CLASS_NAME;

            if (!RegisterClassEx(&_WndClassEx))
                return;
        }

        mhWnd = CreateWindowEx(0, WC_TRAY_CLASS_NAME, NULL, 0, 0, 0, 1, 1, NULL, NULL, hInstance, this);
        if (mhWnd == NULL)
            return;

        UpdateWindow(mhWnd);

        _NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
        _NotifyIconData.hWnd = mhWnd;
        _NotifyIconData.uID = 0;
        _NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE;
        _NotifyIconData.uCallbackMessage = WM_TRAY_CALLBACK_MESSAGE;
        Shell_NotifyIcon(NIM_ADD, &_NotifyIconData);

        _bInitialized = true;
        SetIcon(pszIconPath);
    }

    void CTrayIcon::SetIcon(const CSCHAR *pszIconPath)
    {
        HICON icon;

        if (_pIconPathStr != NULL)
        {
            delete _pIconPathStr;
            _pIconPathStr = NULL;
        }

        size_t size = CSSTRLEN(pszIconPath) + 1;
        _pIconPathStr = new CSCHAR[size];
        CSSTRCPY(_pIconPathStr, pszIconPath, size);
        ExtractIconExW(_pIconPathStr, 0, NULL, &icon, 1);

        if (_NotifyIconData.hIcon)
        {
            DestroyIcon(_NotifyIconData.hIcon);
        }

        _NotifyIconData.hIcon = icon;
        Shell_NotifyIcon(NIM_MODIFY, &_NotifyIconData);
    }

    bool CTrayIcon::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if (ITrayIcon::AddMenuItem(pTrayMenuItem))
        {
            pTrayMenuItem->SetOwner(this);
            RebuildMenu();
            return true;
        }

        return false;
    }

    bool CTrayIcon::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if (ITrayIcon::RemoveMenuItem(pTrayMenuItem))
        {
            pTrayMenuItem->SetOwner(NULL);
            RebuildMenu();
            return true;
        }

        return false;
    }

    void CTrayIcon::RebuildMenu()
    {
        if (mhMenu != NULL)
        {
            DestroyMenu(mhMenu);
            mhMenu = NULL;
        }

        if (_MenuItems.empty())
            return;

        mhMenu = CreatePopupMenu();
        for (std::map<uint32_t, ITrayMenuItem *>::iterator it = _MenuItems.begin(); it != _MenuItems.end(); it++)
        {
            CTrayMenuItem *pItem = (CTrayMenuItem *)it->second;
            pItem->RebuildMenu(mhMenu);
        }
    }

    int CTrayIcon::MessageLoop(bool blocking)
    {
        MSG msg;
        if (blocking)
        {
            GetMessage(&msg, NULL, 0, 0);
        }
        else
        {
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        }
        if (msg.message == WM_QUIT)
        {
            return -1;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return 0;
    }

    // ####### WINDOWS PROC CODE #########

    LRESULT CALLBACK CTrayIcon::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {

        if (uMsg == WM_NCCREATE)
        {
            return WndProc_Create(hWnd, uMsg, wParam, lParam);
        }

        CTrayIcon *pThis = (CTrayIcon *)GetWindowLongPtrW(hWnd, GWLP_USERDATA);

        if (pThis == NULL)
            return 0;

        if (uMsg == WM_NCDESTROY)
        {
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        if (uMsg == WM_TRAY_CALLBACK_MESSAGE)
        {
            return WndProc_TrayCallback(hWnd, uMsg, wParam, lParam, pThis);
        }
        if (uMsg == WM_COMMAND)
        {
            return WndProc_Command(hWnd, uMsg, wParam, lParam, pThis);
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    LRESULT CALLBACK CTrayIcon::WndProc_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        CREATESTRUCT *pcs = (CREATESTRUCT *)lParam;
        if (!pcs)
            return 0;

        CTrayIcon *pThis = (CTrayIcon *)pcs->lpCreateParams;
        if (!pThis)
            return 0;

        pThis->mhWnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    LRESULT CALLBACK CTrayIcon::WndProc_TrayCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, CTrayIcon *pThis)
    {
        if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP)
        {
            if (pThis->mhMenu == NULL)
            {
                pThis->OnSelected();
                return 0;
            }
            else
            {
                POINT p;
                GetCursorPos(&p);
                SetForegroundWindow(hWnd);
                WORD cmd = TrackPopupMenu(pThis->mhMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY,
                                          p.x, p.y, 0, hWnd, NULL);
                SendMessage(hWnd, WM_COMMAND, cmd, 0);
                return 0;
            }
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    LRESULT CALLBACK CTrayIcon::WndProc_Command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, CTrayIcon *pThis)
    {
        MENUITEMINFOW item;
        memset(&item, 0, sizeof(MENUITEMINFOW));
        item.cbSize = sizeof(MENUITEMINFOW);
        item.fMask = MIIM_ID | MIIM_DATA;

        if (GetMenuItemInfoW(pThis->mhMenu, (UINT)wParam, FALSE, &item))
        {
            ITrayMenuItem *pMenuItem = (ITrayMenuItem *)item.dwItemData;
            if (pMenuItem != NULL)
            {
                pMenuItem->OnSelected();
            }
        }

        return 0;
    }
}
