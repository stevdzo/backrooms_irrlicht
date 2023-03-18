#include "Game.h"

Game::Game() {

	device =
		irr::createDevice(irr::video::EDT_OPENGL,
			irr::core::dimension2d<irr::u32>(SCREEN_WIDTH, SCREEN_HEIGHT),
			16,
			false,
			true,
			true);

	device->setWindowCaption(L"CS445 Projekat Stevan Jovanov 4168");
}

Game::~Game() {

	printf("Deleting game.");
}

void Game::initialize_gui() {

	gui = new GUI(device);
	gui->show_menu();
}

void Game::initialize_scene() {

	scene_manager = new SceneManager(device);
}

void Game::initialize_input() {

	input = new Input(device, gui, scene_manager);
	device->setEventReceiver(input);
}

void Game::initialization() {

	initialize_gui();
	initialize_scene();
	initialize_input();
}

void Game::update() {

	gui->update();
	scene_manager->update();
	input->update();
}

void Game::run() {

	initialization();

	while (device->run()) {

		if (device->isWindowActive()) {

			device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));

			device->getSceneManager()->drawAll();
			device->getGUIEnvironment()->drawAll();

			update();

			device->getVideoDriver()->endScene();
		}
		else device->yield();
	}
	device->drop();
}