//
//  ButonProxy.m
//  TrayIconPlayground
//
//  Created by Jesse Redfield on 3/26/21.
//
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
