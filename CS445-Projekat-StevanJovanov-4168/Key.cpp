#include "Key.h"

Key::Key(irr::IrrlichtDevice* _device,
	     const irr::c8* _name) : GameObject(
							     _device,
								 nullptr)
{
	object_node = device->getSceneManager()->getSceneNodeFromName(_name);
	object_node->setName(_name);
	collision = device->getSceneManager()->getSceneCollisionManager();
	add_texture("resources/objects/key/key_material.png");
}

Key::~Key() {

	printf("Deleting key.");
}

irr::scene::ISceneCollisionManager* Key::get_collision() { 

	return collision; 
}

std::vector<Door*> Key::get_doors() {

	return doors;
}

void Key::set_doors(std::vector<Door*> _doors) { 

	doors = _doors; 
}

bool Key::is_picked_up() { 

	return flag_picked_up; 
}

void Key::is_picked_up(bool _flag) { 

	flag_picked_up = _flag;
}

void Key::unlock_door(Door* _door) {

	_door->unlock();
	flag_picked_up = false;
}