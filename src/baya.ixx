module;

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "imgui.h"

#include <GLFW/glfw3.h>

export module baya;
import window;
import camera;


bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program; // we create a program handle
bgfx::ShaderHandle vsh;
bgfx::ShaderHandle fsh;


ImFont* font1; 

Camera camera;

struct PosColorVertex {
	// 3d space position
	float m_x;
	float m_y;
	float m_z;
	// color value
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
	{  0.5f,  0.5f, 0.0f, 0xffffff00 },
	{  0.5f, -0.5f, 0.0f, 0xffffff00 },
	{ -0.5f, -0.5f, 0.0f, 0xffffff11 },
	{ -0.5f,  0.5f, 0.0f, 0xffffff11 }
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

	camera.init();
	camera.set_window_size(width, height);
	camera.set_height(3.0f);

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
	imguiCreate(20.0f);

	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.8583691120147705f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.6995707750320435f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.6470588445663452f, 0.2313725501298904f, 0.2313725501298904f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.501960813999176f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.6695278882980347f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9570815563201904f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1019607856869698f, 0.1137254908680916f, 0.1294117718935013f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2039215713739395f, 0.2078431397676468f, 0.2156862765550613f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 0.2000000029802322f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2352941185235977f, 0.2470588237047195f, 0.2705882489681244f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.2588235437870026f, 0.2745098173618317f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 0.9725490212440491f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4274509847164154f, 0.3607843220233917f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 0.8784313797950745f, 0.8784313797950745f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.2705882489681244f, 0.3803921639919281f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1803921610116959f, 0.2274509817361832f, 0.2784313857555389f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);

	



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
	ImGui::GetStyle().ScaleAllSizes(1.0f);

	double mx, my;



	/*
	uint8_t* data;
	int32_t awidth;
	int32_t aheight;
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&data, &awidth, &aheight);
	auto tex = bgfx::createTexture2D(
		(uint16_t)width
		, (uint16_t)height
		, false
		, 1
		, bgfx::TextureFormat::BGRA8
		, BGFX_STATE_BLEND_SRC_ALPHA | BGFX_STATE_BLEND_INV_SRC_ALPHA
		, bgfx::copy(data, width * height * 4)
	);
	io.Fonts->SetTexID((void*)tex.idx);
	*/

	while (!window_should_close())
	{
		poll_events();



		

		// Handle window resize.
		int oldWidth = width, oldHeight = height;
		get_window_size(width, height);

		if (width != oldWidth || height != oldHeight) {
			bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
			camera.set_window_size(width, height);
		}

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0,
			width,
			height);

		camera.apply();

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


		//int32_t _mx, int32_t _my, uint8_t _button, int32_t _scroll, uint16_t _width, uint16_t _height, int _inputChar, bgfx::ViewId _viewId
		get_mouse_pos(mx, my);
		uint8_t button_state = get_mouse_button_state();
		imguiBeginFrame(mx, my, button_state, 0, width, height, 0, 0);


		ImGui::Begin("My DearImGui Window");
		for (int i = 0; i < 10; i++) ImGui::Text("hello, world");
		ImGui::End();

		ImGui::Render();
		imguiEndFrame();

		bgfx::frame();

	


	}
	return;
}

export void cleanup()
{
	bgfx::destroy(m_vbh);
	bgfx::destroy(m_ibh);
	bgfx::destroy(m_program);
	bgfx::destroy(vsh);
	bgfx::destroy(fsh);

	imguiDestroy();

	bgfx::shutdown();

	cleanup_window();
	
	return;
}