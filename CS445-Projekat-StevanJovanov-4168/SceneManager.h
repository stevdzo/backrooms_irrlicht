#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Globals.h"
#include "GameController.h"

enum CurrentScene {

	SCENE_1 = 0 << 0,
	SCENE_2 = 1 << 0
};

class SceneManager {

private:

	irr::IrrlichtDevice* device;
	GameController* controller;
	irrklang::ISoundEngine* sound_engine;

	CurrentScene current_scene;

	bool lose_flag;
	bool win_flag;

public:

	SceneManager(irr::IrrlichtDevice*);
	~SceneManager();

	GameController* get_controller();
	irrklang::ISoundEngine* get_sound_engine();

	void set_current_scene(CurrentScene);
	CurrentScene get_current_scene();

	bool is_game_over();
	void is_game_over(bool);

	bool is_game_won();
	void is_game_won(bool);

	void clear_camera();
	void add_last_camera();
	void clear_current_scene();
	void load_scene();
	void update();
};
#endif // !SCENEMANAGER_H