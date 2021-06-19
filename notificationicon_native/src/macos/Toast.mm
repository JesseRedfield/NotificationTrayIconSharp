#include "Toast.h"
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#import <objc/runtime.h>

#pragma mark - Swizzle NSBundle

NSString *bundleIdentifier = nil; 
bool bInitialized = false;

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

    notification_tray_icon_private::Toast::HandleCallback(notification);
    //TODO HANDLE ACTION [self ActionCallback]([notification.identifier intValue]);
}

@end

#pragma mark -
namespace notification_tray_icon_private
{
    NotificationCenterDelegate *_notificationCenterDelegate;

    void Toast::Initialize(const CSCHAR *pszAppId, const CSCHAR *pszDisplayName, const CSCHAR *pszIconPath)
    {
        @autoreleasepool {
            bInitialized = installNSBundleHook();
            _notificationCenterDelegate = [NotificationCenterDelegate new];

            bundleIdentifier = pszAppId != NULL ? [[NSString alloc] initWithCString: pszAppId encoding: NSUTF8StringEncoding] : @"";

            NSUserNotificationCenter* notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];
            notificationCenter.delegate = _notificationCenterDelegate;
        }
    }

    void Toast::SendNotification(const CSCHAR *pszTitle, const CSCHAR *pszText, const CSCHAR *pszNotificationId, const CSCHAR *pszIconPath) {
        [NSBundle mainBundle];
        NSUserNotificationCenter* notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];

        NSUserNotification* cocoaNotification = [[NSUserNotification alloc] init];
        cocoaNotification.identifier = pszNotificationId != NULL ? [[NSString alloc] initWithCString: pszNotificationId encoding: NSUTF8StringEncoding] : @"";;
        cocoaNotification.title = pszTitle != NULL ? [[NSString alloc] initWithCString: pszTitle encoding: NSUTF8StringEncoding] : @"";
        cocoaNotification.informativeText = pszText != NULL ? [[NSString alloc] initWithCString: pszText encoding: NSUTF8StringEncoding] : @"";
        [notificationCenter deliverNotification:cocoaNotification];
    }

    void Toast::HandleCallback(C_NSUserNotification *pUserNotification)
    {
        //TODO HANDLE CALLBACK
    }
}
