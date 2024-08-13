module;

#include <vector>

export module scene;

import node;

export class Scene {

	Node* parent;
	
public:
	Scene()
	{
		parent = new Node("root");

		Node* sun = parent->add_child("sun");
		Node* moon = parent->add_child("moon");
		Node* crater = moon->add_child("crater");
		Node* asteroid = parent->add_child("asteroid");
		Node* planet = parent->add_child("planet");
		Node* continent = planet->add_child("continent");
		Node* country = continent->add_child("country");
		Node* city = country->add_child("city");
		Node* building = city->add_child("building");
		Node* room = building->add_child("room");
		Node* table = room->add_child("table");
		Node* chair = room->add_child("chair");
		Node* lamp = room->add_child("lamp");
		Node* book = room->add_child("book");
	}

	~Scene()
	{
		delete parent;
	}

};