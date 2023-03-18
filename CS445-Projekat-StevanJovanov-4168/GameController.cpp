#include "GameController.h"

GameController::GameController(irr::IrrlichtDevice* _device,
	irrklang::ISoundEngine* _engine) : device(_device),
	sound_engine(_engine)
{
	death_timer = 0.0f;
	previous_time = device->getTimer()->getTime();
	lose_flag = false;
	win_flag = false;
	scene_2_flag = false;
}

GameController::~GameController() {

	printf("Deleting gamecontroller.");
}

Player* GameController::get_player() {

	return player;
}

float GameController::get_death_timer() {

	return death_timer;
}

irr::f32 GameController::get_delta_time() {

	return delta_time;
}

std::vector<irr::scene::ISceneNode*> GameController::get_collected_papers() {

	return collected_papers;
}

// Brise se kamera. Potrebno kada se igra zavrsi ili se vracamo u glavni meni.
void GameController::clear_camera() {

	player->set_camera_copy(player->get_camera());
	player->get_camera()->remove();
}

void GameController::add_last_camera() { //

	player->add_camera(player->get_camera_copy()->getPosition(),
					   player->get_camera_copy()->getTarget()
					  );
	player->add_collision();
}


// Inicijalizuje se igrac. Dodaje se kamera kao I kolizija sa elementima scene.
void GameController::add_player(irr::core::vector3df _position) {

	player = new Player(device, sound_engine);
	player->add_camera(_position, irr::core::vector3df(560.0F, 150.0F, 0.0F));
	player->add_collision();
}

// 5 Smajlera se postavlja na neke od 10 mogucih lokacija. Lokacije se biraju nasumicno.
void GameController::add_smilers() {

	if (!smilers.empty()) {
		
		smilers.clear();
	}
	std::vector<irr::core::vector3df> possible_locations;
	std::vector<irr::core::vector3df> real_locations;

	possible_locations.push_back(irr::core::vector3df(290.0F, 150.0F, -210.0F));
	possible_locations.push_back(irr::core::vector3df(19.0F, 150.0F, -64.0F));
	possible_locations.push_back(irr::core::vector3df(-97.0F, 150.0F, -240.0F));
	possible_locations.push_back(irr::core::vector3df(-221.0F, 150.0F, 33.0F));
	possible_locations.push_back(irr::core::vector3df(-221.0F, 150.0F, -357.0F));
	possible_locations.push_back(irr::core::vector3df(50.0F, 150.0F, -480.0F));
	possible_locations.push_back(irr::core::vector3df(16.0F, 150.0F, -240.0F));
	possible_locations.push_back(irr::core::vector3df(141.0F, 150.0F, -240.0F));
	possible_locations.push_back(irr::core::vector3df(141.0F, 150.0F, -146.0F));
	possible_locations.push_back(irr::core::vector3df(-159.0F, 150.0F, -241.0F));

	int max_size = possible_locations.size();

	for (int i = 0; i < max_size / 2; i++) {

		srand(time(NULL));
		int random_index = rand() % possible_locations.size();
		real_locations.push_back(possible_locations.at(random_index));

		possible_locations.erase(possible_locations.begin() + random_index);
	}

	for (auto& location : real_locations) {

		smilers.push_back(new Smiler(device, device->getSceneManager()->addCubeSceneNode(), location));
	}
}

void GameController::remove_smilers() {

	if (!smilers.empty()) { 

		smilers.clear(); 
	}
}

// Smajleri se rotiraju u pravcu igraca.
void GameController::face_player() {

	for (auto it = smilers.begin(); it != smilers.end(); ++it) {

		Smiler* smiler = *it;
		irr::f32 distance = distance_to_player(player->get_camera(), smiler->get_object_node());

		if (distance < 200.0f) {

			irr::f32 angle = rotation_angle(player->get_camera(), smiler->get_object_node());
			smiler->get_object_node()->setRotation(irr::core::vector3df(0.0F, angle, 0.0F));

		}
	}
}

