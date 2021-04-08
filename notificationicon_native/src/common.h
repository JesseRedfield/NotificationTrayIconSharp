#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdint.h>
#include <stdlib.h>

//referenced pointer safe release macros
#define SAFE_DELETE(pPtr) \
    {                     \
        if (pPtr != NULL) \
            delete pPtr;  \
        pPtr = NULL;      \
    }

#ifdef OS_WINDOWS
#define EXPORT __declspec(dllexport)
// CSharp Character Declaration, for windows we send LPWStr to avoid marshalling UTF-16
#define CSCHAR wchar_t
#define CSSTRLEN wcslen
#define CSSTRCPY(dst, src, size) wcsncpy_s(dst, size, src, size)
#else
#define EXPORT
#define __stdcll
// CSharp Character Declaration, for POSIX we send LPStr to avoid marshalling UTF-8
#define CSCHAR char
#define CSSTRLEN strlen
#define CSSTRCPY(dst, src, size) strcpy(dst, src)

#define OBJC_SAFE_RELEASE(pPtr)         \
    {                                   \
        if (pPtr != NULL)               \
            CFRelease((CFTypeRef)pPtr); \
    }
#endif

namespace notification_tray_icon
{
    typedef void(__stdcall *MenuItemSelectedEventCallback)(void *pMenuItem);
}

#endif // __COMMON_H__
