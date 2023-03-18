#ifndef MAIN_H
#define MAIN_H

#include "Game.h"

int main() {

	Game* game = new Game();
	game->run();

	return 0;
}
#endif