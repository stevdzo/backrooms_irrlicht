#ifndef SMILER_H
#define SMILER_H

#include "GameObject.h"

class Smiler : public GameObject {

private:

	irr::core::vector3df position;

public:

	Smiler(irr::IrrlichtDevice*,
		   irr::scene::ISceneNode*,
		   irr::core::vector3df);
	~Smiler();
};
#endif // !SMILER_H