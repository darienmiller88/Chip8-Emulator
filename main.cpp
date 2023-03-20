#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <array>
#include <SFML/Graphics.hpp>
#include "Chip8.h"
using namespace sf;

const int CHIP8_WINDOW_WIDTH = 64;
const int CHIP8_WINDOW_HEIGHT = 32;
const float SCALE = 20;//The window will be SCLAE times bigger than the original 64x32 window.

int main(int argc, char *argv[]){
	using namespace std;

	std::cout << "num args: " << argc << "\n";

    RenderWindow window(
		VideoMode(CHIP8_WINDOW_WIDTH * SCALE, CHIP8_WINDOW_HEIGHT * SCALE), 
		"Darien Miller - Chip8", 
		sf::Style::Close | sf::Style::Titlebar
	);
	Chip8 chip8;
	
	//Load Chip-8 ROM
	chip8.loadRom("./roms/" + string(argv[1]));	
	// window.setFramerateLimit(60);
	while (window.isOpen()) {
		Event e;

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();
			else if (e.type == sf::Event::KeyPressed){
				chip8.handleKeyClick(e);
			}
			else if(e.type == sf::Event::KeyReleased){
				chip8.handleKeyRelease(e);
			}
		}

		//CPU cycle
		chip8.cpuCycle();

		window.clear();
		chip8.drawSprites(window, SCALE);
		window.display();
	}
}