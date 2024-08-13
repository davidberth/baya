module;

#include <bx/bx.h>
#include <bx/math.h>
#include <vector>
#include <string>
#include <iostream>

export module node;

import renderable;

export class Node {
public:


	float local_transform[16] = { 0 };
	float x = 0.0f;
	float y = 0.0f;
	float heading = 0.0f;

	std::vector<Node*> children;
	std::string id;

	Node(std::string id)
	{
		this->id = id;
		std::cout << "Node " << id << " created" << std::endl;
		children.clear();
	}

	~Node()
	{
		for (Node* child : children)
		{
			delete child;
		}
		children.clear();
		std::cout << "Node " << id << " destroyed" << std::endl;
	}

	Node *get_child(int index)
	{
		if (index < children.size())
		{
			return children[index];
		}
		return nullptr;
	}

	void get_transform()
	{
		// This just performs rotation around the Z axis and 2D (x,y) translation
		bx::mtxRotateZ(local_transform, heading);

		local_transform[12] = x;
		local_transform[13] = y;
	}

	Node *add_child(std::string child_id)
	{
		Node* child = new Node(id + '_' + child_id);
		children.push_back(child);
		return child;
	}

};