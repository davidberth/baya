module;

#include <bgfx/bgfx.h>
#include <string>

export module renderable;

import vertex;
import shader;

export class Renderable {
	

	Vertex* vertices;
	uint16_t* indices;
	uint32_t num_vertices;
	uint32_t num_indices;
	std::string id;
	Shader* shader;

	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;

public:

	Renderable(std::string id, Vertex* vertices, uint16_t* indices, uint32_t num_vertices, uint32_t num_indices)
	{
		this->id = id;
		this->vertices = vertices;
		this->indices = indices;
		this->num_vertices = num_vertices;
		this->num_indices = num_indices;

		
		vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(vertices, sizeof(Vertex) * num_vertices),
			Vertex::ms_decl
		);

		ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(indices, sizeof(uint16_t) * num_indices)
		);

	}

	void render()
	{
		

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, vbh);
		bgfx::setIndexBuffer(ibh);

		// Set render states.
		bgfx::setState(BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_MSAA);

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, shader->program);
		
	}

	~Renderable()
	{
		delete[] vertices;
		delete[] indices;
	}

	void set_shader(Shader* shader)
	{
		this->shader = shader;
	}
};

