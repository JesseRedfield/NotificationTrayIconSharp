#include "ITrayIcon.h"

namespace notification_tray_icon_private
{
    ITrayIcon::ITrayIcon() : IMenuContainer()
    {
        _SelectedCallback = NULL;
    }

    ITrayIcon::~ITrayIcon()
    {
    }

    void ITrayIcon::SetSelectedCallback(MenuItemSelectedEventCallback callback)
    {
        _SelectedCallback = callback;
    }

    bool ITrayIcon::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if (IMenuContainer::AddMenuItem(pTrayMenuItem))
        {
            pTrayMenuItem->SetOwner(this);

            return true;
        }

        return false;
    }

    bool ITrayIcon::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if (IMenuContainer::RemoveMenuItem(pTrayMenuItem, recurse))
        {
            pTrayMenuItem->SetOwner(NULL);

            return true;
        }

        return false;
    }
}
