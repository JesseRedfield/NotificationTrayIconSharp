#import "IMenuContainer.hpp"
#import "TrayMenuItem.hpp"

IMenuContainer::IMenuContainer()
{
    mpMenu = NULL;
}
IMenuContainer::~IMenuContainer()
{
    mMenuItems.clear();
    
#if !__has_feature(objc_arc)
    if(mpMenu != NULL)
    {
        [mpMenu release];
    }
#endif
}

bool IMenuContainer::AddMenuItem(CTrayMenuItem* pTrayMenu)
{
    if(mpMenu == NULL)
    {
        mpMenu = [NSMenu alloc];
    }
    
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
    [mpMenu removeItem: pTrayMenu->GetNSMenuItem()];
    
    return true;
}
