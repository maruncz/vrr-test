cmake_minimum_required(VERSION 3.10.0)
project(vrr-test VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GLEW)

if(NOT ${GLEW_FOUND})
    message(CRITICAL "GLEW not found")
endif()

find_package(glfw3 REQUIRED)

find_package(glm REQUIRED)

find_package(OpenGL COMPONENTS OpenGL REQUIRED)




add_subdirectory(src)




include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
