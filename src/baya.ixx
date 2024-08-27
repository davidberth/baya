module;

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <GLFW/glfw3.h>

#include "configure.h"


export module baya;

import window;
import camera;
import scene;
import vertex;
import gui;



bool check_state = false;

Camera camera;



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

	std::string renderer = RENDERER;
	if (renderer == "direct3d11")
	{
		init.type = bgfx::RendererType::Direct3D11;
	}
	else if (renderer == "vulkan")
	{
		init.type = bgfx::RendererType::Vulkan;
}
	else
	{
		init.type = bgfx::RendererType::OpenGL;
	}

	if (!bgfx::init(init))
		return 1;

	camera.init();
	camera.set_window_size(width, height);
	camera.set_height(3.0f);



	init_gui(25.0f);

	init_vertex_layout();
	return 0;
}

export void main_loop()
{
	int width, height;
	get_window_size(width, height);

	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

	bgfx::touch(0);

	double mx, my;

	float rot = 0.0f;

	Scene* scene = new Scene();


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

		/*

		float mtx[16];
		bx::mtxRotateZ(mtx, rot);

		// position x,y,z
		mtx[12] = 0.0f;
		mtx[13] = 0.0f;
		mtx[14] = 0.0f;

		rot += 0.01f;
		if (rot > bx::kPi2) rot -= bx::kPi2;

		// Set model matrix for rendering.
		bgfx::setTransform(mtx);

		

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setIndexBuffer(m_ibh);

		// Set render states.
		bgfx::setState(BGFX_STATE_WRITE_RGB 
			| BGFX_STATE_WRITE_A 
			| BGFX_STATE_MSAA);

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, m_program);

		bgfx::setTransform(mtx);

		bgfx::setVertexBuffer(0, m_vbh);

		bgfx::setIndexBuffer(m_ibhl);
		bgfx::setState(BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_PT_LINES
			| BGFX_STATE_MSAA | BGFX_STATE_LINEAA);

		bgfx::submit(0, m_programl);

		*/

		scene->render();
		
		get_mouse_pos(mx, my);
		uint8_t button_state = get_mouse_button_state();

		render_gui(mx, my, button_state, width, height);
		

	}

	delete scene;

	return;
}

export void cleanup()
{

	shutdown_gui();

	bgfx::shutdown();

	cleanup_window();
	
	return;
}