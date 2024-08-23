module;

#include <bgfx/bgfx.h>

export module vertex;

export struct Vertex {
	// 2d space position
	float x;
	float y;
	// color value
	uint8_t hue;
	uint8_t saturation;
	uint8_t value;
	uint8_t alpha;
	

	static void init() {
		// start the attribute declaration
		ms_decl
			.begin()
			// Has two float values that denote position
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			// and 4 uint8 value that denote HSV color + alpha
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_decl;
};

export bgfx::VertexLayout Vertex::ms_decl;

export void init_vertex_layout() {
	Vertex::init();
}


