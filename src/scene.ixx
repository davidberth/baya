module;

#include <vector>

export module scene;

import node;
import renderable;

export class Scene {

	Node* parent;
	
public:
	Scene()
	{
		parent = new Node("root");

		Node* square = parent->add_child("square");
		//Renderable *square_rd = square->add_renderable("square");

	}

	~Scene()
	{
		delete parent;
	}

};