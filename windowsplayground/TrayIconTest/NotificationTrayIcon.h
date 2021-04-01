#ifndef __NOTIFICATION_TRAY_ICON_API_H__
#define __NOTIFICATION_TRAY_ICON_API_H__

#include "common.h"
#include <stdlib.h>

#ifdef OS_WINDOWS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    namespace notification_tray_icon
    {

    }
#ifdef __cplusplus
}
#endif

#endif // __NOTIFICATION_TRAY_ICON_API_H__