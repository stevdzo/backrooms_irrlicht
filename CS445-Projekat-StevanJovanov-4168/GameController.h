#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Globals.h"
#include "Player.h"
#include "Smiler.h"
#include "Key.h"
#include "Snake.h"
 
class GameController {

private:

	int PAPER_COUNT = 4;

	irr::IrrlichtDevice* device;
	irrklang::ISoundEngine* sound_engine;
	Player* player;
	Snake* snake;
	std::vector<Smiler*> smilers;
	std::vector<Key*> keys;
	std::vector<Door*> doors;
	irr::scene::ISceneNode* radio;
	irr::scene::ISceneNode* lever1;
	irr::scene::ISceneNode* lever2;
	irr::scene::ILightSceneNode* light_node;
	std::vector<irr::scene::ISceneNode*> interaction_items;
	std::vector<irr::scene::ISceneNode*> papers;
	std::vector<irr::scene::ISceneNode*> collected_papers;
	std::vector<irr::scene::ISceneNode*> bodies;
	irrklang::ISound* footsteps;
	irrklang::ISound* hiss;

	irr::f32 death_timer;
	irr::f32 delta_time;
	irr::u32 previous_time;

	bool timer_flag;
	bool lose_flag;
	bool win_flag;
	bool scene_2_flag;
	bool interaction_text_flag;
	
public:

	GameController(irr::IrrlichtDevice*,
				   irrklang::ISoundEngine*);
	~GameController();

	Player* get_player();
	irr::f32 get_death_timer();
	irr::f32 get_delta_time();
	std::vector<irr::scene::ISceneNode*> get_collected_papers();

	void clear_camera();
	void add_last_camera();
	void add_player(irr::core::vector3df=irr::core::vector3df(260.0F, 150.0F, 30.0F));

	// SCENE1

	void add_smilers();
	void remove_smilers();
	void face_player();
	void kill_player();
	void get_doors_from_scene();
	void get_keys_from_scene();
	void get_objects_from_scene1();
	void get_objects_from_scene2();	
	void get_papers_from_scene();
	void assign_key_to_door();
	void pickup_key();
	void unlock_door();
	void play_radio();
	void play_buzz_sound();
	void play_hallway_sound();
	void pull_lever();
	void flush_key(irr::scene::ISceneNode*);
	void teleport();
	void collect_paper();	
	void show_collected_papers();	

	// SCENE 2

	void get_snake_from_scene();
	void get_bodies_from_scene();
	void get_billboard_from_scene();
	void add_water_surface();
	void snake_chase_player();
	void pull_lever_2();
	void rotate_bodies();
	void play_indoor_sound();
	void check_for_escape();

	void update();
	void restart_game();

	bool can_show_interaction_text();
	bool wrong_door();
	bool game_over();
	bool game_won();
	bool can_load_scene_2();

};
#endif // !GAMECONTROLLER_H