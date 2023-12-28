project(ATOM)


find_package(OpenCV REQUIRED )



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
        Networking/Server.h
        Networking/Server.cpp
        Networking/Networking.h
        Networking/Networking.cpp
        ATOM/Core/Layers/Layer.cpp
        ATOM/Core/Layers/Layer.h
        ATOM/Core/Layers/LayerStack.cpp
        ATOM/Core/Layers/LayerStack.h
        ATOM/Application.cpp


)

set(serial_SRCS
        Communication/Serial/Serial.cpp
        Communication/Serial/Serial.h
        Communication/Serial/impl/v8stdint.h
        ATOM/EmbededPlatform/SerialCommunication.cpp
        ATOM/EmbededPlatform/SerialCommunication.h
        ATOM/Core/VideoServer/Frame.cpp
        ATOM/Core/VideoServer/Frame.h
        ATOM/Core/ServerLayer.cpp
        ATOM/Core/ServerLayer.h
)
if(APPLE)
 # If OSX
 list(APPEND
         Communication/Serial/impl/unix.cpp
         Communication/Serial/impl/unix.h)

 list(APPEND serial_SRCS
         Communication/Serial/impl/list_ports/lists_ports_osx.cpp
 )

elseif(UNIX)
 # If unix
 list(APPEND serial_SRCS
         Communication/Serial/impl/unix.cpp
         Communication/Serial/impl/unix.h)

 list(APPEND serial_SRCS
         Communication/Serial/impl/list_ports/lists_ports_linux.cpp)
else()
 # If windows
 list(APPEND serial_SRCS
         Communication/Serial/impl/win.cpp
         Communication/Serial/impl/win.h)
 list(APPEND serial_SRCS
         Communication/Serial/impl/list_ports/lists_ports_win.cpp)
endif()

add_executable(executable_headless ${SOURCES} ${serial_SRCS})

target_link_libraries(executable_headless
        glm
        spdlog
        GameNetworkingSockets
        ${OpenCV_LIBS}
)



target_include_directories(executable_headless PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)


