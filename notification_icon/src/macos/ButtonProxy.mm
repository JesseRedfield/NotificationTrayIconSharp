#import "ButtonProxy.hpp"

@implementation ButtonProxy

- (id)initWithOwner:(IButtonProxyTarget*)pOwner
{
    self = [super init];
    mpOwner = pOwner;
    return self;
}

- (IBAction)leftClick:(id)sender
{
    mpOwner->OnSelected();
}

@end
