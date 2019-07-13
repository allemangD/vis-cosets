cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

add_library(glad vendor/glad/src/glad.c cosets/src/main.cpp)
target_include_directories(glad PUBLIC vendor/glad/include)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(vendor/glfw)

option(GLM_TEST_ENABLE OFF)
add_subdirectory(vendor/glm)

set(BOOST_ROOT vendor/boost)
find_package(Boost REQUIRED COMPONENTS format)

if (Boost_FOUND)
    message('boost found!')
else()
    message('boost not found!')
endif ()

add_subdirectory(cosets)
