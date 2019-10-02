add_custom_target(shaders ALL DEPENDS shader_output)
add_custom_command(
        OUTPUT shader_output
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "copying shaders"
)

add_executable(cosets
        src/main.cpp
        src/util/files.cpp
        src/util/numeric.cpp
        src/util/shader.cpp
        src/util/window.cpp)

target_include_directories(cosets PRIVATE include)

add_dependencies(cosets shaders)

target_link_libraries(cosets
        PRIVATE glad glm glfw)
