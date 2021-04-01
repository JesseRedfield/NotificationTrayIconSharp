#import "TrayIcon.hpp"
#import "TrayMenuItem.hpp"
#import <Foundation/Foundation.h>

CTrayIcon::CTrayIcon()
{
    mpButtonProxy = NULL;
    mpStatusItem = NULL;
    mpImage = NULL;
    mpIconPath = NULL;
    
    mpButtonProxy = [[ButtonProxy alloc] initWithOwner: this];
}
    
CTrayIcon::~CTrayIcon()
{
#if !__has_feature(objc_arc)
    if(mpStatusItem != NULL)
    {
        [[NSStatusBar systemStatusBar] removeStatusItem: mpStatusItem];
        [mpStatusItem release];
        mpStatusItem = NULL;
    }

    if(mpImage != NULL)
    {
        [mpImage release];
        mpImage = NULL;
    }
    
    if(mpIconPath != NULL)
    {
        [mpIconPath release];
        mpIconPath = NULL;
    }
    
    if(mpButtonProxy != NULL)
    {
        [mpButtonProxy release];
        mpButtonProxy = NULL;
    }
#endif
}

void CTrayIcon::InitializeMenu(const char* pszIconPath)
{
    mpStatusItem = [[NSStatusBar systemStatusBar] statusItemWithLength: NSSquareStatusItemLength];
    
    [[mpStatusItem button] setImage: mpImage];
    [[mpStatusItem button] setEnabled: true];
    [[mpStatusItem button] setTarget: mpButtonProxy];
    [[mpStatusItem button] setAction: @selector(leftClick:)];
    [mpStatusItem retain];
    SetIcon(pszIconPath);
}

void CTrayIcon::SetIcon(const char* pszIconPath)
{
#if !__has_feature(objc_arc)
    if(mpImage != NULL)
    {
        [mpImage release];
        mpImage = NULL;
    }
    
    if(mpIconPath != NULL)
    {
        [mpIconPath release];
        mpIconPath = NULL;
    }
#endif
    
    if(pszIconPath == NULL) return;
    
    mpIconPath = [[NSString alloc] initWithCString: pszIconPath encoding: NSUTF8StringEncoding];
    mpImage = [[NSImage alloc] initWithContentsOfFile: mpIconPath];
    
    [[mpStatusItem button] setImage: mpImage];
}

void CTrayIcon::OnSelected()
{
    CTrayMenuItem* pMenuItem = new CTrayMenuItem(1000, "Hello");
    AddMenuItem(pMenuItem);
}

bool CTrayIcon::AddMenuItem(CTrayMenuItem *pTrayMenu)
{
    if(IMenuContainer::AddMenuItem(pTrayMenu))
    {
        [mpStatusItem setMenu: mpMenu];
        [mpMenu addItem: pTrayMenu->GetNSMenuItem()];

        return true;
    }
    
    return false;
}


//void CTrayIcon::SetSelectedCallback(MenuItemSelectedEventCallback callback)
//{
//    mSelectedCallback = callback;
//}
