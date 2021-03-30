//
//  ViewController.m
//  TrayIconPlayground
//
//  Created by Jesse Redfield on 3/26/21.
//

#import "ViewController.h"
#include "TrayIcon.hpp"

static CTrayIcon* pIcon;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    pIcon = new CTrayIcon();
    pIcon->InitializeMenu("/Users/jesser/dev/SystemTrayIcon/icon.jpg");
    pIcon->SetIcon("/Users/jesser/dev/SystemTrayIcon/icon.png");
//    
    CTrayMenuItem* pItem = new CTrayMenuItem(1000, "Hello");
    CTrayMenuItem* pItem2 = new CTrayMenuItem(1001, "World");
    
    pIcon->AddMenuItem(pItem);
    pItem->SetText("World");
    pItem->AddMenuItem(pItem2);
    
    //delete pIcon;
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
