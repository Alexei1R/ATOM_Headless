#sel last version of cmake
cmake_minimum_required(VERSION 3.1)
project(ATOM VERSION 0.1.0 LANGUAGES C CXX)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(libs)
add_subdirectory(app)
