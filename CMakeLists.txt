cmake_minimum_required(VERSION 3.10)
project(GrandBedroomScene CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------
# Dependencies
#   - GLFW  : windowing + input        (apt: libglfw3-dev, vcpkg: glfw3, brew: glfw)
#   - GLM   : math library, header-only (apt: libglm-dev,  vcpkg: glm,   brew: glm)
#   - GLAD  : OpenGL 3.3 core loader   (vendored in third_party/glad, no install needed)
# ---------------------------------------------------------------------
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# GLM is header-only; try find_package first, fall back to the system include path.
find_package(glm QUIET)

add_executable(GrandBedroomScene
    main.cpp
    third_party/glad/src/glad.c
)

target_include_directories(GrandBedroomScene PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
)

target_link_libraries(GrandBedroomScene PRIVATE glfw OpenGL::GL)

if(UNIX AND NOT APPLE)
    target_link_libraries(GrandBedroomScene PRIVATE dl pthread)
endif()

if(glm_FOUND)
    target_link_libraries(GrandBedroomScene PRIVATE glm::glm)
endif()

# Copy every shader (.vs/.fs) next to the built executable so
# Shader("vertexShader.vs", ...) can find them at runtime with a relative path.
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/*.vs" "${CMAKE_SOURCE_DIR}/*.fs")
foreach(SHADER ${SHADER_FILES})
    add_custom_command(TARGET GrandBedroomScene POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER} $<TARGET_FILE_DIR:GrandBedroomScene>)
endforeach()
