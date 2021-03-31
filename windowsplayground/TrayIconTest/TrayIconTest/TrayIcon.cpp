#include "TrayIcon.h"
#include "TrayMenuItem.h"

CTrayIcon::CTrayIcon()
{
    mpWndClassEx = NULL;
    mpNotifyIconData = NULL;
    mhWnd = NULL;
    mhMenu = NULL;
    mpIconPathStr = NULL;
}
    
CTrayIcon::~CTrayIcon()
{
    if (mpWndClassEx != NULL)
        delete mpWndClassEx;
    mpWndClassEx = NULL;


    if (mpNotifyIconData != NULL) {
        Shell_NotifyIcon(NIM_DELETE, mpNotifyIconData);

        if (mpNotifyIconData->hIcon != NULL)
            DestroyIcon(mpNotifyIconData->hIcon);

        mpNotifyIconData = NULL;
    }

    if (mhMenu != NULL)
        DestroyMenu(mhMenu);
    mhMenu = NULL;

    UnregisterClass(WC_TRAY_CLASS_NAME, GetModuleHandle(NULL));

    if (mhWnd != NULL)
        DestroyWindow(mhWnd);
    mhWnd = NULL;

    if (mpIconPathStr != NULL)
        delete mpIconPathStr;
    mpIconPathStr = NULL;
}

void CTrayIcon::InitializeMenu(const char* pszIconPath)
{
    WNDCLASS wc;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    if (!GetClassInfo(hInstance, WC_TRAY_CLASS_NAME, &wc)) {
        mpWndClassEx = new WNDCLASSEX();
        memset(mpWndClassEx, 0, sizeof(WNDCLASSEX));
        mpWndClassEx->cbSize = sizeof(WNDCLASSEX);
        mpWndClassEx->lpfnWndProc = WndProc;
        mpWndClassEx->hInstance = GetModuleHandle(NULL);
        mpWndClassEx->lpszClassName = WC_TRAY_CLASS_NAME;

        if (!RegisterClassEx(mpWndClassEx)) {
            return;
        }
    }

    mhWnd = CreateWindowEx(0, WC_TRAY_CLASS_NAME, NULL, 0, 0, 0, 1, 1, NULL, NULL, hInstance, this);
    if (mhWnd == NULL) {
        return;
    }
    UpdateWindow(mhWnd);

    mpNotifyIconData = new NOTIFYICONDATA;
    mpNotifyIconData->cbSize = sizeof(NOTIFYICONDATA);
    mpNotifyIconData->hWnd = mhWnd;
    mpNotifyIconData->uID = 0;
    mpNotifyIconData->uFlags = NIF_ICON | NIF_MESSAGE;
    mpNotifyIconData->uCallbackMessage = WM_TRAY_CALLBACK_MESSAGE;
    Shell_NotifyIcon(NIM_ADD, mpNotifyIconData);

    SetIcon(pszIconPath);
}

void CTrayIcon::SetIcon(const char* pszIconPath)
{
    HICON icon;

    if (mpIconPathStr != NULL) {
        delete mpIconPathStr;
        mpIconPathStr = NULL;
    }

    mpIconPathStr = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, pszIconPath, -1, mpIconPathStr, 4096);
    ExtractIconEx(mpIconPathStr, 0, NULL, &icon, 1);

    if (mpNotifyIconData->hIcon) {
        DestroyIcon(mpNotifyIconData->hIcon);
    }

    mpNotifyIconData->hIcon = icon;
    Shell_NotifyIcon(NIM_MODIFY, mpNotifyIconData);
}

void CTrayIcon::OnSelected()
{
    CTrayMenuItem* pNewItem = new CTrayMenuItem(1000, "HELLO WORLD");
    AddMenuItem(pNewItem);
}

bool CTrayIcon::AddMenuItem(CTrayMenuItem *pTrayMenu)
{
    if(IMenuContainer::AddMenuItem(pTrayMenu)) {
        pTrayMenu->SetOwner(this);
        RebuildMenu();
        return true;
    }

    return false;
}

void CTrayIcon::RebuildMenu()
{
    if (mhMenu != NULL) {
        DestroyMenu(mhMenu);
        mhMenu = NULL;
    }

    if (mMenuItems.empty()) return;

    mhMenu = CreatePopupMenu();
    for (std::map<uint64_t, CTrayMenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); it++) {
        it->second->RebuildMenu(mhMenu);
    }
}

int CTrayIcon::MessageLoop(bool blocking)
{
    MSG msg;
    if (blocking) {
        GetMessage(&msg, NULL, 0, 0);
    }
    else {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    }
    if (msg.message == WM_QUIT) {
        return -1;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return 0;
}

// ####### WINDOWS CODE #########

LRESULT CALLBACK CTrayIcon::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    CTrayIcon* pThis = NULL;

    //Window create message handling.
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
        
        if (!pcs) return 0;

        pThis = (CTrayIcon*)pcs->lpCreateParams;
        
        if (!pThis) return 0;

        pThis->mhWnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        pThis = (CTrayIcon*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    }

    if (pThis != NULL) {
        if (uMsg == WM_NCDESTROY) {
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
        }
        else if (uMsg == WM_TRAY_CALLBACK_MESSAGE) {
            if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP) {
                if (pThis->mhMenu == NULL) {
                    pThis->OnSelected();
                    return 0;
                }
                else {
                    POINT p;
                    GetCursorPos(&p);
                    SetForegroundWindow(hWnd);
                    WORD cmd = TrackPopupMenu(pThis->mhMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON |
                        TPM_RETURNCMD | TPM_NONOTIFY,
                        p.x, p.y, 0, hWnd, NULL);
                    SendMessage(hWnd, WM_COMMAND, cmd, 0);
                    return 0;
                }
            }
        }
        else if (uMsg == WM_COMMAND) {
            MENUITEMINFO item;
            memset(&item, 0, sizeof(MENUITEMINFO));
            item.cbSize = sizeof(MENUITEMINFO);
            item.fMask = MIIM_ID | MIIM_DATA;

            if (GetMenuItemInfo(pThis->mhMenu, wParam, FALSE, &item)) {
                CTrayMenuItem* pMenuItem = (CTrayMenuItem*)item.dwItemData;
                if (pMenuItem != NULL) {
                    pMenuItem->OnSelected();
                }
            }
            return 0;
        }
        
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}