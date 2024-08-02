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
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(baya WIN32 "main.cpp")
    target_compile_definitions(baya PRIVATE ENTRY_POINT=WinMain)
else()
    add_executable(baya "main.cpp" )
    target_compile_definitions(baya PRIVATE ENTRY_POINT=main)
endif()

set_target_properties(baya PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${TargetName}/"
    
)

target_sources(baya PRIVATE FILE_SET CXX_MODULES FILES "baya.ixx" "window.ixx" "camera.ixx")

target_include_directories(baya PRIVATE 
			"${LIBS_DIR}/bx/include"
			"${LIBS_DIR}/bgfx/include"	
			"${LIBS_DIR}/bgimg/include"
			"${LIBS_DIR}/glfw/include"
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