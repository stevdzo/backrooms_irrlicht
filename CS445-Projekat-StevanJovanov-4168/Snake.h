#ifndef SNAKE_H
#define SNAKE_H

#include "GameObject.h"

class Player;

class Snake : public GameObject {

private:

	irr::scene::IAnimatedMeshSceneNode* animated_mesh;

public:

	Snake(irr::IrrlichtDevice*,
		  const irr::c8*);
	~Snake();

	void add_collision();

	bool chase_player(Player*);
	bool kill_player(Player*);

};
#endif // ! SNAKE_H