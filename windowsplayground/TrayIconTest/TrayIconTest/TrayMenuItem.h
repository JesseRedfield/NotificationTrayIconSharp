#ifndef __TRAY_MENU_H__
#define __TRAY_MENU_H__
#include "IMenuContainer.h"
#include <windows.h>
#include <shellapi.h>
#include <cstdint>

class CTrayIcon;

class CTrayMenuItem : IMenuContainer
{
public:
    CTrayMenuItem(uint64_t uuid, const char* pszText);

    ~CTrayMenuItem();

    void SetOwner(CTrayIcon* pOwner);

    void SetText(const char* pszText);
    
    bool SetDisabled(bool disabled);

    bool GetDisabled();

    bool SetChecked(bool checked);
    
    bool GetChecked();
    
    uint64_t GetUUID();
       
    bool AddMenuItem(CTrayMenuItem* pTrayMenu);
    
    void OnSelected();
    
    void RebuildMenu(HMENU hMenu);
    //void SetSelectedCallback(MenuItemSelectedEventCallback callback);

private:
    //MenuItemSelectedEventCallback mSelectedCallback;
    bool mbDisabled;
    bool mbChecked;
    uint64_t mUUID;
    MENUITEMINFO mhMenuItem;
    HMENU mhMenu;
    CTrayIcon* mpOwner;
};

#endif  // __TRAY_MENU_H__


//MENUITEMINFO item;
//memset(&item, 0, sizeof(item));
//item.cbSize = sizeof(MENUITEMINFO);
//item.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
//item.fType = 0;
//item.fState = 0;