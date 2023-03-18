#include "GUI.h"

GUI::GUI(irr::IrrlichtDevice* _device) : device(_device) {

	font = device->getGUIEnvironment()->getFont("resources/gui/font/lucida.xml");
	timer = device->getTimer();
	interaction_text_flag = false;	
	hint_index = 1;
	hint_time = 0;	
}

GUI::~GUI() {

	printf("Deleting gui.");
}

void GUI::set_collected_papers_id(std::vector<const irr::c8*> _papers_id) {

	collected_papers_id = _papers_id;
}

void GUI::set_crosshair() {

	irr::video::ITexture* crosshair = device->getVideoDriver()->getTexture("resources/gui/crosshair.png");
	device->getGUIEnvironment()->addImage(crosshair, irr::core::position2di(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
}

void GUI::show_menu() {

	device->getCursorControl()->setVisible(true);
	device->getGUIEnvironment()->clear();
	clear_vectors();

	irr::video::ITexture* background = device->getVideoDriver()->getTexture("resources/gui/menu_image.png");
	device->getGUIEnvironment()->addImage(background, irr::core::position2d<irr::s32>(0, 0));	

	add_custom_button("button_play", "resources/gui/buttons/button_play.png", "resources/gui/buttons/button_play_hovered.png", BTN_PLAY);
	add_custom_button("button_quit", "resources/gui/buttons/button_quit.png", "resources/gui/buttons/button_quit_hovered.png", BTN_QUIT, 0.0f, 100.0F);
}

void GUI::show_game_won_screen() {

	device->getCursorControl()->setVisible(true);
	device->getGUIEnvironment()->clear();
	clear_vectors();

	irr::video::ITexture* background = device->getVideoDriver()->getTexture("resources/gui/game_won_image.png");
	device->getGUIEnvironment()->addImage(background, irr::core::position2d<irr::s32>(0, 0));

	add_custom_button("button_play", "resources/gui/buttons/button_play.png", "resources/gui/buttons/button_play_hovered.png", BTN_PLAY);
	add_custom_button("button_menu", "resources/gui/buttons/button_menu.png", "resources/gui/buttons/button_menu_hovered.png", BTN_MENU, 0.0f, 100.0F);
}

void GUI::show_game_over_screen() {

	device->getCursorControl()->setVisible(true);
	device->getGUIEnvironment()->clear();
	clear_vectors();

	irr::video::ITexture* background = device->getVideoDriver()->getTexture("resources/gui/game_over_image.png");
	device->getGUIEnvironment()->addImage(background, irr::core::position2d<irr::s32>(0, 0));

	add_custom_button("button_play", "resources/gui/buttons/button_play.png", "resources/gui/buttons/button_play_hovered.png", BTN_PLAY);
	add_custom_button("button_menu", "resources/gui/buttons/button_menu.png", "resources/gui/buttons/button_menu_hovered.png", BTN_MENU, 0.0f, 100.0F);
}

void GUI::show_pause_screen() {

	device->getCursorControl()->setVisible(true);
	device->getGUIEnvironment()->clear();
	clear_vectors();

	irr::video::ITexture* background = device->getVideoDriver()->getTexture("resources/gui/pause_image.png");
	device->getGUIEnvironment()->addImage(background, irr::core::position2d<irr::s32>(0, 0));

	add_custom_button("button_resume", "resources/gui/buttons/button_resume.png", "resources/gui/buttons/button_resume_hovered.png", BTN_RESUME, -40.0f);
	add_custom_button("button_menu", "resources/gui/buttons/button_menu.png", "resources/gui/buttons/button_menu_hovered.png", BTN_MENU, 0.0f, 100.0F);
	add_custom_button("button_quit", "resources/gui/buttons/button_quit.png", "resources/gui/buttons/button_quit_hovered.png", BTN_QUIT, 0.0f, 200.0F);	
}

void GUI::show_hud() {

	device->getGUIEnvironment()->clear();
	set_crosshair();
}

void GUI::show_interaction_text() {

	font->draw(L"Press E to interact.",
		irr::core::recti(
			SCREEN_WIDTH / 2 - 50,
			SCREEN_HEIGHT - 100,
			SCREEN_WIDTH / 2 + 50,
			SCREEN_HEIGHT - 120),
		irr::video::SColor(255, 255, 255, 255));
}

void GUI::show_objective_text_scene_1() {

	font->draw(L"Objective: In first part, explore rooms to find the key and the door.\n"
		L"After you open the door, explore the next room and match the numbers above the doors\nwith the numbers you find on the notes hidden around objects.\n"
		L"Find the key that unlocks the correct door.",
		irr::core::recti(
			SCREEN_WIDTH / 2 - 300,
			10,
			SCREEN_WIDTH / 2 + 300,
			30),
		irr::video::SColor(255, 255, 255, 255));
}

void GUI::show_objective_text_scene_2() {

	font->draw(L"Objective: Pull the lever that's hidden in one of the swimming pools and then find the exit.\n"
		L"Beware of the snake.",
		irr::core::recti(
			SCREEN_WIDTH / 2 - 100,
			10,
			SCREEN_WIDTH / 2 + 100,
			30),
		irr::video::SColor(255, 255, 255, 255));
}

void GUI::show_hint_scene_1() {
	
	if (elapsed_time > 20 * hint_index)  {

		font->draw(L"Hint: Follow blood traces and remember where you came from",
			irr::core::recti(
				SCREEN_WIDTH  / 2 - 200,
				SCREEN_HEIGHT / 2,
				SCREEN_WIDTH  / 2 + 200,
				SCREEN_HEIGHT / 2 - 20),
			irr::video::SColor(255, 255, 255, 255));

		hint_time += delta_time;
		if (hint_time > 5.0f) {	
			hint_index++;
			hint_time = 0.0f;
		}
	}	
}

void GUI::show_hint_scene_2() {

	if (elapsed_time > 20 * hint_index) {

		font->draw(L"Hint: The exit is somewhere around walls.",
			irr::core::recti(
				SCREEN_WIDTH  / 2 - 100,
				SCREEN_HEIGHT / 2,
				SCREEN_WIDTH  / 2 + 100,
				SCREEN_HEIGHT / 2 - 20),
			irr::video::SColor(255, 255, 255, 255));

		hint_time += delta_time;
		if (hint_time > 5.0f) {
			hint_index++;
			hint_time = 0.0f;			
		}
	}
}

void GUI::show_timer() {

	elapsed_time = timer->getTime() / 1000;

	ui_timer = L"Elapsed time: ";
	ui_timer += elapsed_time;
	ui_timer += "s";

	font->draw(ui_timer, irr::core::recti(10, 10, 260, 22), irr::video::SColor(255, 255, 255, 255));
}

void GUI::show_total_time() {

	timer->stop();

	irr::core::stringw total_time;

	total_time = L"Total time: ";
	total_time += elapsed_time;
	total_time += "s";

	font->draw(total_time, irr::core::recti(10, 10, 260, 22), irr::video::SColor(255, 255, 255, 255));
}

void GUI::add_custom_button(const irr::c8* _name, const irr::io::path _path1, const irr::io::path _path2, ButtonID _button_id, irr::f32 offset_x, irr::f32 offset_y) {

	float position_x = 100.0F;
	float position_y = 200.0F;

	irr::video::ITexture* button_image_unhovered = device->getVideoDriver()->getTexture(_path1);
	irr::video::ITexture* button_image_hovered = device->getVideoDriver()->getTexture(_path2);
	button_hover_textures.push_back(button_image_hovered);
	button_unhover_textures.push_back(button_image_unhovered);

	irr::gui::IGUIButton* button = device->getGUIEnvironment()->addButton
	(
		irr::core::rect<irr::s32>(
			position_x + offset_x,
			position_y + offset_y,
			position_x + offset_x + button_image_unhovered->getSize().Width,
			position_y + offset_y + button_image_unhovered->getSize().Height),
		0,
		_button_id,
		L""
	);
	button->setDrawBorder(false);
	button->setUseAlphaChannel(true);
	button->setImage(button_image_unhovered);
	button->setName(_name);

	buttons.push_back(button);
}

void GUI::button_hover() {

	if (!buttons.empty() && !button_hover_textures.empty()) {
		for (int i = 0; i < buttons.size(); i++) {
			current_hovered_button = device->getGUIEnvironment()->getHovered();
			if (current_hovered_button->getType() == irr::gui::EGUIET_BUTTON) {
				if (buttons[i]->getName() == current_hovered_button->getName()) {
					buttons[i]->setImage(button_hover_textures[i]);
				}
			}
		}
	}
}

void GUI::button_unhover() {

	if (!buttons.empty() && !button_unhover_textures.empty()) {
		for (int i = 0; i < buttons.size(); i++) {
			if (current_hovered_button->getType() == irr::gui::EGUIET_BUTTON) {
				buttons[i]->setImage(button_unhover_textures[i]);
			}
		}
	}
}

void GUI::clear_vectors() {

	if (!buttons.empty()) {
		buttons.clear();
	}
	if (!button_hover_textures.empty()) {
		button_hover_textures.clear();
	}
	if (!button_unhover_textures.empty()) {
		button_unhover_textures.clear();
	}
}

void GUI::update() {

	const irr::u32 current_time = timer->getTime();
	delta_time = (irr::f32)(current_time - previous_time) / 1000.f;
	previous_time = current_time;	

	if (interaction_text_flag) {
	
		show_interaction_text();
	}
	if (objective_text_scene_1_flag) {

		show_objective_text_scene_1();
	}
	if (objective_text_scene_2_flag) {

		show_objective_text_scene_2();
	}	
	if (hint_scene_1_flag) {

		show_hint_scene_1();
	}
	if (hint_scene_2_flag) {

		show_hint_scene_2();
	}
}

void GUI::reset_timer() {

	timer->setTime(0);	
}

void GUI::pause_timer() {

	timer->stop();
}

void GUI::resume_timer() {

	timer->start();
}

void GUI::can_interact(bool _interaction_text_flag) {

	interaction_text_flag = _interaction_text_flag;
}

void GUI::can_show_objective_1(bool _objective_text) {

	objective_text_scene_1_flag = _objective_text;
}

void GUI::can_show_objective_2(bool _objective_text) {

	objective_text_scene_2_flag = _objective_text;
}

void GUI::can_show_hint_scene_1(bool _hint) {

	hint_scene_1_flag = _hint;
}

void GUI::can_show_hint_scene_2(bool _hint) {

	hint_scene_2_flag = _hint;
}