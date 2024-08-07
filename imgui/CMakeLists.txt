set(LIBS_DIR "c:/db/libs")

# Add source to this project's executable.
add_library(imgui "imguio.cpp" "imgui.cpp" "imgui_draw.cpp" "imgui_widgets.cpp" "imgui_tables.cpp")

target_include_directories(imgui PRIVATE 
			"${LIBS_DIR}/bx/include"
			"${LIBS_DIR}/bgfx/include"	
			"${LIBS_DIR}/bgfx/examples/common/imgui"
			"${LIBS_DIR}/bgfx/3rdparty"
			"${LIBS_DIR}/bgfx/3rdparty/dear-imgui"
			"${LIBS_DIR}/bx/include/compat/msvc"
			"${LIBS_DIR}/bimg/include"
)
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
	target_compile_options(imgui PUBLIC "/Zc:__cplusplus")
	target_compile_options(imgui PUBLIC "/Zc:preprocessor")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(imgui PRIVATE BX_CONFIG_DEBUG=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions(imgui PRIVATE BX_CONFIG_DEBUG=0)
endif()

