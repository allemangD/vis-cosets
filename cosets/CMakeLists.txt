project(cosets)

add_executable(${PROJECT_NAME}
        src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glad glm glfw)

target_include_directories(${PROJECT_NAME}
        PRIVATE include)
