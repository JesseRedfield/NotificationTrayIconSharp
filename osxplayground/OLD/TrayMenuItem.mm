#include "TrayMenuItem.hpp"
#include <cstdlib>
#include <string.h>


CTrayMenuItem::CTrayMenuItem(uint64_t uuid, const char* pszText)
{
    mUUID = uuid;
    mpTitle = NULL;
    mbDisabled = false;
    mbChecked = false;
    mpButtonProxy = [[ButtonProxy alloc] initWithOwner: this];
    mpMenuItem = [NSMenuItem alloc];
    
    [mpMenuItem setEnabled: true];
    [mpMenuItem setTarget: mpButtonProxy];
    [mpMenuItem setAction: @selector(leftClick:)];
    
    SetText(pszText);
}

CTrayMenuItem::~CTrayMenuItem()
{
#if !__has_feature(objc_arc)
    if(mpTitle != NULL)
    {
        [mpTitle release];
        mpTitle = NULL;
    }
    
    if(mpMenuItem != NULL)
    {
        [mpMenuItem release];
        mpMenuItem = NULL;
    }
    
    if(mpButtonProxy != NULL)
    {
        [mpButtonProxy release];
        mpButtonProxy = NULL;
    }
#endif
}

void CTrayMenuItem::SetText(const char* pszText)
{
#if !__has_feature(objc_arc)
    if(mpTitle != NULL)
    {
        [mpTitle release];
        mpTitle = NULL;
    }
#endif
    
    if(pszText == NULL)
        mpTitle = [[NSString alloc] initWithString: @""];
    else
        mpTitle = [[NSString alloc] initWithCString: pszText encoding: NSUTF8StringEncoding];
    
    [mpMenuItem setTitle: mpTitle];
}

bool CTrayMenuItem::SetDisabled(bool disabled)
{
    mbDisabled = disabled;
    [mpMenuItem setEnabled: !disabled];

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

NSMenuItem* CTrayMenuItem::GetNSMenuItem()
{
    return mpMenuItem;
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
    [mpMenuItem setState: mbChecked ? 1 : 0];
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
        [mpMenuItem setSubmenu: mpMenu];
        [mpMenu addItem: pTrayMenu->GetNSMenuItem()];

        return true;
    }
    
    return false;
}

//void CTrayMenuItem::SetSelectedCallback(MenuItemSelectedEventCallback callback)
//{
//    mSelectedCallback = callback;
//}
