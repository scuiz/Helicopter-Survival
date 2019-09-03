#include "game.h"

int main() {
	
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	game game;

	while (game.isRunning()) {

		if (game.isMenu()) {
			
			game.updateMenu();
			game.renderMenu();

		}
		else {
			//Update all the objects
			game.update();

			//Render all the objects in the window
			game.render();
		}


	}

	return 0;
}