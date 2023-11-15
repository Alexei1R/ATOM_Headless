cmake_minimum_required(VERSION 3.0)
project(ATOM)




set(SOURCES
        main.cpp
        ATOM/Application.cpp
        ATOM/Application.h
        ATOM/atompch.h
        ATOM/macros.h
        ATOM/Core/Logging/Logging.cpp
        ATOM/Core/Logging/Logging.h
        Networking/Client.cpp
        Networking/Client.h




        ATOM/Communication/Serial/impl/list_ports/lists_ports_osx.cpp

)

set(serial_SRCS
        ATOM/Communication/Serial/Serial.cpp
        ATOM/Communication/Serial/Serial.h
        ATOM/Communication/Serial/impl/v8stdint.h
)
if(APPLE)
 # If OSX
 list(APPEND
         ATOM/Communication/Serial/impl/unix.cpp
         ATOM/Communication/Serial/impl/unix.h)

 list(APPEND serial_SRCS
         ATOM/Communication/Serial/impl/list_ports/lists_ports_osx.cpp)
elseif(UNIX)
 # If unix
 list(APPEND serial_SRCS
         ATOM/Communication/Serial/impl/unix.cpp
         ATOM/Communication/Serial/impl/unix.h)

 list(APPEND serial_SRCS
         ATOM/Communication/Serial/impl/list_ports/lists_ports_linux.cpp)
else()
 # If windows
 list(APPEND serial_SRCS
         ATOM/Communication/Serial/impl/win.cpp
         ATOM/Communication/Serial/impl/win.h)
 list(APPEND serial_SRCS
         ATOM/Communication/Serial/impl/list_ports/lists_ports_win.cpp)
endif()

add_executable(executable_headless ${SOURCES} ${serial_SRCS})

target_link_libraries(executable_headless
    glm
    spdlog
    GameNetworkingSockets
 )



target_include_directories(executable_headless PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)