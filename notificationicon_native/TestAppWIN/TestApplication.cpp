#include <iostream>
#include "..\src\NotificationTrayIcon.h"

using namespace notification_tray_icon_private;
using namespace notification_tray_icon;

void *mpTrayIcon = NULL;
void *mpSubMenu = NULL;

void __stdcall set_checked(void *pParam)
{
    CTrayMenuItem *pParentItem = (CTrayMenuItem *)pParam;

    bool checked = TrayMenuItem_GetChecked(pParentItem);
    TrayMenuItem_SetChecked(pParentItem, !checked);
}

void __stdcall set_disabled(void *pParam)
{
    CTrayMenuItem *pParentItem = (CTrayMenuItem *)pParam;

    TrayMenuItem_SetDisabled(pParentItem, true);
}

void __stdcall set_text(void *pParam)
{
    CTrayMenuItem *pParentItem = (CTrayMenuItem *)pParam;
    std::wstring text(L"Changed Text!");
    const wchar_t *szText = text.c_str();

    TrayMenuItem_SetText(pParentItem, szText);
}

void __stdcall item_remove(void *pParam)
{
    CTrayMenuItem *pMenuItem = (CTrayMenuItem *)pParam;

    TrayMenuItem_RemoveMenuItem((CTrayMenuItem *)mpSubMenu, pMenuItem);
}

void __stdcall item_remove_recurse(void *pParam)
{
    CTrayMenuItem *pMenuItem = (CTrayMenuItem *)pParam;

    TrayIcon_RemoveMenuItem((CTrayIcon *)mpTrayIcon, pMenuItem, true);
}

void __stdcall create_submenu(void *pParam)
{
    CTrayMenuItem *pParentItem = (CTrayMenuItem *)pParam;

    std::wstring check(L"Item Remove Recurse");
    const wchar_t *szCheck = check.c_str();
    void *pMenuItem1 = TrayMenuItem_Create(szCheck);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem1, item_remove_recurse);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem *)pMenuItem1);

    std::wstring disable(L"Item Remove");
    const wchar_t *szDisable = disable.c_str();
    void *pMenuItem2 = TrayMenuItem_Create(szDisable);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem2, item_remove);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem *)pMenuItem2);
}

void __stdcall item_clicked(void *pParam)
{
    CTrayMenuItem *pParentItem = (CTrayMenuItem *)pParam;

    std::wstring check(L"Check On Click");
    const wchar_t *szCheck = check.c_str();
    void *pMenuItem1 = TrayMenuItem_Create(szCheck);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem1, set_checked);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem *)pMenuItem1);

    std::wstring disable(L"Disable On Click");
    const wchar_t *szDisable = disable.c_str();
    void *pMenuItem2 = TrayMenuItem_Create(szDisable);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem2, set_disabled);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem *)pMenuItem2);

    std::wstring text(L"Set Text");
    const wchar_t *szText = text.c_str();
    void *pMenuItem3 = TrayMenuItem_Create(szText);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem3, set_text);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem *)pMenuItem3);

    std::wstring submenu(L"Create Submenu");
    const wchar_t *szSubmenu = submenu.c_str();
    mpSubMenu = TrayMenuItem_Create(szSubmenu);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)mpSubMenu, create_submenu);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem *)mpSubMenu);
}

void __stdcall icon_clicked(void *pParam)
{
    std::wstring text(L"Hello");
    const wchar_t *szText = text.c_str();
    void *pMenuItem = TrayMenuItem_Create(szText);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem, item_clicked);
    TrayIcon_AddMenuItem((CTrayIcon *)mpTrayIcon, (CTrayMenuItem *)pMenuItem);

    std::wstring world(L"World");
    const wchar_t *szWorld = world.c_str();
    void *pMenuItem2 = TrayMenuItem_Create(szWorld);
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem *)pMenuItem2, item_remove_recurse);
    TrayIcon_AddMenuItem((CTrayIcon *)mpTrayIcon, (CTrayMenuItem *)pMenuItem2);
}

int main()
{
    std::cout << "Hello World!\n";
    mpTrayIcon = TrayIcon_Create();

    //SET AN ICON PATH HERE PNG/JPG on OSX
    std::wstring icon(L"C:\\PATH\\TO\\YOUR\\ICON");
    const wchar_t *szIcon = icon.c_str();
    TrayIcon_Initialize((notification_tray_icon_private::CTrayIcon *)mpTrayIcon, szIcon);
    TrayIcon_SetSelectedCallback((notification_tray_icon_private::CTrayIcon *)mpTrayIcon, (MenuItemSelectedEventCallback)&icon_clicked);

    while (true)
    {
        TrayIcon_PumpMessageLoop((notification_tray_icon_private::CTrayIcon *)mpTrayIcon, false);
    };
}