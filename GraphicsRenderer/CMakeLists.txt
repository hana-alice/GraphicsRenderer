PROJECT(main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-lGL -lGLU)
    message(STATUS "optional:-lGL -lGLU")   
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(./)
include_directories(./dependencies/include)
include_directories(./dependencies/include/stbimg)

link_directories(${CMAKE_SOURCE_DIR}/dependencies/linux)
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local/lib /dependencies/linux NO_DEFAULT_PATH)
link_libraries(
    libglfw.so
    libglfw.so.3
    #libglfw.so.3.2
    libglfw3.a
    )

ADD_SUBDIRECTORY(dependencies)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS})
TARGET_LINK_LIBRARIES(main GLAD)