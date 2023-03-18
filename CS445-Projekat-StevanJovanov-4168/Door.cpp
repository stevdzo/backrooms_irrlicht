#include "Door.h"

Door::Door(irr::IrrlichtDevice* _device, 
		   const irr::c8* _name) : GameObject(
								   _device,
								   nullptr)
{
	object_node = device->getSceneManager()->getSceneNodeFromName(_name);
	object_node->setName(_name);
	flag_unlocked = false;
	add_texture("resources/objects/door/door.jpg");
}

Door::~Door() {

	printf("Deleting door.");
}

bool Door::is_unlocked() { 

	return flag_unlocked; 
}

void Door::unlock() {

	dynamic_cast<irr::scene::IMeshSceneNode*>(object_node)->setMesh(device->getSceneManager()->getMesh("resources/objects/door/door_open.obj"));
	flag_unlocked = true;
	add_texture("resources/objects/door/door.jpg");
}