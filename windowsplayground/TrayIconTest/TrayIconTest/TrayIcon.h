#include "TrayMenuItem.h"
#include "IMenuContainer.h"
#include <windows.h>
#include <shellapi.h>
#include <tchar.h>

#ifndef TrayIcon_h
#define TrayIcon_h

#define WM_TRAY_CALLBACK_MESSAGE (WM_USER + 1)
#define WC_TRAY_CLASS_NAME _T("tray_icon_class")

class CTrayIcon : IMenuContainer
{
public:
    CTrayIcon();
    
    ~CTrayIcon();

    void InitializeMenu(const char* pszIconPath);

    void SetIcon(const char* pszIconPath);
    
    void OnSelected();
    
    bool AddMenuItem(CTrayMenuItem* pTrayMenu);
    
    void RebuildMenu();

    //void SetSelectedCallback(MenuItemSelectedEventCallback callback);
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    int MessageLoop(bool blocking);

private:
    //MenuItemSelectedEventCallback mSelectedCallback;
    wchar_t* mpIconPathStr;
    WNDCLASSEX* mpWndClassEx;
    NOTIFYICONDATA* mpNotifyIconData;
    HWND mhWnd;
    HMENU mhMenu;
};

#endif /* TrayIcon_h */
