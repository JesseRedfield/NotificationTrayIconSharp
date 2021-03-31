#include "TrayMenuItem.h"
#include "TrayIcon.h"
#include <cstdlib>
#include <string.h>

CTrayMenuItem::CTrayMenuItem(uint64_t uuid, const char* pszText)
{
    mUUID = uuid;
    mbDisabled = false;
    mbChecked = false;
    mhMenu = NULL;
    mpOwner = NULL;
    memset(&mhMenuItem, 0, sizeof(MENUITEMINFO));

    SetText(pszText);
}

CTrayMenuItem::~CTrayMenuItem()
{

}

void CTrayMenuItem::SetOwner(CTrayIcon* pOwner)
{
    mpOwner = pOwner;
    for (std::map<uint64_t, CTrayMenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); it++) {
        it->second->SetOwner(pOwner);
    }
}

void CTrayMenuItem::SetText(const char* pszText)
{
    if (mpText != NULL) {
        delete mpText;
        mpText = NULL;
    }

    mpText = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, pszText, -1, mpText, 4096);
}

bool CTrayMenuItem::SetDisabled(bool disabled)
{
    mbDisabled = disabled;
    if (mpOwner != NULL) mpOwner->RebuildMenu();
    return mbDisabled;
}

bool CTrayMenuItem::GetDisabled()
{
    return mbDisabled;
}

uint64_t CTrayMenuItem::GetUUID()
{
    return mUUID;
}

void CTrayMenuItem::OnSelected()
{
    if(!mbChecked)
    {
        SetChecked(!mbChecked);
    }
    else
    {
        CTrayMenuItem* pMenuItem = new CTrayMenuItem(1001, "World");
        AddMenuItem(pMenuItem);
    }
}

bool CTrayMenuItem::SetChecked(bool checked)
{
    mbChecked = checked;
    if (mpOwner != NULL) mpOwner->RebuildMenu();
    return mbChecked;
}

bool CTrayMenuItem::GetChecked()
{
    return mbChecked;
}

bool CTrayMenuItem::AddMenuItem(CTrayMenuItem* pTrayMenu)
{
    if(IMenuContainer::AddMenuItem(pTrayMenu))
    {
        pTrayMenu->SetOwner(mpOwner);
        if (mpOwner != NULL) mpOwner->RebuildMenu();
        return true;
    }
    
    return false;
}

void CTrayMenuItem::RebuildMenu(HMENU hMenu)
{
    if (mhMenu != NULL) {
        DestroyMenu(mhMenu);
        mhMenu = NULL;
    }

    memset(&mhMenuItem, 0, sizeof(MENUITEMINFO));
    mhMenuItem.cbSize = sizeof(MENUITEMINFO);
    mhMenuItem.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
    mhMenuItem.fType = 0;
    mhMenuItem.fState = 0;

    mhMenuItem.wID = mUUID;
    mhMenuItem.dwTypeData = new wchar_t[4096];
    mhMenuItem.dwItemData = (ULONG_PTR)this;
    MultiByteToWideChar(CP_ACP, 0, "HELLO WORLD", -1, mhMenuItem.dwTypeData, 4096);

    if (mbDisabled) {
        mhMenuItem.fState |= MFS_DISABLED;
    }
    if (mbChecked) {
        mhMenuItem.fState |= MFS_CHECKED;
    }
    if (!mMenuItems.empty())
    {
        mhMenu = CreatePopupMenu();

        for (std::map<uint64_t, CTrayMenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); it++) {
            it->second->RebuildMenu(mhMenu);
        }

        mhMenuItem.fMask = mhMenuItem.fMask | MIIM_SUBMENU;
        mhMenuItem.hSubMenu = mhMenu;
    }

    InsertMenuItem(hMenu, 0, TRUE, &mhMenuItem);
}

//void CTrayMenuItem::SetSelectedCallback(MenuItemSelectedEventCallback callback)
//{
//    mSelectedCallback = callback;
//}
