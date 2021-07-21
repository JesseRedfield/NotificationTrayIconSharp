#include "Toast.h"
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <objc/runtime.h>

#pragma mark - Swizzle NSBundle

NSString *bundleIdentifier = nil; 

@implementation NSBundle(swizle)

// Overriding bundleIdentifier works, but overriding NSUserNotificationAlertStyle does not work.

- (NSString *)__bundleIdentifier
{
    if (self == [NSBundle mainBundle]) {
        return bundleIdentifier ? bundleIdentifier : @"com.apple.finder";
    } else {
        return [self __bundleIdentifier];
    }
}

@end

BOOL installNSBundleHook()
{
    Class bundleClass = objc_getClass("NSBundle");
    if (bundleClass) {
        method_exchangeImplementations(class_getInstanceMethod(bundleClass, @selector(bundleIdentifier)),
                                       class_getInstanceMethod(bundleClass, @selector(__bundleIdentifier)));
        return YES;
    }
    return NO;
}

#pragma mark - NotificationCenterDelegate

@interface NotificationCenterDelegate : NSObject<NSUserNotificationCenterDelegate>

@property (nonatomic, assign) BOOL keepRunning;

@end

@implementation NotificationCenterDelegate

- (void)userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification
{
    self.keepRunning = YES;
}

-(void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
    if (notification.activationType != NSUserNotificationActivationTypeContentsClicked &&
        notification.activationType != NSUserNotificationActivationTypeActionButtonClicked) {
        return;
    }

    notification_tray_icon_private::Toast::HandleCallback((void*)CFBridgingRetain(notification));
}

@end

#pragma mark -
namespace notification_tray_icon_private
{
    bool Toast::Initialized = false;
    NotificationCenterDelegate *notificationCenterDelegate;

    NotificationSelectedEventCallback Toast::NotificationSelectedCallback = NULL;

    void Toast::Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName, const CSCHAR *pszIconPath)
    {
        if(Toast::Initialized) return;

        @autoreleasepool {
            bundleIdentifier = pszAppId != NULL ? [[NSString alloc] initWithCString: pszAppId encoding: NSUTF8StringEncoding] : @"";

            Toast::Initialized = installNSBundleHook();
            notificationCenterDelegate = [NotificationCenterDelegate new];

            NSUserNotificationCenter* notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];
            notificationCenter.delegate = notificationCenterDelegate;
        }
    }

    void Toast::SendNotification(const CSCHAR *pszTitle, const CSCHAR *pszText, const CSCHAR *pszNotificationId, const CSCHAR *pszIconPath) {
        if(!Toast::Initialized) return;

        [NSBundle mainBundle];
        NSUserNotificationCenter* notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];

        NSUserNotification* cocoaNotification = [[NSUserNotification alloc] init];
        cocoaNotification.identifier = pszNotificationId != NULL ? [[NSString alloc] initWithCString: pszNotificationId encoding: NSUTF8StringEncoding] : @"";;
        cocoaNotification.title = pszTitle != NULL ? [[NSString alloc] initWithCString: pszTitle encoding: NSUTF8StringEncoding] : @"";
        cocoaNotification.informativeText = pszText != NULL ? [[NSString alloc] initWithCString: pszText encoding: NSUTF8StringEncoding] : @"";
        if(pszIconPath != NULL)
        {
            NSString* imagePath = [[NSString alloc] initWithCString: pszIconPath encoding: NSUTF8StringEncoding];
            NSImage* image = [[NSImage alloc]initWithContentsOfFile: imagePath];
            cocoaNotification.contentImage = image;
        }

        [notificationCenter deliverNotification:cocoaNotification];
    }

    void Toast::SetSelectedCallback(NotificationSelectedEventCallback callback)
    {
        Toast::NotificationSelectedCallback = callback;
    }

    void Toast::HandleCallback(C_NSUserNotification *pUserNotification)
    {
        NSUserNotification* cocoaNotification = (__bridge NSUserNotification*)pUserNotification;
        const CSCHAR* identifier = [[cocoaNotification identifier] cStringUsingEncoding:NSUTF8StringEncoding];
        
        if (Toast::NotificationSelectedCallback != NULL)
            Toast::NotificationSelectedCallback(identifier);
    }

    void Toast::UnInitialize()
    {
        if(Toast::Initialized)
        {
            NSUserNotificationCenter* notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];
            notificationCenter.delegate = NULL;
            Toast::Initialized = false;
            Toast::NotificationSelectedCallback = NULL;
        }
    }
}
