project(cosets)

add_custom_target(shaders ALL DEPENDS shader_output)
add_custom_command(
        OUTPUT shader_output
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "copying shaders"
)

add_executable(${PROJECT_NAME}
        src/main.cpp)
add_dependencies(cosets shaders)
target_link_libraries(${PROJECT_NAME}
        PRIVATE glad glm glfw)

add_executable(coxeter src/coxeter.cpp)

add_executable(mirror src/mirrors.cpp)
target_link_libraries(mirror PRIVATE glm)

add_executable(mesh src/mesh.cpp)
target_link_libraries(mesh PRIVATE glm)
