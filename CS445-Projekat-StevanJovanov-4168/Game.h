#ifndef GAME_H
#define GAME_H

#include "GUI.h"
#include "Input.h"
#include "SceneManager.h"

class Game { 

private:

	irr::IrrlichtDevice* device;

	GUI* gui;
	SceneManager* scene_manager;
	Input* input;

public:

	Game();
	~Game();

	void initialize_gui();
	void initialize_scene();
	void initialize_input();
	void initialization();
	void update();
	void run();
};
#endif // !GAME_H