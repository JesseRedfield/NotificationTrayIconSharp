#include "TrayIconMac.h"

using namespace notification_tray_icon;

namespace notification_tray_icon_private
{
    void CTrayIconMac::InitializeMenu()
    {
        pStatusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength: -1.0] retain];
        [pStatusItem setHighlightMode: true];
        [pStatusItem setEnabled: true];
        
        NSString *pIconPath = [[NSString alloc] initWithCString: mpszIconPath encoding: NSUTF8StringEncoding];
        NSImage* pImage = [[NSImage alloc]initWithContentsOfFile: pIconPath];
        //[[pStatusItem button] setImage: from
        
    }
}
