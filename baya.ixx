module;

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <GLFW/glfw3.h>

export module baya;
import window;

struct PosColorVertex {
	// 3d space position
	float m_x;
	float m_y;
	float m_z;
	// Color value
	uint32_t m_abgr;

	static void init() {
		// start the attribute declaration
		ms_decl
			.begin()
			// Has three float values that denote position
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			// and a uint8 color value that denotes color
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosColorVertex::ms_decl;

PosColorVertex s_cubeVertices[] =
{
	{  0.5f,  0.5f, 0.0f, 0xff0000ff },
	{  0.5f, -0.5f, 0.0f, 0xff0000ff },
	{ -0.5f, -0.5f, 0.0f, 0xff00ff00 },
	{ -0.5f,  0.5f, 0.0f, 0xff00ff00 }
};

const uint16_t s_cubeTriList[] =
{
	0,1,3,
	1,2,3
};



bgfx::ShaderHandle loadShader(const char* _name) {
	std::ifstream file(_name, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << _name << std::endl;
		return BGFX_INVALID_HANDLE;
	}

	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> data(fileSize);
	if (!file.read(data.data(), fileSize)) {
		std::cerr << "Failed to read shader file: " << _name << std::endl;
		return BGFX_INVALID_HANDLE;
	}

	const bgfx::Memory* mem = bgfx::copy(data.data(), static_cast<uint32_t>(fileSize));
	bgfx::ShaderHandle handle = bgfx::createShader(mem);
	bgfx::setName(handle, _name);

	return handle;
}

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program; // we create a program handle
bgfx::ShaderHandle vsh;
bgfx::ShaderHandle fsh;

// Define the entry point based on the build type
#ifdef ENTRY_POINT
#define MAIN_ENTRY ENTRY_POINT
#else
#define MAIN_ENTRY main
#endif


export int init_game()
{

	if (!init_window())
		return 1;

	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;

	init.platformData.nwh = get_native_window();


	int width, height;
	get_window_size(width, height);

	init.resolution.width = (uint32_t)width;
	init.resolution.height = (uint32_t)height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	init.type = bgfx::RendererType::Vulkan;
	if (!bgfx::init(init))
		return 1;


	PosColorVertex::init();
	m_vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
		PosColorVertex::ms_decl
	);

	m_ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
	);

	vsh = loadShader("shaders\\v_simple.bin");
	fsh = loadShader("shaders\\f_simple.bin");

	m_program = bgfx::createProgram(vsh, fsh, true);
	return 0;
}

export void main_loop()
{
	int width, height;
	get_window_size(width, height);

	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

	bgfx::touch(0);

	while (!window_should_close())
	{
		poll_events();

		// Handle window resize.
		int oldWidth = width, oldHeight = height;
		get_window_size(width, height);

		if (width != oldWidth || height != oldHeight) {
			bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
		}

		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };

		// Set view and projection matrix for view 0.
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj,
			60.0f,
			float(width) / float(height),
			0.1f, 100.0f,
			bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0,
			width,
			height);

		bgfx::touch(kClearView);
		float mtx[16];
		bx::mtxRotateY(mtx, 0.0f);

		// position x,y,z
		mtx[12] = 0.0f;
		mtx[13] = 0.0f;
		mtx[14] = 0.0f;

		// Set model matrix for rendering.
		bgfx::setTransform(mtx);

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setIndexBuffer(m_ibh);

		// Set render states.
		bgfx::setState(BGFX_STATE_DEFAULT);

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, m_program);

		bgfx::frame();
	}
	return;
}

export void cleanup()
{
	bgfx::shutdown();
	cleanup_window();
	
	return;
}