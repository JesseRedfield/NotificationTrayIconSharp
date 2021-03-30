#ifndef __IMENU_CONTAINER_H__
#define __IMENU_CONTAINER_H__
#import <AppKit/NSMenu.h>
#include <map>
#include <cstdint>

// forward declarations
class CTrayMenuItem;

class IMenuContainer
{
public:
    
    IMenuContainer();
    
    ~IMenuContainer();
    
    bool AddMenuItem(CTrayMenuItem* pTrayMenu);

    bool HasMenuItem(CTrayMenuItem* pTrayMenu);

    bool RemoveMenuItem(CTrayMenuItem* pTrayMenu);

protected:
    NSMenu* mpMenu;
    std::map<uint64_t, CTrayMenuItem*> mMenuItems;
};

#endif // __IMENU_CONTAINER_H__
