#ifndef __IMENU_CONTAINER_H__
#define __IMENU_CONTAINER_H__
#include <map>
#include "common.h"

namespace notification_tray_icon_private
{
    class ITrayMenuItem;

    class IMenuContainer
    {
    public:
        IMenuContainer();

        ~IMenuContainer();

        bool AddMenuItem(ITrayMenuItem *pTrayMenuItem);

        bool HasMenuItem(ITrayMenuItem *pTrayMenuItem);

        bool RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse = false);

        static uint32_t GenerateId();

    protected:
        std::map<uint32_t, ITrayMenuItem *> _MenuItems;
    
    private:
        static uint32_t MenuItemId;
    };
}

#endif // __IMENU_CONTAINER_H__