// Igrac umire ako se dovoljno priblizi Smajlerima. U tom trenutku Smajler se stvara ispred kamere ~jumpscare.
void GameController::kill_player() {

	for (auto it = smilers.begin(); it != smilers.end(); ++it) {

		Smiler* smiler = *it;
		irr::f32 distance = distance_to_player(player->get_camera(), smiler->get_object_node());

		if (distance < 40.0f) {

			player->get_camera()->addChild(smiler->get_object_node());
			smiler->get_object_node()->setPosition(irr::core::vector3df(0.0F, 0.0F, 3.0F));
			smiler->get_object_node()->setRotation(irr::core::vector3df(0.0F, 0.0F, 0.0F));
			sound_engine->play2D("resources/sounds/jumpscare.mp3", false, false, true);		

			timer_flag = true;
		}
	}
}

// Ucitavanje svih vrata iz scene u poseban vektor
void GameController::get_doors_from_scene() {

	if (!doors.empty()) {

		doors.clear();
	}
	doors.push_back(new Door(device, "door0"));
	doors.push_back(new Door(device, "door1"));
	doors.push_back(new Door(device, "door2"));
	doors.push_back(new Door(device, "door3"));
	doors.push_back(new Door(device, "door4"));
	doors.push_back(new Door(device, "door5"));
	doors.push_back(new Door(device, "door6"));

	for (auto& door : doors) {
	
		interaction_items.push_back(door->get_object_node());
	}
}

// Ucitavanje kljuceva iz scene u poseban vektor
void GameController::get_keys_from_scene() {

	if (!keys.empty()) {

		keys.clear();
	}
	keys.push_back(new Key(device, "key0"));
	keys.push_back(new Key(device, "key1"));

	for (auto& key : keys) {

		interaction_items.push_back(key->get_object_node());
	}
}

void GameController::get_objects_from_scene1() {

	radio = device->getSceneManager()->getSceneNodeFromName("radio");
	lever1 = device->getSceneManager()->getSceneNodeFromName("lever");

	interaction_items.push_back(radio);
	interaction_items.push_back(lever1);
}

void GameController::get_objects_from_scene2() {

	lever2 = device->getSceneManager()->getSceneNodeFromName("lever2");
	interaction_items.push_back(lever2);
}

void GameController::get_snake_from_scene() {

	snake = new Snake(device, "snake");
	snake->add_collision();
	hiss = sound_engine->play3D("resources/sounds/snake_hiss.mp3", snake->get_object_node()->getAbsolutePosition(), true, true, true, irrklang::ESM_AUTO_DETECT, true);
	hiss->setVolume(1);
	hiss->setMinDistance(50);
}

void GameController::get_bodies_from_scene() {

	bodies.push_back(device->getSceneManager()->getSceneNodeFromName("HANG1"));
	bodies.push_back(device->getSceneManager()->getSceneNodeFromName("HANG2"));
	bodies.push_back(device->getSceneManager()->getSceneNodeFromName("HANG3"));
}

void GameController::get_papers_from_scene() {

	for (size_t i = 1; i <= PAPER_COUNT; i++) {
		std::stringstream ss_name;
		ss_name << i;
		std::string str_name = "paper_" + ss_name.str();
		std::string path = "resources/gui/papers/" + str_name + ".png";

		irr::scene::ISceneNode* paper = device->getSceneManager()->getSceneNodeFromName(str_name.c_str());
		papers.push_back(paper);

		interaction_items.push_back(paper);
	}
	irr::scene::ISceneNode* paper = device->getSceneManager()->getSceneNodeFromName("paper_1");
	irr::core::vector3df new_position = paper->getPosition();
	new_position.Z -= 4.0f;
	new_position.Y -= 1.0f;
	paper->setPosition(new_position);
	paper->setRotation(irr::core::vector3df(0.0f, 90.0f, 0.0f));
}

// Dodeljivanje kljuceva svim vratima. 
// Prvi kljuc otkljucava prva vrata (Level 0), dok drugi sva ostala (Level 1).
void GameController::assign_key_to_door() {

	std::vector<Door*> temp;
	temp.push_back(doors[0]);

	keys.at(0)->set_doors(temp);
	keys[0]->get_doors()[0]->get_object_node()->setID(111);

	temp.clear();
	for (size_t i = 1; i < doors.size(); i++) {

		temp.push_back(doors[i]);
	}
	keys.at(1)->set_doors(temp);
	keys.at(1)->get_doors().at(4)->get_object_node()->setID(555); // poseban ID za vrata koja vode do izlaza
}

