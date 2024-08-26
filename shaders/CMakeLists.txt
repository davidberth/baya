# Define the shader compiler executable
set(SHADER_COMPILER ${CMAKE_SOURCE_DIR}/../libs/bgfx/.build/win64_vs2022/bin/shadercRelease.exe)
if (RENDERER STREQUAL "vulkan")
	set(SHADER_PROFILE spirv)
elseif(RENDERER STREQUAL "direct3d11")
	set(SHADER_PROFILE s_5_0)
endif()


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