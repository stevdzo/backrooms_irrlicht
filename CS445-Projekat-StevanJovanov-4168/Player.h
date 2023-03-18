#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject {

private:

	irr::scene::ICameraSceneNode* camera;
	irr::scene::ICameraSceneNode* camera_copy;
	irrklang::ISoundEngine* sound_engine;
	irrklang::ISound* footsteps;

public:

	Player(irr::IrrlichtDevice*,
		   irrklang::ISoundEngine*);
	~Player();
	
	irr::scene::ICameraSceneNode* get_camera();
	irr::scene::ICameraSceneNode* get_camera_copy();
	void set_camera(irr::scene::ICameraSceneNode*);
	void set_camera_copy(irr::scene::ICameraSceneNode*);

	void add_camera(irr::core::vector3df =
		            irr::core::vector3df(0.0F, 0.0F, 0.0F), 
					irr::core::vector3df = 
					irr::core::vector3df(0.0F, 0.0F, 0.0F));

	void add_collision();

	void play_footsteps();
	void pause_footsteps();
};
#endif // !PLAYER_H