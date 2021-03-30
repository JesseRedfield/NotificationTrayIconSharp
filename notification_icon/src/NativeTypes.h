#ifndef __NATIVE_TYPES__
#define __NATIVE_TYPES__

#ifdef OS_WINDOWS
#import <wchar.h>
    #define EXPORT __declspec(dllexport)
    // CSharp Character Declaration, for windows we send LPWStr to avoid marshalling UTF-16
    #define CSCHAR wchar_t 
    #define CSTRLEN wcslen
    #define CSTRCPY(dst, src, size) wcsncpy(dst, src, size)
#else
    #import <string>
    #define EXPORT
    #define __stdcll
    // CSharp Character Declaration, for POSIX we send LPStr to avoid marshalling UTF-8
    #define CSCHAR char
    #define CSTRLEN strlen
    #define CSTRCPY(dst, src, size) strcpy(dst, src)
#endif

namespace notification_tray_icon
{
    typedef void(__stdcall* MenuItemSelectedEventCallback)(void* pMenuItem);
}

#endif  // __NATIVE_TYPES__