// Klikom na dugme E proverava se da li igrac moze da pokupi kljuc, ukoliko moze, kljuc se postavlja kao dete kamere I moze da ga nosi sa sobom.
void GameController::pickup_key() {

	irr::core::line3df ray =
		device->getSceneManager()->
		getSceneCollisionManager()->
		getRayFromScreenCoordinates
		(irr::core::vector2di(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 
			player->get_camera());

	for (auto& key : keys) {
		irr::scene::ISceneNode* node =
			device->getSceneManager()->
			getSceneCollisionManager()->
			getSceneNodeFromRayBB(ray);
		if (node) {	
			if (node->getName() == key->get_object_node()->getName()) {		
				key->is_picked_up(true);
				if (key->is_picked_up()) {				
					player->get_camera()->addChild(key->get_object_node());
					key->get_object_node()->updateAbsolutePosition();
					key->get_object_node()->setPosition(irr::core::vector3df(0.5F, -0.5F, 1.F));
					key->get_object_node()->setRotation(irr::core::vector3df(-90.0F, 0.0F, 225.0F));
					key->get_object_node()->setScale(irr::core::vector3df(2.0F, 2.0F, 2.0F));
					if (key->get_object_node()->getAnimators().size() > 0) { 
						key->get_object_node()->removeAnimators(); 
						interaction_items.erase(std::remove(interaction_items.begin(), interaction_items.end(), key->get_object_node()), interaction_items.end());
					}
				}
			}
		}
	}
}

void GameController::unlock_door() {

	for (auto& key : keys) {	
		if (key->is_picked_up()) {		
			for (auto& door : doors) {
				if (!door->is_unlocked()) {							
					if (distance_to_player(player->get_camera(), door->get_object_node()) < 20.0f) {
						key->unlock_door(door);
						key->destroy();
						player->add_collision();												
						if (door->get_object_node()->getID() == 111) {										
							play_hallway_sound();
							interaction_items.erase(std::remove(interaction_items.begin(), interaction_items.end(), door->get_object_node()), interaction_items.end());
						}
					} 
				}
			}
		}
	}
}

// RickRoll
void GameController::play_radio() {

	irrklang::ISound* rick;
	interaction_items.push_back(radio);

	if (radio) {
		if (distance_to_player(player->get_camera(), radio) < 20.0f) {
			if (!sound_engine->isCurrentlyPlaying("resources/sounds/rick.mp3")) {
				rick = sound_engine->play3D("resources/sounds/rick.mp3", radio->getAbsolutePosition(), true, false, true, irrklang::ESM_AUTO_DETECT, true);
				rick->setVolume(1);
				rick->setMinDistance(75);
				rick->setPlaybackSpeed(0.95F);
				irrklang::ISoundEffectControl* fx = rick->getSoundEffectControl();
				fx->enableFlangerSoundEffect();
				fx->enableWavesReverbSoundEffect();
			}
		} 
	}
}

void GameController::play_buzz_sound() {

	irrklang::ISound* buzz = sound_engine->play2D("resources/sounds/buzz.mp3", true, false, true);
	buzz->setVolume(0.03F);
}

void GameController::play_hallway_sound() {

	irrklang::ISound* hallway = sound_engine->play2D("resources/sounds/hallway.mp3", true, false, true);
}

// Pritiskom na dugme E u blizini prekidaca, igrac spusta prekidac I iznad wc solje se stvara kljuc.
void GameController::pull_lever() {

	irr::scene::ISceneNode* wc = device->getSceneManager()->getSceneNodeFromName("toilet");

	irr::core::line3df ray =
		device->getSceneManager()->getSceneCollisionManager()->
		getRayFromScreenCoordinates(irr::core::vector2di(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 
		player->get_camera());

	irr::scene::ISceneNode* selected_node =
		device->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);

	if (selected_node->getName() == lever1->getName()) {
		if (distance_to_player(player->get_camera(), lever1) <= 20.0f) {
			lever1->setRotation(irr::core::vector3df(0.0F, 180.0F, 180.0F));
			if (!sound_engine->isCurrentlyPlaying("resources/sounds/wc.mp3")) {
				irrklang::ISound* flush = sound_engine->play3D("resources/sounds/wc.mp3", wc->getAbsolutePosition(), false, false, true, irrklang::ESM_AUTO_DETECT, true);
				flush->setVolume(1);
				flush->setMinDistance(50);
			}
			flush_key(wc);
		}
	}
}

void GameController::flush_key(irr::scene::ISceneNode* _wc) {

	irr::scene::ISceneNode* key = keys.at(1)->get_object_node();

	irr::core::vector3df position = _wc->getPosition();
	position.Y += 25.0F;
	position.Z += 10.0F;

	key->setPosition(irr::core::vector3df(position.X, position.Y, position.Z));

	irr::scene::ISceneNodeAnimator* anim = device->getSceneManager()->createRotationAnimator(irr::core::vector3df(1.0F, 1.0F, 1.0F));
	key->addAnimator(anim);

	anim->drop();
}

void GameController::teleport() {

	irr::scene::ISceneNode* portal = device->getSceneManager()->getSceneNodeFromName("portal");
	if (distance_to_player(player->get_camera(), portal) <= 20.0f) {

		clear_camera();		
		player->add_camera(irr::core::vector3df(-44.0f, 154.0f, -31.0f), irr::core::vector3df(-144.0f, 128.0f, -157.0f));
		player->add_collision();
	}
}

void GameController::collect_paper() {

	irr::core::line3df ray =
		device->getSceneManager()->
		getSceneCollisionManager()->
		getRayFromScreenCoordinates
		(irr::core::vector2di(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			player->get_camera());

	irr::scene::ISceneNode* node =
		device->getSceneManager()->
		getSceneCollisionManager()->
		getSceneNodeFromRayBB(ray);

	int offset_y = 70;
	for (size_t i = 0; i < papers.size(); i++) {	
		if (node) {
			if (node->getName() == papers[i]->getName()) {				
				collected_papers.push_back(papers[i]);
				show_collected_papers();
				interaction_items.erase(std::remove(interaction_items.begin(), interaction_items.end(), papers[i]), interaction_items.end());
				papers[i]->remove();
			}
		}
	}
}

void GameController::show_collected_papers() {

	irr::u32 offset_y = 70;
	irr::u32 position = 0;
	for (size_t i = 0; i < collected_papers.size(); i++) {

		std::string str_name = collected_papers[i]->getName();
		position = (str_name.at(6) - '0') - 1;

		std::string path = "resources/gui/papers/" + str_name + ".png";
		irr::c8* name = (irr::c8*)str_name.c_str();
		irr::video::ITexture* paper_texture = device->getVideoDriver()->getTexture(path.c_str());
		irr::gui::IGUIImage* paper_image = device->getGUIEnvironment()->addImage(paper_texture,
			irr::core::position2di(20, SCREEN_HEIGHT / 2 + offset_y * position));
	}
}

void GameController::update() {

	const irr::u32 current_time = device->getTimer()->getTime();
	delta_time = (irr::f32)(current_time - previous_time) / 1000.f;
	previous_time = current_time;	

	if (timer_flag == true) {
		death_timer += delta_time;
		if (death_timer > 1.0f) {
			
			device->getGUIEnvironment()->clear();
			death_timer = 0.0f;
			lose_flag = true;
			timer_flag = false;
		}
	}
}

void GameController::restart_game() {

	death_timer = 0.0f;
	timer_flag = false;
	lose_flag = false;
	win_flag = false;
	interaction_items.clear();
	papers.clear();
	collected_papers.clear();
	bodies.clear();
}

void GameController::add_water_surface() {

	irr::u32 tileNumber = 128;
	irr::scene::IAnimatedMesh* terrain_mesh = device->getSceneManager()->addHillPlaneMesh("",
		irr::core::dimension2d<float>(8, 8),
		irr::core::dimension2d<irr::u32>(tileNumber, tileNumber), 0, 0.0f,
		irr::core::dimension2df(0, 0),
		irr::core::dimension2df(16, 16));

	irr::scene::ISceneNode* water_mesh = device->getSceneManager()->
		addWaterSurfaceSceneNode(
			terrain_mesh->getMesh(0), 
			.1f,
			300.0f, 
			10.0f, 
			0, 
			-1, 
			irr::core::vector3df(0.0f, 73.0f, 0.0f));
	water_mesh->setMaterialTexture(0, device->getVideoDriver()->getTexture("resources/scene2/scene2_resources/water.jpg"));
}

void GameController::snake_chase_player() {

	if (!lose_flag) {
		if (snake->chase_player(player)) {

			hiss->setIsPaused(false);
		}
		else hiss->setIsPaused(true);

		if (snake->kill_player(player)) {

			if (!sound_engine->isCurrentlyPlaying("resources/sounds/snake_attack.mp3")) {
				sound_engine->play2D("resources/sounds/snake_attack.mp3", false, false, true);
			}
			hiss->setIsPaused(true);
			timer_flag = true;
		}
	}
}

void GameController::pull_lever_2() {
	 
	interaction_items.push_back(lever2);

	irr::core::line3df ray =
		device->getSceneManager()->getSceneCollisionManager()->
		getRayFromScreenCoordinates(irr::core::vector2di(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
			player->get_camera());

	irr::scene::ISceneNode* selected_node =
		device->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);

	if (selected_node->getName() == lever2->getName()) {
		if (distance_to_player(player->get_camera(), lever2) <= 20.0f) {
			lever2->setRotation(irr::core::vector3df(-90.0F, 0.0F, 180.0F));
			irr::scene::ISceneNode* secret_door = device->getSceneManager()->getSceneNodeFromName("secret_door");
			irr::core::vector3df new_position = secret_door->getPosition();
			new_position.Y += 50.0f;
			secret_door->setPosition(new_position);

			irrklang::ISound* push_sound = 
				sound_engine->play3D("resources/sounds/wall_push.mp3", secret_door->getAbsolutePosition(), false, false, true, irrklang::ESM_AUTO_DETECT, true);

			push_sound->setVolume(1);
			push_sound->setMinDistance(200);
		}
	}
}

void GameController::get_billboard_from_scene() {

	light_node = (irr::scene::ILightSceneNode*) device->getSceneManager()->getSceneNodeFromName("Light15");

	irr::scene::ISceneNode* aura = device->getSceneManager()->addBillboardSceneNode(light_node, irr::core::dimension2d<irr::f32>(10.0f, 10.0f));
	aura->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	aura->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	aura->setMaterialTexture(0, device->getVideoDriver()->getTexture("resources/scene2/scene2_resources/aura.png"));

	irr::video::SLight pointLightData;
	pointLightData.DiffuseColor = irr::video::SColor(255, 255, 255, 255);
	pointLightData.SpecularColor = irr::video::SColor(255, 255, 255, 255);
	light_node->setLightData(pointLightData);
}

void GameController::rotate_bodies() {

	srand(time(NULL));
	for (auto& body : bodies) {
		
		float random_rotation_factor = (rand() % 11 + 1) / 10.0f;
		irr::scene::ISceneNodeAnimator* anim = device->getSceneManager()->createRotationAnimator(irr::core::vector3df(0.0F, random_rotation_factor, 0.0F));
		body->addAnimator(anim);

		anim->drop();
	}
}

void GameController::play_indoor_sound() {

	irrklang::ISound* indoor = sound_engine->play2D("resources/sounds/indoor_pool.mp3", true, false, true);
	indoor->setVolume(0.03F);
}

void GameController::check_for_escape() {

	if (distance_to_player(player->get_camera(), light_node) < 20.0f) {
	
		win_flag = true;
	}
}

bool GameController::can_show_interaction_text() {		

	for (auto& item : interaction_items) {
	
		if (distance_to_player(player->get_camera(), item) < 20.0f) {

			interaction_text_flag = true;
			break;
		}
		interaction_text_flag = false;
	}
	return interaction_text_flag;
}

bool GameController::wrong_door() {

	for (auto& door : keys.at(1)->get_doors()) {
		if (door->is_unlocked()) {
			if (door->get_object_node()->getID() != 555) {
				if (player->get_camera()->getPosition().X > door->get_object_node()->getPosition().X + 20.0F) {			
					return true;
				}
			}
			else if (door->get_object_node()->getID() == 555) {
				if (player->get_camera()->getPosition().X > door->get_object_node()->getPosition().X + 80.0F) {

					scene_2_flag = true;
					device->getGUIEnvironment()->clear();
				}
			}
		}
	}
	return false;
}

bool GameController::game_over() {

	return lose_flag;
}

bool GameController::game_won() {

	return win_flag;
}

bool GameController::can_load_scene_2() {

	return scene_2_flag;
}