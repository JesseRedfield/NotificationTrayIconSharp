#include "TrayMenuItem.h"
#include "TrayIcon.h"

namespace notification_tray_icon_private
{
    CTrayMenuItem::CTrayMenuItem(const CSCHAR *pszText) : ITrayMenuItem(pszText)
    {
        mpText = NULL;
        HMENU mhMenu = NULL;
        memset(&mhMenuItem, 0, sizeof(MENUITEMINFOW));

        SetText(pszText);
    }

    CTrayMenuItem::~CTrayMenuItem()
    {
        SAFE_DELETE(mhMenu);
        SAFE_DELETE(mpText);
    }

    void CTrayMenuItem::SetText(const CSCHAR *pszText)
    {
        SAFE_DELETE(mpText);

        size_t size = CSSTRLEN(pszText) + 1;
        mpText = new CSCHAR[size];
        CSSTRCPY(mpText, pszText, size);

        RebuildMenu();
    }

    void CTrayMenuItem::SetChecked(bool checked)
    {
        ITrayMenuItem::SetChecked(checked);

        RebuildMenu();
    }

    void CTrayMenuItem::SetDisabled(bool disabled)
    {
        ITrayMenuItem::SetDisabled(disabled);

        RebuildMenu();
    }

    bool CTrayMenuItem::AddMenuItem(CTrayMenuItem *pTrayMenuItem)
    {
        if (ITrayMenuItem::AddMenuItem(pTrayMenuItem))
        {
            RebuildMenu();

            return true;
        }

        return false;
    }

    bool CTrayMenuItem::RemoveMenuItem(CTrayMenuItem *pTrayMenuItem, bool recurse)
    {
        if (ITrayMenuItem::AddMenuItem(pTrayMenuItem))
        {
            RebuildMenu();

            return true;
        }

        return false;
    }

    void CTrayMenuItem::RebuildMenu()
    {
        if (_pOwner != NULL)
            ((CTrayIcon *)_pOwner)->RebuildMenu();
    }

    void CTrayMenuItem::RebuildMenu(HMENU hMenu)
    {
        if (mhMenu != NULL)
        {
            DestroyMenu(mhMenu);
            mhMenu = NULL;
        }

        if (!_MenuItems.empty())
        {
            mhMenu = CreatePopupMenu();

            for (std::map<uint32_t, ITrayMenuItem *>::iterator it = _MenuItems.begin(); it != _MenuItems.end(); it++)
            {
                CTrayMenuItem *pValue = (CTrayMenuItem *)it->second;
                pValue->RebuildMenu(mhMenu);
            }
        }

        ConstructMenuItemInfo();
        InsertMenuItemW(hMenu, _UUID, FALSE, &mhMenuItem);
    }

    void CTrayMenuItem::ConstructMenuItemInfo()
    {
        memset(&mhMenuItem, 0, sizeof(MENUITEMINFOW));
        mhMenuItem.cbSize = sizeof(MENUITEMINFOW);
        mhMenuItem.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
        mhMenuItem.fType = 0;
        mhMenuItem.fState = 0;

        mhMenuItem.wID = _UUID;
        mhMenuItem.dwTypeData = mpText;
        mhMenuItem.dwItemData = (ULONG_PTR)this;

        if (_bDisabled)
        {
            mhMenuItem.fState |= MFS_DISABLED;
        }

        if (_bChecked)
        {
            mhMenuItem.fState |= MFS_CHECKED;
        }

        if (mhMenu != NULL)
        {
            mhMenuItem.fMask = mhMenuItem.fMask | MIIM_SUBMENU;
            mhMenuItem.hSubMenu = mhMenu;
        }
    }
}