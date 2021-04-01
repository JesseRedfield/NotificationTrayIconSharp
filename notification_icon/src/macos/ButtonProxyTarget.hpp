#ifndef ButtonProxyTarget_h
#define ButtonProxyTarget_h
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

class IButtonProxyTarget
{
public:
    virtual void OnSelected(void) = 0;
};

#endif /* ButtonProxyTarget_h */
