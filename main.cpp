#include <iostream>
#include <string>
#include <cstdlib>
#include "Snake.h"

using namespace sf;

void printGrid(Sprite &grayBlock, RenderWindow &window);
void placeFood(Sprite &food);

//Number of rows and columns in the game
constexpr int NUM_COLUMNS = 35, NUM_ROWS = 25;

//How wide/tall each square in each row and column are.
const int ROW_COLUMN_SIZE = 30;

//Calculate window width and height from the number of rows and column multiplied by how wide they are.
const int WINDOW_WIDTH = NUM_COLUMNS * ROW_COLUMN_SIZE, WINDOW_HEIGHT = NUM_ROWS * ROW_COLUMN_SIZE;

int main(){
	srand(time(nullptr));
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake!", sf::Style::Close | sf::Style::Titlebar);
	Texture foodTexture, grayBlockTexture;

	foodTexture.loadFromFile("sprites/greenblock.png");
	grayBlockTexture.loadFromFile("sprites/grayblock.png");

	Sprite food(foodTexture), grayBlock(grayBlockTexture);
	Snake s1(4, ROW_COLUMN_SIZE);

	//resize the sprite to half of its size original size.
	food.setScale(ROW_COLUMN_SIZE / food.getGlobalBounds().width, ROW_COLUMN_SIZE / food.getGlobalBounds().height);
	grayBlock.setScale(ROW_COLUMN_SIZE / grayBlock.getGlobalBounds().width, ROW_COLUMN_SIZE / grayBlock.getGlobalBounds().height);

	Clock clock;
	float timer = 0.f, delay = 0.3f;
	window.setFramerateLimit(15);
	placeFood(food);
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time; 
		
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();
		}
 
		s1.controlSnake(ROW_COLUMN_SIZE);
		s1.updateSnake(WINDOW_WIDTH - ROW_COLUMN_SIZE, WINDOW_HEIGHT - ROW_COLUMN_SIZE);

		//Handle snake controls, and update its position by passing in the window boundaries.
		//Check to see if the snake ate the food, and if so, the snake will grow.
		if (s1.eat(food.getPosition())){
			placeFood(food);
			std::cout << "food position x: " << food.getPosition().x  << " and y: " << food.getPosition().y << "\n";
		}
		
		s1.resetSnake();

		window.clear();
		printGrid(grayBlock, window);
		s1.drawSnake(window);
		window.draw(food);
		window.display();
	}
}

void placeFood(Sprite &food){
	int x = rand() % (NUM_COLUMNS - 1) * ROW_COLUMN_SIZE;
	int y = rand() % (NUM_ROWS - 1)    * ROW_COLUMN_SIZE;
	
	food.setPosition(x, y);
}

void printGrid(Sprite &grayBlock, RenderWindow &window){
	for (int i = 0; i < NUM_ROWS; i++){
		for (int j = 0; j < NUM_COLUMNS; j++){
			grayBlock.setPosition(j * ROW_COLUMN_SIZE, i * ROW_COLUMN_SIZE);
			window.draw(grayBlock);
		}
	}
}