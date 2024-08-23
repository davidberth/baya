module;

#include <vector>

export module scene;

import node;
import vertex;
import renderable;
import shader;


uint16_t indices[] =
{
	0,1,3,
	1,2,3
};

Vertex vertices[] =
{
	{  0.5f,  0.5f, 0, 255, 255, 255},
	{  0.5f, -0.5f, 15, 255, 255, 255},
	{ -0.5f, -0.5f, 55, 255, 122, 255},
	{ -0.5f,  0.5f, 100, 255, 125, 255},
};

export class Scene {

	Node* parent;
	Shader* shader;
	
public:
	Scene()
	{
		parent = new Node("root");

		Node* square = parent->add_child("square");


		shader = new Shader();
		shader->load("shaders\\v_simple.bin", "shaders\\f_simple.bin");

		Renderable* rdb = new Renderable("square", vertices, indices, sizeof(vertices), sizeof(indices));
		square->add_renderable(rdb);
		rdb->set_shader(shader);


	}

	~Scene()
	{
		delete parent;
		delete shader;
	}

	void render()
	{
		parent->render();
	}

};