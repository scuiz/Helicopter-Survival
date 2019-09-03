#include "game.h"

//Constructors and destructors

game::game() {
	
	this->initWindow();
	this->initGraphics();
	this->initVariables();
	this->initBuilding();
	this->initHeli();
	this->initScore();
	this->initMenu();

}

game::~game() {

	delete this->window;

}

//Initialiser functions

void game::initWindow() {
	
	this->window = nullptr;
	
	this->videoMode.width  = 800;
	this->videoMode.height = 600;

	this->window = new sf::RenderWindow(this->videoMode, "Helicopter Survival");

	this->window->setFramerateLimit(100);
}

void game::initGraphics() {
	
	//Load the font for the menu
	if (!this->menuFont.loadFromFile("fonts/sansation.ttf"))
	{
		std::cout << "Error loading font\n";
	}

	//Load the font for the score
	if (!this->scoreFont.loadFromFile("fonts/sansation.ttf"))
	{
		std::cout << "Error loading font\n";
	}

	//load heli texture
	if (!this->heliTexture.loadFromFile("textures/heli.png"))
	{
		std::cout << "Error loading helicopter texture\n";
	}
	this->pHeliTexture = &heliTexture;

	//load building texture
	if (!this->buildingTexture.loadFromFile("textures/building.png"))
	{
		std::cout << "Error loading building texture\n";
	}
	this->pBuildingTexture = &buildingTexture;
}

void game::initVariables() {
	   	
	this->state = MENU;
	this->score = 0;
	this->finalScore = 0;
	this->heliSize = 40.f;
	this->minGap = 3.5 * this->heliSize;
	this->maxGap = 0.4 * this->window->getSize().y;
	this->maxBuildingHeight = 0.85 * this->window->getSize().y;
	this->minBuildingHeight = 0.15 * this->window->getSize().y;
	this->maxBuildingWidth = 2*this->heliSize;
	this->minBuildingWidth = this->heliSize;
	this->crashFlag = false;
	
}

void game::initBuilding() {
	
	this->building.setPosition(0.f, 0.f);
	this->building.setSize(sf::Vector2f(50.f, 50.f));
	this->building.setTexture(this->pBuildingTexture);

	this->topBuilding.setPosition(0.f, 0.f);
	this->topBuilding.setSize(sf::Vector2f(50.f, 50.f));
	this->topBuilding.setTexture(this->pBuildingTexture);

}

void game::initHeli() {

	this->heli.setPosition(100.f, 0.5 * this->window->getSize().y);
	this->heli.setSize(sf::Vector2f(this->heliSize, this->heliSize));
	this->heli.setTexture(this->pHeliTexture);

}

void game::initScore()
{
	this->scoreText.setFont(this->scoreFont);
	this->scoreText.setCharacterSize(20);
	this->scoreText.setStyle(sf::Text::Bold);
	this->scoreText.setPosition(0, 0);
	this->scoreText.setString("0");
	this->scoreText.setFillColor(sf::Color::Black);
}

void game::initMenu() {
	
	this->menuText.setFont(this->menuFont);
	this->menuText.setCharacterSize(50);
	this->menuText.setStyle(sf::Text::Bold);
	this->menuText.setPosition(0,0.3 * this->window->getSize().y );
	this->menuText.setString("HELICOPTER SURVIVAL \n \n \nPRESS P TO PLAY");
	this->menuText.setFillColor(sf::Color::Black);

}


//Event polling

void game::pollEvents() {

	while (this->window->pollEvent(this->event)) {

		switch (this->event.type) {
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape) {
				this->window->close();
				break;
			}
			if (this->event.key.code == sf::Keyboard::P) {
				this->state = PLAY;
				break;
			}
			if (this->event.key.code == sf::Keyboard::M) {
				this->resetGame();
				break;
			}
			
		}
	}

}

//Game running check

const bool game::isRunning() const {

	return this->window->isOpen();

}

const bool game::isMenu() const {
	
	return this->state == MENU;

}

