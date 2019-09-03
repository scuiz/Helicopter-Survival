#ifndef GAME_H
#define GAME_H

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Network.hpp>
#include<SFML/Window.hpp>

#include<vector>
#include<ctime>
#include<iostream>
#include<sstream>

class game
{
private:

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;

	sf::Font scoreFont;
	sf::Text scoreText;

	sf::Font menuFont;
	sf::Text menuText;

	//Game objects
	sf::RectangleShape heli;
	sf::RectangleShape building, topBuilding;
	std::vector<sf::RectangleShape> buildings, topBuildings;

	//textures
	sf::Texture heliTexture;
	sf::Texture *pHeliTexture;

	sf::Texture buildingTexture;
	sf::Texture *pBuildingTexture;
	
	//Gameplay Variables
	unsigned int score, finalScore;
	float heliSize, maxBuildingHeight, minBuildingHeight, maxBuildingWidth, minBuildingWidth, maxGap, minGap;
	bool crashFlag;

	//Game state variables
	enum gameState {MENU, PLAY};
	gameState state;


	//Initialising functions
	void initWindow();
	void initVariables();
	void initGraphics();
	void initBuilding();
	void initHeli();
	void initScore();
	void initMenu();

public:
	//Constructors and destructors
	game();
	~game();

	//Game update functions
	void pollEvents();
	
	const bool isRunning() const;
	const bool isMenu() const;

	void newBuilding();
	
	void updateMenu();
	void updateHeli();
	void updateBuildings();
	void updateScore();
	void update();

	void renderMenu();
	void renderHeli();
	void renderBuildings();
	void renderScore();
	void render();

	void showFinalScore();
	void resetGame();
	

};

#endif // !GAME_H



