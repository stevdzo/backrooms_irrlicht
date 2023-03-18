#ifndef KEY_H
#define KEY_H

#include "GameObject.h"
#include "Door.h"

class Key : public GameObject {

private:

	irr::scene::ISceneCollisionManager* collision;
	std::vector<Door*> doors;

	bool flag_picked_up;

public:

	Key(irr::IrrlichtDevice*,
		const irr::c8*);
	~Key();

	irr::scene::ISceneCollisionManager* get_collision();

	std::vector<Door*> get_doors();
	void set_doors(std::vector<Door*>);

	bool is_picked_up();
	void is_picked_up(bool);

	void unlock_door(Door*);
};
#endif