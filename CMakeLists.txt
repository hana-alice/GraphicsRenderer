cmake_minimum_required(VERSION 3.0.0)
project(GraphicsRenderer VERSION 0.1.0)

include(CTest)
enable_testing()
include_directories(
    "${PROJECT_SOURCE_DIR}/dependencies/include"
)
link_directories(${PROJECT_SOURCE_DIR}/dependencies/lib)
link_libraries(glfw3 glfw3dll)

aux_source_directory(${PROJECT_SOURCE_DIR}/dependencies/impl/ DIR_LIB_SRCS)
add_library (glad ${DIR_LIB_SRCS})


add_executable(GraphicsRenderer source/graphicsrenderer.cpp)

target_link_libraries(GraphicsRenderer glad)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
