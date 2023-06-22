cmake_minimum_required(VERSION 3.1)

project(GLFW-CMake-starter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(glad/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finput-charset=UTF-8")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8")

add_subdirectory(glfw)

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

set(GLFW-CMAKE-STARTER-SRC
        main.cpp
        glad/src/glad.c
        )

add_executable(GLFW-CMake-starter WIN32 ${GLFW-CMAKE-STARTER-SRC})

target_link_libraries(GLFW-CMake-starter ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})

if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter)
    endif()
endif()