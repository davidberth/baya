cmake_minimum_required (VERSION 3.30)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
project (baya LANGUAGES CXX)

# Correct placement and setting for static runtime library
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(LIBS_DIR "c:/db/libs")
# Define the paths to the debug and release versions of the libraries
set(BGFX_DEBUG_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bgfxDebug.lib")
set(BGFX_RELEASE_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bgfxRelease.lib")
set(BX_DEBUG_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bxDebug.lib")
set(BX_RELEASE_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bxRelease.lib")
set(BIMG_DEBUG_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bimgDebug.lib")
set(BIMG_RELEASE_LIB "${LIBS_DIR}/bgfx/.build/win64_vs2022/bin/bimgRelease.lib")
set(GLFW_DEBUG_LIB "${LIBS_DIR}/glfw/build/src/Debug/glfw3.lib")
set(GLFW_RELEASE_LIB "${LIBS_DIR}/glfw/build/src/Release/glfw3.lib")

# Add source to this project's executable.
add_executable (baya "baya.cpp")

target_sources(baya PRIVATE FILE_SET CXX_MODULES FILES "window.ixx")

target_include_directories(baya PRIVATE 
			"../libs/bx/include"
			"../libs/bgfx/include"	
			"../libs/bgimg/include"
			"../libs/glfw/include"
)
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
	target_compile_options(baya PUBLIC "/Zc:__cplusplus")
	target_compile_options(baya PUBLIC "/Zc:preprocessor")
endif()

# Link the appropriate libraries based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(baya PRIVATE ${BGFX_DEBUG_LIB} ${BX_DEBUG_LIB} ${BIMG_DEBUG_LIB} ${GLFW_DEBUG_LIB})
	target_compile_definitions(baya PRIVATE BX_CONFIG_DEBUG=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(baya PRIVATE ${BGFX_RELEASE_LIB} ${BX_RELEASE_LIB} ${BIMG_RELEASE_LIB} ${GLFW_RELEASE_LIB})
	target_compile_definitions(baya PRIVATE BX_CONFIG_DEBUG=0)
endif()


set_property(TARGET baya PROPERTY CXX_STANDARD 23)

# Define the shader compiler executable
set(SHADER_COMPILER ${CMAKE_SOURCE_DIR}/../libs/bgfx/.build/win64_vs2022/bin/shadercRelease.exe)
set(SHADER_PROFILE spirv)

# Define the output directories for debug and release builds
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)

# Find all vertex and fragment shaders
file(GLOB VERTEX_SHADERS ${CMAKE_SOURCE_DIR}/shaders/v_*.sc)
file(GLOB FRAGMENT_SHADERS ${CMAKE_SOURCE_DIR}/shaders/f_*.sc)

# Function to add custom commands for shader compilation
function(add_shader_compile_commands SHADER_TYPE SHADER_FILES)
    foreach(SHADER_FILE ${SHADER_FILES})
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
        add_custom_command(
            OUTPUT ${OUTPUT_DIR}/${SHADER_NAME}.bin
            COMMAND ${SHADER_COMPILER} -f ${SHADER_FILE} -o ${OUTPUT_DIR}/${SHADER_NAME}.bin --platform windows --type ${SHADER_TYPE} -i ${CMAKE_SOURCE_DIR}/../libs/bgfx/src -p ${SHADER_PROFILE}
            DEPENDS ${SHADER_FILE}
            COMMENT "Compiling ${SHADER_TYPE} shader: ${SHADER_NAME}"
        )
        list(APPEND SHADER_BINARIES ${OUTPUT_DIR}/${SHADER_NAME}.bin)
    endforeach()
    set(SHADER_BINARIES ${SHADER_BINARIES} PARENT_SCOPE)
endfunction()

# Add custom commands for vertex and fragment shaders
add_shader_compile_commands(vertex "${VERTEX_SHADERS}")
add_shader_compile_commands(fragment "${FRAGMENT_SHADERS}")

# Add custom target to ensure shaders are built as part of the build process
add_custom_target(compile_shaders ALL DEPENDS ${SHADER_BINARIES})