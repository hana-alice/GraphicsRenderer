cmake_minimum_required(VERSION 3.0.0)
project(GraphicsRenderer VERSION 0.1.0)

include(CTest)
enable_testing()
include_directories(
    "${PROJECT_SOURCE_DIR}/dependencies/include"
    "${PROJECT_SOURCE_DIR}/Header/base"
    "${PROJECT_SOURCE_DIR}/Header/object"
)
link_directories(${PROJECT_SOURCE_DIR}/dependencies/lib)
link_libraries(glfw3 glfw3dll)

add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/impl/)
add_subdirectory(${PROJECT_SOURCE_DIR}/source/base)
add_subdirectory(${PROJECT_SOURCE_DIR}/source/object)

add_executable(GraphicsRenderer source/graphicsrenderer.cpp)

target_link_libraries(GraphicsRenderer 
    base
    object
    glad 
    glfw3
    glfw3dll
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
