//
//  ViewController.m
//  TrayIconPlayground
//
//  Created by Jesse Redfield on 3/26/21.
//

#import "ViewController.h"
#include "macos/TrayIcon.h"
#include "NotificationTrayIcon.h"

using namespace notification_tray_icon;
using namespace notification_tray_icon_private;

static CTrayIcon* pIcon;

void sub_item_clicked(void* pParam)
{
      CTrayMenuItem* pMenuItem = (CTrayMenuItem*)pParam;
      pIcon->RemoveMenuItem(pMenuItem, true);
}
void item_clicked(void* pParam)
{
      CTrayMenuItem* pMenuItem = (CTrayMenuItem*)pParam;

    const char* text = "World";
    CTrayMenuItem* pNewMenuItem = new CTrayMenuItem(text);
    pNewMenuItem->SetSelectedCallback(sub_item_clicked);

    pMenuItem->AddMenuItem(pNewMenuItem);

    const char* clicked = "Clicked";
    pMenuItem->SetChecked(true);
    pMenuItem->SetDisabled(true);
    pMenuItem->SetText(clicked);
}

void icon_clicked(void* pParam)
{
    CTrayIcon* pTrayIcon = (CTrayIcon*)pParam;

    const char* text = "Hello";
    CTrayMenuItem* pMenuItem = new CTrayMenuItem(text);
    pMenuItem->SetSelectedCallback(item_clicked);

    pTrayIcon->AddMenuItem(pMenuItem);
}

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    pIcon = TrayIcon_Create();
    const char* iconPath = "/Users/jesser/dev/SystemTrayIcon/icon.jpg";
    pIcon->InitializeMenu(iconPath);
    pIcon->SetSelectedCallback(icon_clicked);
    
    //delete pIcon;
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
