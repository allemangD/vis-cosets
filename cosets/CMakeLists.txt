project(cosets)

add_executable(${PROJECT_NAME}
        src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glad glm glfw)

target_include_directories(${PROJECT_NAME}
        PRIVATE include)

add_custom_command(
    TARGET cosets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
            ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_executable(coxeter src/tc.cpp)
