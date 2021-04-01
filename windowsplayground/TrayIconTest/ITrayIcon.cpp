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

    void ITrayIcon::OnSelected()
    {
        if (_SelectedCallback != NULL)
            _SelectedCallback(this);
    }

    bool ITrayIcon::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        return IMenuContainer::AddMenuItem(pTrayMenuItem);
    }

    bool ITrayIcon::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        return IMenuContainer::RemoveMenuItem(pTrayMenuItem);
    }
}
