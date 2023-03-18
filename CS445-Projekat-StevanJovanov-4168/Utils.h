#ifndef UTILS_H
#define UTILS_H

#include "Globals.h"

inline irr::f32 distance_to_player(irr::scene::ICameraSceneNode* _player, irr::scene::ISceneNode* _node) {

	irr::core::vector3df distance_vector = irr::core::abs_(_node->getPosition() - _player->getPosition());

	return irr::core::squareroot(distance_vector.X * distance_vector.X +
								 distance_vector.Z * distance_vector.Z);
}

inline irr::f32 rotation_angle(irr::scene::ICameraSceneNode* _node1, irr::scene::ISceneNode* _node2) {

	irr::core::vector3df heading = _node1->getPosition() - _node2->getPosition();

	return std::atan2f(heading.X, heading.Z) * irr::core::RADTODEG;
}
#endif