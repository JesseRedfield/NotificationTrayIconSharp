#ifndef __TRAY_MENU_H__
#define __TRAY_MENU_H__
#import <AppKit/NSMenuItem.h>
#import "ButonProxy.hpp"
#import "ButtonProxyTarget.hpp"
#import "IMenuContainer.hpp"
#include <cstdint>

class CTrayMenuItem : IMenuContainer, IButtonProxyTarget
{
public:
    CTrayMenuItem(uint64_t uuid, const char* pszText);

    ~CTrayMenuItem();

    void SetText(const char* pszText);
    
    bool SetDisabled(bool disabled);

    bool GetDisabled();

    bool SetChecked(bool checked);
    
    bool GetChecked();
    
    uint64_t GetUUID();
    
    NSMenuItem* GetNSMenuItem();
    
    bool AddMenuItem(CTrayMenuItem* pTrayMenu);
    
    void OnSelected();
    
    //void SetSelectedCallback(MenuItemSelectedEventCallback callback);

private:
    //MenuItemSelectedEventCallback mSelectedCallback;
    bool mbDisabled;
    bool mbChecked;
    uint64_t mUUID;
    NSString* mpTitle;
    NSMenuItem* mpMenuItem;
    ButtonProxy* mpButtonProxy;
};

#endif  // __TRAY_MENU_H__
