#import "ButtonProxyTarget.hpp"

#ifndef ButonProxy_h
#define ButonProxy_h

@interface ButtonProxy : NSObject
{
    IButtonProxyTarget *mpOwner;
}

- (id)initWithOwner:(IButtonProxyTarget *)pOwner;

- (IBAction)leftClick:(id)sender;

@end

#endif /* ButonProxy_h */
