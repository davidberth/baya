#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <iostream>


import window;



int main(int argc, char** argv)
{

	Window window;
	if (!window.init())
		return 1;

	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;

	init.platformData.nwh = window.getNativeWindow();
		

	int width, height;
	window.getWindowSize(width, height);
	std::cout << "width: " << width << " height: " << height << std::endl;
	
	init.resolution.width = (uint32_t)width;
	init.resolution.height = (uint32_t)height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	if (!bgfx::init(init))
		return 1;
	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
	while (!window.shouldClose())
	{
		window.pollEvents();
	
		// Handle window resize.
		int oldWidth = width, oldHeight = height;
		window.getWindowSize(width, height);
	
		if (width != oldWidth || height != oldHeight) {
			std::cout << "resizing" << std::endl;
			bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
			bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		}
		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(kClearView);
	
		bgfx::frame();
	}
	bgfx::shutdown();
	
	
	return 0;
}