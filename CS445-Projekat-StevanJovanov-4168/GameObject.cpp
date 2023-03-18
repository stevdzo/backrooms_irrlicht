#include "GameObject.h"

GameObject::GameObject(irr::IrrlichtDevice* _device, 
					   irr::scene::ISceneNode* _node) : 
														device(_device), 
														object_node(_node) 
{

}

GameObject::~GameObject() {

	printf("Deleting gameobject.");
}

irr::scene::ISceneNode* GameObject::get_object_node() {

	return object_node;
}

void GameObject::add_texture(const irr::io::path _path, irr::video::E_MATERIAL_TYPE _material) {

	object_node->setMaterialTexture(0, device->getVideoDriver()->getTexture(_path));
	object_node->setMaterialType(_material);
	object_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
}

void GameObject::destroy() {

	object_node->remove();
}