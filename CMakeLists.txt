# CMakeList.txt : CMake project for maya, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(LIBS_DIR "c:/db/libs")

# Define the paths to the debug and release versions of the libraries
set(BGFX_DEBUG_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bgfxDebug.lib")
set(BGFX_RELEASE_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bgfxRelease.lib")
set(GLFW_DEBUG_LIB "${LIBS_DIR}/glfw/build/src/Debug/glfw3.lib")
set(GLFW_RELEASE_LIB "${LIBS_DIR}/glfw/build/src/Release/glfw3.lib")


project (maya)


# Add source to this project's executable.
add_executable (maya "maya.cpp")
target_include_directories(maya PRIVATE 
			"../libs/bx/include"
			"../libs/bgfx/include"	
			"../libs/bgimg/include"
			"../libs/glfw/include"
)
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
	target_compile_options(maya PUBLIC "/Zc:__cplusplus")
	target_compile_options(maya PUBLIC "/Zc:preprocessor")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add the preprocessor definition BX_CONFIG_DEBUG to the target my_target
    target_compile_definitions(maya PRIVATE BX_CONFIG_DEBUG)
endif()

# Link the appropriate libraries based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(maya PRIVATE ${BGFX_DEBUG_LIB} ${GLFW_DEBUG_LIB})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(maya PRIVATE ${BGFX_RELEASE_LIB} ${GLFW_RELEASE_LIB})
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET maya PROPERTY CXX_STANDARD 20)
endif()

