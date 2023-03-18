#ifndef INPUT_H
#define INPUT_H

#include "GUI.h"
#include "SceneManager.h"

class Input : public irr::IEventReceiver {

private: 

	bool key_down[irr::KEY_KEY_CODES_COUNT];
	bool key_up[irr::KEY_KEY_CODES_COUNT];	
	virtual bool isKeyDown(irr::EKEY_CODE) const;
	virtual bool isKeyUp(irr::EKEY_CODE) const;
	virtual bool OnEvent(const irr::SEvent&);

	irr::IrrlichtDevice* device;
	GUI* gui;
	SceneManager* scene_manager;

	bool game_running_flag;

public:

	Input(irr::IrrlichtDevice*, GUI*, SceneManager*);
	~Input();

	void on_player_movement();
	void on_player_stop_movement();
	void on_interaction();
	void on_keyboard();
	void on_play();
	void on_pause();
	void on_resume();
	void on_quit();
	void on_menu();
	void on_hover();
	void on_unhover();
	void on_game_won();
	void on_game_over();	
	void update();
};
#endif // !INPUT_H