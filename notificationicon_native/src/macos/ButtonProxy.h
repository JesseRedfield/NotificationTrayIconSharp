#ifndef ButonProxy_h
#define ButonProxy_h
#import "ButtonProxyTarget.h"

#define OBJC_INCLUDE_BUTTONPROXY(class_name)           \
    @interface class_name : NSObject                   \
    {                                                  \
        IButtonProxyTarget *mpOwner;                   \
    }                                                  \
                                                       \
    -(id)initWithOwner : (IButtonProxyTarget *)pOwner; \
                                                       \
    -(IBAction)leftClick : (id)sender;                 \
                                                       \
    @end                                               \
                                                       \
    @implementation class_name                         \
                                                       \
    -(id)initWithOwner : (IButtonProxyTarget *)pOwner  \
    {                                                  \
        self = [super init];                           \
        mpOwner = pOwner;                              \
        return self;                                   \
    }                                                  \
                                                       \
    -(IBAction)leftClick : (id)sender                  \
    {                                                  \
        mpOwner->OnSelected();                         \
    }                                                  \
                                                       \
    @end
#endif /* ButonProxy_h */
