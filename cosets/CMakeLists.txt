project(cosets)

add_custom_target(shaders ALL DEPENDS shader_output)
add_custom_command(
        OUTPUT shader_output
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "copying shaders"
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/util/coxeter.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

add_dependencies(cosets shaders)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glad glm glfw)
