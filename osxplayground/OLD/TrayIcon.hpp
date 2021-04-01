#import <AppKit/NSMenu.h>
#import <AppKit/NSImage.h>
#import <AppKit/NSStatusBar.h>
#import <AppKit/NSStatusBarButton.h>
#import <AppKit/NSStatusItem.h>
#import <Cocoa/Cocoa.h>
#import "ButonProxy.hpp"
#import "ButtonProxyTarget.hpp"
#import "TrayMenuItem.hpp"
#import "IMenuContainer.hpp"

#ifndef TrayIcon_h
#define TrayIcon_h

class CTrayIcon : IMenuContainer, IButtonProxyTarget
{
public:
    CTrayIcon();
    
    ~CTrayIcon();

    void InitializeMenu(const char* pszIconPath);

    void SetIcon(const char* pszIconPath);
    
    void OnSelected();
    
    bool AddMenuItem(CTrayMenuItem* pTrayMenu);
    
    //void SetSelectedCallback(MenuItemSelectedEventCallback callback);
    
private:
    //MenuItemSelectedEventCallback mSelectedCallback;
    NSStatusItem* mpStatusItem;
    NSImage* mpImage;
    NSString* mpIconPath;
    ButtonProxy *mpButtonProxy;
};

#endif /* TrayIcon_h */
