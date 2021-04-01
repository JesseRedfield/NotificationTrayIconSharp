#include "common.h"
#include "IMenuContainer.h"
#include "ITrayMenuItem.h"

namespace notification_tray_icon_private
{
    IMenuContainer::IMenuContainer()
    {
        _MenuItems.clear();
    }

    IMenuContainer::~IMenuContainer()
    {
        _MenuItems.clear();
    }

    bool IMenuContainer::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if (HasMenuItem(pTrayMenuItem))
            return false;

        _MenuItems[pTrayMenuItem->GetUUID()] = pTrayMenuItem;

        return true;
    }

    bool IMenuContainer::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse)
    {
        bool bReturnVal = false;
        if (recurse)
        {
            for (std::map<uint32_t, ITrayMenuItem *>::iterator it = _MenuItems.begin(); it != _MenuItems.end(); it++)
            {
                if (it->second->RemoveMenuItem(pTrayMenuItem, recurse))
                    bReturnVal = true;
            }
        }

        std::map<uint32_t, ITrayMenuItem *>::const_iterator it = _MenuItems.find(pTrayMenuItem->GetUUID());
        if (it != _MenuItems.end())
        {
            _MenuItems.erase(it);
            bReturnVal = true;
        }

        return bReturnVal;
    }

    bool IMenuContainer::HasMenuItem(ITrayMenuItem *pTrayMenu)
    {
        std::map<uint32_t, ITrayMenuItem *>::const_iterator it = _MenuItems.find(pTrayMenu->GetUUID());
        return it != _MenuItems.end();
    }

    uint32_t IMenuContainer::MenuItemId = 1000;
    uint32_t IMenuContainer::GenerateId()
    {
        //over 4 billion IDs, no one is making a tray menu that big
        return MenuItemId++;
    }
}
