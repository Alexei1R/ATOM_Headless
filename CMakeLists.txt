cmake_minimum_required(VERSION 3.2.0)
project(ATOM VERSION 0.1.0 LANGUAGES C CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(libs)
add_subdirectory(app)
