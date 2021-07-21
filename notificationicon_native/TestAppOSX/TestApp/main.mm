#import <Foundation/Foundation.h>
#include "../../src/NotificationTrayIcon.h"

using namespace notification_tray_icon;
using namespace notification_tray_icon_private;

void *mpTrayIcon = NULL;
void *mpSubMenu = NULL;
bool destroy = false;

void set_checked(void* pParam)
{
    CTrayMenuItem* pParentItem = (CTrayMenuItem*)pParam;

    bool checked = TrayMenuItem_GetChecked(pParentItem);
    TrayMenuItem_SetChecked(pParentItem, !checked);
}

void set_disabled(void* pParam)
{
    CTrayMenuItem* pParentItem = (CTrayMenuItem*)pParam;

    TrayMenuItem_SetDisabled(pParentItem, true);
}

void set_text(void* pParam)
{
    CTrayMenuItem* pParentItem = (CTrayMenuItem*)pParam;
    TrayMenuItem_SetText(pParentItem, "Changed Text!");
}

void item_remove(void* pParam)
{
    CTrayMenuItem* pMenuItem = (CTrayMenuItem*)pParam;

    TrayMenuItem_RemoveMenuItem((CTrayMenuItem*)mpSubMenu, pMenuItem);
}

void item_remove_recurse(void* pParam)
{
    CTrayMenuItem* pMenuItem = (CTrayMenuItem*)pParam;

    TrayIcon_RemoveMenuItem((CTrayIcon*)mpTrayIcon, pMenuItem, true);
    TrayMenuItem_Destroy((CTrayMenuItem*) pMenuItem);
}

void menu_destroy(void* pParam)
{
    destroy = true;
}

void create_submenu(void* pParam)
{
    CTrayMenuItem* pParentItem = (CTrayMenuItem*)pParam;

    void* pMenuItem1 = TrayMenuItem_Create("Item Remove Recurse");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem1, item_remove_recurse);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)pMenuItem1);

    void* pMenuItem2 = TrayMenuItem_Create("Item Remove");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem2, item_remove);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)pMenuItem2);
    
    void* pMenuItem3 = TrayMenuItem_Create("Menu Destroy");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem3, menu_destroy);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)pMenuItem3);
}

void item_clicked(void* pParam)
{
    CTrayMenuItem* pParentItem = (CTrayMenuItem*)pParam;

    void* pMenuItem1 = TrayMenuItem_Create("Check On Click");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem1, set_checked);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)pMenuItem1);

    void* pMenuItem2 = TrayMenuItem_Create("Disable On Click");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem2, set_disabled);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)pMenuItem2);

    void* pMenuItem3 = TrayMenuItem_Create("Set Text");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem3, set_text);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)pMenuItem3);

    mpSubMenu = TrayMenuItem_Create("Create Submenu");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)mpSubMenu, create_submenu);
    TrayMenuItem_AddMenuItem(pParentItem, (CTrayMenuItem*)mpSubMenu);
}

void icon_clicked(void *pParam)
{
    void *pMenuItem = TrayMenuItem_Create("Hello");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem, item_clicked);
    TrayIcon_AddMenuItem((CTrayIcon*)mpTrayIcon, (CTrayMenuItem*)pMenuItem);

    void *pMenuItem2 = TrayMenuItem_Create("World");
    TrayMenuItem_SetSelectedCallback((CTrayMenuItem*)pMenuItem2, item_remove_recurse);
    TrayIcon_AddMenuItem((CTrayIcon *)mpTrayIcon, (CTrayMenuItem *)pMenuItem2);
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        mpTrayIcon = TrayIcon_Create();
        
        //SET AN ICON PATH HERE PNG/JPG on OSX PNG/JPG/ICO Windows
        TrayIcon_Initialize((CTrayIcon *)mpTrayIcon, "/PATH/TO/YOUR/ICON.png");
        TrayIcon_SetSelectedCallback((CTrayIcon *)mpTrayIcon, (MenuItemSelectedEventCallback)&icon_clicked);
        
        Toast_Initialize("ExampleAppId", "My Example App", "/PATH/TO/YOUR/ICON.png");

        Toast_SendNotification("Hello", "World", "APP ID 1212", "/PATH/TO/YOUR/ICON.png");

        Toast_SendNotification("", "No title empty string", "APP ID 324", "/PATH/TO/YOUR/ICON.png");

        Toast_SendNotification(NULL, "No title NULL string", "APP ID 345", "/PATH/TO/YOUR/ICON.png");
        
        while (true)
        {
            if(mpTrayIcon != NULL)
            {
                TrayIcon_PumpMessageLoop((CTrayIcon *)mpTrayIcon, false);
            }
            
            if(destroy)
            {
                TrayIcon_Destroy((CTrayIcon *)mpTrayIcon);
                mpTrayIcon = NULL;
            }
        
        };
    }
    return 0;
}
