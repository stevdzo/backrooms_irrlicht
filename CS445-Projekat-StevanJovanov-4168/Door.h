#ifndef DOOR_H
#endif DOOR_H

#include "GameObject.h"

class Door : public GameObject {

private:

	bool flag_unlocked;

public:

	Door(irr::IrrlichtDevice*, const irr::c8*);
	~Door();

	bool is_unlocked();
	void unlock();
};