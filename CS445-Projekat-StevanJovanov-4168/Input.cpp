#include "Input.h"

Input::Input(irr::IrrlichtDevice* _device, GUI* _gui, SceneManager* _scene_manager) : device(_device), gui(_gui), scene_manager(_scene_manager) {

	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) {

		key_down[i] = false;
		key_up[i] = false;
	}
}

Input::~Input() {

	printf("Deleting input.");
}

bool Input::isKeyDown(irr::EKEY_CODE _key_code) const {

	return key_down[_key_code]; 
}

bool Input::isKeyUp(irr::EKEY_CODE _key_code) const {

	return key_up[_key_code];
}

bool Input::OnEvent(const irr::SEvent& _event) {

	if (_event.EventType == irr::EET_KEY_INPUT_EVENT) {

		key_down[_event.KeyInput.Key] = _event.KeyInput.PressedDown;
		key_up[_event.KeyInput.Key] = !_event.KeyInput.PressedDown;

		on_keyboard();
	}
	if (_event.EventType == irr::EET_GUI_EVENT) {

		if (_event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED) {
		
			switch (_event.GUIEvent.Caller->getID()) {

			case BTN_PLAY:

				on_play();
				break;

			case BTN_QUIT:

				on_quit();
				break;

			case BTN_MENU:

				on_menu();
				break;
			case BTN_RESUME:

				on_resume();
				break;
			default:
				break;
			}
		}

		if (_event.GUIEvent.EventType == irr::gui::EGBS_BUTTON_MOUSE_OVER) {

			on_hover();
		}	

		if (_event.GUIEvent.EventType == irr::gui::EGBS_BUTTON_MOUSE_OFF) {

			on_unhover();
		}
	}
	return false;
}

void Input::on_keyboard() {

	if (scene_manager->get_controller()->get_player() != nullptr) {

		if (isKeyDown(irr::KEY_KEY_W) ||
			isKeyDown(irr::KEY_KEY_A) ||
			isKeyDown(irr::KEY_KEY_S) ||
			isKeyDown(irr::KEY_KEY_D))
		{
			on_player_movement();
		}
		if (!isKeyDown(irr::KEY_KEY_W) &&
			!isKeyDown(irr::KEY_KEY_A) &&
			!isKeyDown(irr::KEY_KEY_S) &&
			!isKeyDown(irr::KEY_KEY_D))
		{
			on_player_stop_movement();
		}
		if (isKeyDown(irr::KEY_KEY_E)) {

			on_interaction();
		}
		if (isKeyDown(irr::KEY_ESCAPE)) {

			on_pause();
		}
	}
}

void Input::on_player_movement() {

	scene_manager->get_controller()->get_player()->play_footsteps();
}

void Input::on_player_stop_movement() {

	scene_manager->get_controller()->get_player()->pause_footsteps();
}

void Input::on_interaction() {

	if (scene_manager->get_current_scene() == SCENE_1) {

		scene_manager->get_controller()->pickup_key();
		scene_manager->get_controller()->unlock_door();
		scene_manager->get_controller()->play_radio();
		scene_manager->get_controller()->pull_lever();
		scene_manager->get_controller()->collect_paper();
	}
	if (scene_manager->get_current_scene() == SCENE_2) {

		scene_manager->get_controller()->pull_lever_2();
	}
}

void Input::on_play() {

	scene_manager->set_current_scene(SCENE_1);
	scene_manager->load_scene();
	gui->reset_timer();
	gui->resume_timer();
	gui->show_hud();	
	game_running_flag = true;
}

void Input::on_pause() {

	if (game_running_flag) {

		scene_manager->clear_camera();
		scene_manager->get_sound_engine()->stopAllSounds();
		gui->show_pause_screen();
		gui->pause_timer();
		game_running_flag = false;
	}
}

void Input::on_resume() {

	scene_manager->add_last_camera();
	gui->show_hud();	
	gui->resume_timer();

	if (scene_manager->get_current_scene() == SCENE_1) {

		scene_manager->get_controller()->show_collected_papers();
		scene_manager->get_controller()->play_buzz_sound();
	}
	if (scene_manager->get_current_scene() == SCENE_2) {
	
		scene_manager->get_controller()->play_indoor_sound();
	}
	game_running_flag = true;
}

void Input::on_quit() {

	device->closeDevice();
}

void Input::on_menu() {
	
	scene_manager->clear_camera();
	scene_manager->get_sound_engine()->stopAllSounds();
	gui->show_menu();
}

void Input::on_hover() {

	gui->button_hover();
}

void Input::on_unhover() {

	gui->button_unhover();
}

void Input::on_game_won() {

	if (scene_manager->is_game_won() && game_running_flag) {

		scene_manager->clear_camera();
		scene_manager->get_sound_engine()->stopAllSounds();
		gui->show_game_won_screen();
		game_running_flag = false;
	}
}

void Input::on_game_over() {

	if (scene_manager->is_game_over() && game_running_flag) {
		
		scene_manager->clear_camera();	
		scene_manager->get_sound_engine()->stopAllSounds();
		gui->show_game_over_screen();
		game_running_flag = false;	
	}
}

void Input::update() {

	if (game_running_flag) {

		gui->show_timer();
		if (scene_manager->get_current_scene() == SCENE_1) {
		
			gui->can_show_objective_1(true);
			gui->can_show_objective_2(false);

			gui->can_show_hint_scene_1(true);
			gui->can_show_hint_scene_2(false);
		}
		if (scene_manager->get_current_scene() == SCENE_2) {

			gui->can_show_objective_2(true);
			gui->can_show_objective_1(false);

			gui->can_show_hint_scene_2(true);
			gui->can_show_hint_scene_1(false);
		}
	} else {
		gui->can_show_objective_1(false);
		gui->can_show_objective_2(false);

		gui->can_show_hint_scene_1(false);
		gui->can_show_hint_scene_2(false);
	}
	if (scene_manager->get_controller()->can_show_interaction_text()) {
	
		gui->can_interact(true);
	}
	else {
	
		gui->can_interact(false);
	}
	on_game_won();
	on_game_over();
}