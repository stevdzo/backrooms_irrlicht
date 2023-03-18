#include "Smiler.h"

Smiler::Smiler(irr::IrrlichtDevice* _device,
			   irr::scene::ISceneNode* _node,
			   irr::core::vector3df _position) : GameObject(
												 _device,
												 _node), 
											     position(_position)
{
	object_node->setScale(irr::core::vector3df(1.0F, 1.0F, 0.001F));
	object_node->setPosition(position);

	add_texture("resources/enemies/smiler.png", irr::video::EMT_TRANSPARENT_ADD_COLOR);
}

Smiler::~Smiler() {

	printf("Deleting smiler.");
}