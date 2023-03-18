#ifndef GUI_H
#define GUI_H

#include "Globals.h"

enum ButtonID {

	BTN_PLAY = 0 << 0,
	BTN_QUIT = 1 << 0,
	BTN_MENU = 1 << 1,
	BTN_RESUME = 1 << 2
};

class GUI {

private:

	irr::IrrlichtDevice* device;
	irr::gui::IGUIFont* font;
	std::vector<irr::gui::IGUIImage*> collected_papers;
	std::vector<const irr::c8*> collected_papers_id;
	std::vector<irr::gui::IGUIButton*> buttons;
	std::vector<irr::video::ITexture*> button_hover_textures;
	std::vector<irr::video::ITexture*> button_unhover_textures;
	irr::gui::IGUIElement* current_hovered_button;

	irr::f32 delta_time;
	irr::u32 previous_time;

	irr::u32 elapsed_time;
	irr::f32 hint_time;
	irr::u32 hint_index;
	irr::ITimer* timer;
	irr::core::stringw ui_timer;
	
	void set_crosshair();

	bool interaction_text_flag;
	bool objective_text_scene_1_flag;
	bool objective_text_scene_2_flag;
	bool hint_scene_1_flag;
	bool hint_scene_2_flag;

public:

	GUI(irr::IrrlichtDevice*);
	~GUI();

	void set_collected_papers_id(std::vector<const irr::c8*>);

	void show_menu();
	void show_game_won_screen();
	void show_game_over_screen();
	void show_pause_screen();
	void show_hud();
	void show_interaction_text();
	void show_objective_text_scene_1();
	void show_objective_text_scene_2();
	void show_hint_scene_1();
	void show_hint_scene_2();
	void show_timer();
	void show_total_time();

	void add_custom_button(const irr::c8*,
						   const irr::io::path,
						   const irr::io::path,
						   ButtonID,
						   irr::f32=0.0f,
						   irr::f32=0.0f);

	void button_hover();
	void button_unhover();
	void clear_vectors();
	void update();
	void reset_timer();
	void pause_timer();
	void resume_timer();

	void can_interact(bool);
	void can_show_objective_1(bool);
	void can_show_objective_2(bool);
	void can_show_hint_scene_1(bool);
	void can_show_hint_scene_2(bool);
};
#endif