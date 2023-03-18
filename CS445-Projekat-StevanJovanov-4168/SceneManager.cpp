#include "SceneManager.h"

SceneManager::SceneManager(irr::IrrlichtDevice* _device) : device(_device) {

	sound_engine = irrklang::createIrrKlangDevice();
	controller = new GameController(device, sound_engine);
	current_scene = CurrentScene::SCENE_1;
}

SceneManager::~SceneManager() {

	printf("Deleting scene manager");
}

GameController* SceneManager::get_controller() {

	return controller;
}

irrklang::ISoundEngine* SceneManager::get_sound_engine() {

	return sound_engine;
}

bool SceneManager::is_game_over() {

	return lose_flag;
}

void SceneManager::is_game_over(bool _lose_flag) {

	lose_flag = _lose_flag;
}

bool SceneManager::is_game_won() {

	return win_flag;
}

void SceneManager::is_game_won(bool _win_flag) {

	win_flag = _win_flag;
}

void SceneManager::set_current_scene(CurrentScene _current_scene) {

	current_scene = _current_scene;
}

CurrentScene SceneManager::get_current_scene() {

	return current_scene;
}

void SceneManager::clear_camera() {

	controller->clear_camera();
}

void SceneManager::add_last_camera() {

	controller->add_last_camera();
}

void SceneManager::clear_current_scene() {

	device->getSceneManager()->clear();
	sound_engine->stopAllSounds();
}

// Ucitavanje scena kao i inicijalizacija igraca i ostalih objekata sa kojima igrac moze da vrsi interakciju.
void SceneManager::load_scene() {

	switch (current_scene) {

	case SCENE_1:

		controller->restart_game();
		clear_current_scene();
		device->getSceneManager()->loadScene("resources/scene/scene.irr");
		controller->add_player();
		controller->add_smilers();
		controller->get_doors_from_scene();
		controller->get_keys_from_scene();
		controller->get_objects_from_scene1();
		controller->get_papers_from_scene();
		controller->assign_key_to_door();
		controller->play_buzz_sound();	
		break;

	case SCENE_2:

		controller->restart_game();
		clear_current_scene();
		device->getSceneManager()->loadScene("resources/scene2/scene2.irr");
		controller->add_player(irr::core::vector3df(260.0F, 98.0F, 30.0F));
		controller->get_objects_from_scene2();
		controller->get_snake_from_scene();
		controller->get_bodies_from_scene();
		controller->get_billboard_from_scene();
		controller->rotate_bodies();
		controller->add_water_surface();	
		controller->play_indoor_sound();
		break;

	default:
		lose_flag = false;
		win_flag = false;
		break;
	}
}

// Ovde se nalaze funkcije koje treba da se azuriraju u svakom frejmu igre.
void SceneManager::update() {

	if (device->getSceneManager()->getActiveCamera() != nullptr) {

		controller->update();

		switch ((current_scene))
		{
		case SCENE_1:

			controller->face_player();
			controller->kill_player();
			controller->teleport();
			if (controller->wrong_door()) {

				current_scene = SCENE_1;
				load_scene();
			}
			if (controller->can_load_scene_2()) {

				current_scene = SCENE_2;
				load_scene();
			}
			break;

		case SCENE_2: {

			controller->snake_chase_player();
			controller->check_for_escape();

			break;
		}
		default:
			break;
		}
		sound_engine->setListenerPosition(controller->get_player()->get_camera()->getAbsolutePosition(),
			controller->get_player()->get_camera()->getTarget());

		lose_flag = controller->game_over();
		win_flag = controller->game_won();
	}
}