//Spawn a new building -> this ensures that top and bottom building vectors are same length
void game::newBuilding() {
	
	//Spawn a new bottom building
	float buildingHeight = static_cast<float>( (rand() % static_cast<int>(this->maxBuildingHeight - this->minBuildingHeight)) + this->minBuildingHeight);
	float buildingWidth  = static_cast<float>( (rand() % static_cast<int>(this->maxBuildingWidth - this->minBuildingWidth)) + this->minBuildingWidth);
	
	this->building.setSize(sf::Vector2f(buildingWidth, buildingHeight));
	this->building.setPosition(this->window->getSize().x,this->window->getSize().y - buildingHeight);

	this->buildings.push_back(this->building);


	//Spawn a new top building
	float gap = static_cast<float>((rand() % static_cast<int>(this->maxGap - this->minGap)) + this->minGap);

	float buildingHeightTop = std::max(building.getPosition().y - gap, 0.f);
	float buildingWidthTop  = static_cast<float>((rand() % static_cast<int>(this->maxBuildingWidth - this->minBuildingWidth)) + this->minBuildingWidth);
	
	this->topBuilding.setSize(sf::Vector2f(buildingWidthTop, buildingHeightTop));
	this->topBuilding.setPosition(this->window->getSize().x, 0.f);

	this->topBuildings.push_back(this->topBuilding);

}


//Object update functions

//Update the main menu
void game::updateMenu() {

	this->pollEvents();

}


void game::updateHeli() {
	
	if (this->heli.getPosition().y + this->heli.getSize().y <= this->window->getSize().y) {
		
		if (this->heli.getPosition().y >= 0.f) {
			
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				this->heli.move(0.f, -3.f);
			}
			else {
				this->heli.move(0.f, 3.f);
			}

		}
		else {
			this->heli.move(0.f, 3.f);
		}


	}
	else {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->heli.move(0.f, -3.f);
		}
	}

}


void game::updateBuildings() {
	
	//spawn a building
	if (!this->buildings.size()) {
		this->newBuilding();
	}
	else if (this->window->getSize().x - this->buildings.back().getPosition().x > 350.f) {
		this->newBuilding();
	}


	//move the buildings
	for (auto& b : this->buildings) {
		b.move(-3.f, 0.f);
	}

	for (auto& b : this->topBuildings) {
		b.move(-3.f, 0.f);
	}

	//remove any building which are off the LHS of the screen
	for (size_t i = 0; i < this->buildings.size(); i++) {
		
		if (this->buildings[i].getPosition().x < 0.f) {
			this->buildings.erase(this->buildings.begin() + i);
			this->score += 1;
		}

	}

	for (size_t i = 0; i < this->topBuildings.size(); i++) {

		if (this->topBuildings[i].getPosition().x < 0.f) {
			this->topBuildings.erase(this->topBuildings.begin() + i);
		}

	}

}

void game::updateScore() {

	std::ostringstream ss;
	ss << "Score:" << this->score;

	this->scoreText.setString(ss.str());
}

void game::update() {

	this->pollEvents();
	
	if (this->crashFlag == false) {
		
		this->updateBuildings();
		this->updateHeli();
		this->updateScore();

		for (size_t i = 0; i < this->buildings.size(); i++) {
			if (buildings[i].getGlobalBounds().intersects(this->heli.getGlobalBounds())) {
				this->crashFlag = true;
				this->finalScore = this->score;
			}
		}

		for (size_t i = 0; i < this->topBuildings.size(); i++) {
			if (topBuildings[i].getGlobalBounds().intersects(this->heli.getGlobalBounds())) {
				this->crashFlag = true;
				this->finalScore = this->score;
			}
		}
	
	}


}


//Show the final score
void game::showFinalScore() {
	std::ostringstream ss;
	ss << "FINAL SCORE: " << this->finalScore << "\n \nPress Esc to quit \nPress M for Menu";

	this->scoreText.setCharacterSize(40);
	this->scoreText.setString(ss.str());
	this->window->draw(this->scoreText);
}






//Object render functions

//Show the main menu
void game::renderMenu() {

	this->window->clear(sf::Color::Color(211, 211, 211));
	this->window->draw(this->menuText);
	this->window->display();

}

void game::renderHeli() {
	this->window->draw(this->heli);
}

void game::renderBuildings() {
	
	for (auto& b : this->buildings) {
		this->window->draw(b);
	}

	for (auto& b : this->topBuildings) {
		this->window->draw(b);
	}
}

void game::renderScore() {
	this->window->draw(this->scoreText);
}

void game::render() {
	
	//clear the existing window
	this->window->clear(sf::Color::Color(211, 211, 211) );
	
	if (this->crashFlag == false) {
		this->renderBuildings();
		this->renderHeli();
		this->renderScore();
	}
	else {
		this->showFinalScore();
	}
	
	//display everything that has been drawn in the window
	this->window->display();
}


//Reset the game so it can be replayed
void game::resetGame() {
	
	this->initVariables(); //game state set to MENU in this function
	this->initBuilding();
	this->initHeli();
	this->initScore();
	this->initMenu();

	std::vector<sf::RectangleShape> tempVec1, tempVec2;
	this->buildings = tempVec1;
	this->topBuildings = tempVec2;

}

