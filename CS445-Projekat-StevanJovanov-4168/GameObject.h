#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"

class GameObject {

public:

	GameObject(irr::IrrlichtDevice*, 
			   irr::scene::ISceneNode*);
	~GameObject();
	
	irr::scene::ISceneNode* get_object_node();
	void destroy();

protected:

	irr::IrrlichtDevice* device;
	irr::scene::ISceneNode* object_node;

	void add_texture(const irr::io::path _path, 
					 irr::video::E_MATERIAL_TYPE=
				     irr::video::EMT_SOLID);

};
#endif // !GAMEOJECT_H