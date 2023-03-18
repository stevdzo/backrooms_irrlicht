#include "Snake.h"
#include "Player.h"

Snake::Snake(irr::IrrlichtDevice* _device,
	const irr::c8* _name) : GameObject(
		_device,
		nullptr)
{
	object_node = device->getSceneManager()->getSceneNodeFromName(_name);
	animated_mesh = (irr::scene::IAnimatedMeshSceneNode*) object_node;

	animated_mesh->setLoopMode(true);
	animated_mesh->setFrameLoop(0, 33);
	animated_mesh->setAnimationSpeed(30);
}

Snake::~Snake() {

	printf("Deleting snake.");
}

void Snake::add_collision() {

	if (object_node->getAnimators().size() > 1) {
		object_node->removeAnimator(*(object_node->getAnimators().getLast()));
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
		object_node,
		irr::core::vector3df(1.0F, 0.5F, 0.5F),
		irr::core::vector3df(0.0F, 1.0F, 0.0F),
		irr::core::vector3df(0.0F, 0.0F, 0.0F));
	meta->drop();

	object_node->addAnimator(anim);
	anim->drop();
}

bool Snake::chase_player(Player* _player) {

	float dist = distance_to_player(_player->get_camera(), object_node);

	if (distance_to_player(_player->get_camera(), object_node) < 150.0f) {

		irr::f32 angle = rotation_angle(_player->get_camera(), object_node);
		object_node->setRotation(irr::core::vector3df(0.0F, angle - 90.0f, 0.0F));

		irr::core::vector3df seek_velocity;

		seek_velocity.X = _player->get_camera()->getPosition().X - object_node->getPosition().X;
		seek_velocity.Y = 0;
		seek_velocity.Z = _player->get_camera()->getPosition().Z - object_node->getPosition().Z;

		seek_velocity /= 200.0f;

		object_node->setPosition(object_node->getPosition() + seek_velocity);

		return true;
	}
	return false;
}

bool Snake::kill_player(Player* _player) {

	if (distance_to_player(_player->get_camera(), object_node) < 20.0f) {

		animated_mesh->setFrameLoop(33, 66);
		animated_mesh->setAnimationSpeed(30);

		irr::video::ITexture* attack_texture = device->getVideoDriver()->getTexture("resources/gui/snake_attack.jpg");
		irr::gui::IGUIImage* attack_image = device->getGUIEnvironment()->addImage(attack_texture, irr::core::position2d<irr::s32>(0, 0));

		return true;
	}
	return false;
}