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
)



add_executable(executable_headless ${SOURCES})

target_link_libraries(executable_headless
    glm
    spdlog
    GameNetworkingSockets
 )



target_include_directories(executable_headless PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)