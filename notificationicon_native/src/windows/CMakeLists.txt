cmake_minimum_required (VERSION 3.7)

set(HEADERS ${HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Toast.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TrayIcon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TrayMenuItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DesktopNotificationManagerCompat.h
PARENT_SCOPE)

set(SOURCE ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/Toast.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/TrayIcon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TrayMenuItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DesktopNotificationManagerCompat.cpp
PARENT_SCOPE)
