#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <array>
#include "Chip8.h"
#include <SFML/Graphics.hpp>
using namespace sf;

const int CHIP8_WINDOW_WIDTH = 64;
const int CHIP8_WINDOW_HEIGHT = 32;
const float SCALE = 20;//The window will be X times bigger than the original 64x32 window.

void fillWindow(RenderWindow &window, const std::array<int, 64 * 32> &graphics);
void initializeArray(std::array<int, 64 * 32> &graphics);

int main(){
	using namespace std;

	// uint8_t num = 1;
	// uint16_t opcode = (num << 8) | 1; 
	// cout << "opcode: " << opcode << "\n";
	// uint16_t res = opcode & 0xF000;
	// cout << "res: " << res << "\n";

	srand(time(nullptr));

    RenderWindow window(
		VideoMode(CHIP8_WINDOW_WIDTH * SCALE, CHIP8_WINDOW_HEIGHT * SCALE), 
		"Chip8", 
		sf::Style::Close | sf::Style::Titlebar
	);
	Chip8 chip8;
	array<int, 64 * 32> graphics;       

	//Load Chip-8 ROM
	chip8.loadRom("./roms/programs/IBM Logo.ch8");
	initializeArray(graphics);
	uint16_t opcode = 1 << 8 | 1;
	std::cout << "opcode main: " << opcode << "\n";
	std::cout << "shifted: " << (opcode & 0x0FFF) << "\n";
	// while (window.isOpen()) {
	// 	Event e;

	// 	while (window.pollEvent(e)) {
	// 		if (e.type == sf::Event::Closed)
	// 			window.close();
	// 	}

	// 	//CPU cycle
		chip8.cpuCycle();

	// 	//Handle Chip-8

	// 	window.clear();
	// 	fillWindow(window, graphics);
	// 	window.display();
	// }
}

void initializeArray(std::array<int, 64 * 32> &graphics){
	for (int i = 0; i < graphics.size(); i++){
		graphics[i] = rand() % 2;
	}
}
 
void fillWindow(RenderWindow &window, const std::array<int, 64 * 32> &graphics){
	int index = 0;

	for (int i = 0; i < CHIP8_WINDOW_HEIGHT; i++){
		for (int j = 0; j < CHIP8_WINDOW_WIDTH; j++){
			RectangleShape rect({SCALE, SCALE});
			
			(graphics[++index] == 1) ? rect.setFillColor(Color::Black) : rect.setFillColor(Color::White);
			rect.setPosition({j * SCALE, i * SCALE});
			window.draw(rect);
		}		
	}
}