#include "Player.h"

Player::Player(irr::IrrlichtDevice* _device, 
			   irrklang::ISoundEngine* _sound_engine) : GameObject(
														_device, 
														nullptr) 
{
	sound_engine = _sound_engine;
	footsteps = sound_engine->play2D("resources/sounds/footsteps.mp3", true, true);
}

Player::~Player() {

	printf("Deleting player.");
}

irr::scene::ICameraSceneNode* Player::get_camera() {

	return camera;
}

irr::scene::ICameraSceneNode* Player::get_camera_copy() {

	return camera_copy;
}

void Player::set_camera(irr::scene::ICameraSceneNode* _camera) { //

	camera = _camera;
}

void Player::set_camera_copy(irr::scene::ICameraSceneNode* _camera_copy) { //

	camera_copy = _camera_copy;
}

// FPS kamera. Igra kontrolise kameru pomocu tastera W, A, S, D.
void Player::add_camera(irr::core::vector3df _start_position, irr::core::vector3df _start_target) {

	irr::SKeyMap key_map[4];

	key_map[0].Action = irr::EKA_MOVE_FORWARD;
	key_map[0].KeyCode = irr::KEY_KEY_W;

	key_map[1].Action = irr::EKA_MOVE_BACKWARD;
	key_map[1].KeyCode = irr::KEY_KEY_S;

	key_map[2].Action = irr::EKA_STRAFE_LEFT;
	key_map[2].KeyCode = irr::KEY_KEY_A;

	key_map[3].Action = irr::EKA_STRAFE_RIGHT;
	key_map[3].KeyCode = irr::KEY_KEY_D;

	camera = device->getSceneManager()->addCameraSceneNodeFPS
	(
		nullptr,
		100.0F,
		0.04,
		0,
		key_map,
		4,
		true,
		0.0F
	);
	camera->bindTargetAndRotation(true);
	camera->setPosition(_start_position);
	camera->setTarget(_start_target);

	device->getCursorControl()->setVisible(false);
}

// Registrovanje kolizije sa elementima scene.
void Player::add_collision() {

	if (camera->getAnimators().size() > 1) {
		camera->removeAnimator(*(camera->getAnimators().getLast()));
	}

	irr::scene::IMetaTriangleSelector* meta = device->getSceneManager()->createMetaTriangleSelector();
	irr::core::array<irr::scene::ISceneNode*> nodes;
	device->getSceneManager()->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes);

	for (size_t i = 0; i < nodes.size(); i++) {

		irr::scene::ISceneNode* node = nodes[i];
		irr::scene::ITriangleSelector* selector = 0;

		switch (node->getType()) {

			//case irr::scene::ESNT_CUBE:
		case irr::scene::ESNT_ANIMATED_MESH:
			selector = device->getSceneManager()->createTriangleSelectorFromBoundingBox(node);
			break;

		case irr::scene::ESNT_MESH:
			selector = device->getSceneManager()->createTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
			break;

		case irr::scene::ESNT_SPHERE:
			selector = device->getSceneManager()->createTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
			break;

		case irr::scene::ESNT_TERRAIN:
			selector = device->getSceneManager()->createTerrainTriangleSelector((irr::scene::ITerrainSceneNode*)node);
			break;

		case irr::scene::ESNT_OCTREE:
			selector = device->getSceneManager()->createOctreeTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
			break;
		}
		if (selector) {

			meta->addTriangleSelector(selector);
			selector->drop();
		}
	}
	irr::scene::ISceneNodeAnimator* anim = device->getSceneManager()->createCollisionResponseAnimator(
		meta,
		camera,
		irr::core::vector3df(3.0F, 3.0F, 3.0F),
		irr::core::vector3df(0.0F, -2.0F, 0.0F),
		irr::core::vector3df(0.0F, 20.0F, 0.0F));
	meta->drop();

	camera->addAnimator(anim);
	anim->drop();
}

void Player::play_footsteps() {

	footsteps->setIsPaused(false);
}

void Player::pause_footsteps() {

	footsteps->setIsPaused(true);
}