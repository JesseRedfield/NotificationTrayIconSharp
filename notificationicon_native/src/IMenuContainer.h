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

        static uint32_t GenerateId();
        
        bool HasMenuItem(ITrayMenuItem *pTrayMenuItem);
        
        virtual bool AddMenuItem(ITrayMenuItem *pTrayMenuItem);
        
        virtual bool RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse = false);
        
    protected:
        std::map<uint32_t, ITrayMenuItem *> _MenuItems;

    private:
        static uint32_t MenuItemId;
    };
}

#endif // __IMENU_CONTAINER_H__
