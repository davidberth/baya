﻿cmake_minimum_required (VERSION 3.30)

# cache variables
set(LIBS_DIR "" CACHE PATH "Path to the directory containing the external dependencies")
set(RENDERER "vulkan" CACHE STRING "The backend to use for rendering. Options are Vulkan, DirectX11")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
project (baya LANGUAGES CXX VERSION 0.1.0)

 set(APPNAME ${PROJECT_NAME})
 set(APPVERSION ${CMAKE_PROJECT_VERSION})

# Correct placement and setting for static runtime library
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory(configure)
add_subdirectory(imgui)
add_subdirectory(src)
add_subdirectory(shaders)