project(cosets)

add_executable(${PROJECT_NAME}
        src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glad glm glfw)

add_custom_command(
        TARGET cosets POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_executable(coxeter scr/coxeter.cpp)
add_executable(mirror src/mirrors.cpp)
target_link_libraries(mirror PRIVATE glm)
