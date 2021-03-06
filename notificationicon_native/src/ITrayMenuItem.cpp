#include "ITrayMenuItem.h"
#include "ITrayIcon.h"

namespace notification_tray_icon_private
{
    ITrayMenuItem::ITrayMenuItem(const CSCHAR *pszText)
    {
        _UUID = ITrayMenuItem::GenerateId();
        _bDisabled = false;
        _bChecked = false;
        _pOwner = NULL;
        _SelectedCallback = NULL;
    }

    ITrayMenuItem::~ITrayMenuItem()
    {
    }

    uint32_t ITrayMenuItem::GetUUID()
    {
        return _UUID;
    }

    void ITrayMenuItem::SetOwner(ITrayIcon *pOwner)
    {
        _pOwner = pOwner;

        for (std::map<uint32_t, ITrayMenuItem *>::iterator it = _MenuItems.begin(); it != _MenuItems.end(); it++)
        {
            it->second->SetOwner(pOwner);
        }
    }

    bool ITrayMenuItem::GetChecked()
    {
        return _bChecked;
    }

    void ITrayMenuItem::SetChecked(bool checked)
    {
        _bChecked = checked;
    }

    bool ITrayMenuItem::GetDisabled()
    {
        return _bDisabled;
    }

    void ITrayMenuItem::SetDisabled(bool disabled)
    {
        _bDisabled = disabled;
    }

    void ITrayMenuItem::SetSelectedCallback(MenuItemSelectedEventCallback callback)
    {
        _SelectedCallback = callback;
    }

    bool ITrayMenuItem::AddMenuItem(ITrayMenuItem *pTrayMenuItem)
    {
        if (IMenuContainer::AddMenuItem(pTrayMenuItem))
        {
            pTrayMenuItem->SetOwner(_pOwner);

            return true;
        }

        return false;
    }

    bool ITrayMenuItem::RemoveMenuItem(ITrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if (IMenuContainer::RemoveMenuItem(pTrayMenuItem, recurse))
        {
            pTrayMenuItem->SetOwner(NULL);

            return true;
        }

        return false;
    }
}
