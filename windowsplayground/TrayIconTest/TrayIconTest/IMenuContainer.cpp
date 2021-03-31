#include "IMenuContainer.h"
#include "TrayMenuItem.h"

IMenuContainer::IMenuContainer()
{

}
IMenuContainer::~IMenuContainer()
{
    mMenuItems.clear();
}

bool IMenuContainer::AddMenuItem(CTrayMenuItem* pTrayMenu)
{  
    if(HasMenuItem(pTrayMenu))
        return false;
    
    mMenuItems[pTrayMenu->GetUUID()] = pTrayMenu;
        
    return true;
}

bool IMenuContainer::HasMenuItem(CTrayMenuItem* pTrayMenu)
{
    std::map<uint64_t, CTrayMenuItem*>::const_iterator it = mMenuItems.find(pTrayMenu->GetUUID());
    return it!=mMenuItems.end();
}

bool IMenuContainer::RemoveMenuItem(CTrayMenuItem* pTrayMenu)
{
    std::map<uint64_t, CTrayMenuItem*>::const_iterator it = mMenuItems.find(pTrayMenu->GetUUID());
    if(it!=mMenuItems.end()) return false;
    
    mMenuItems.erase(it);
    
    return true;
}