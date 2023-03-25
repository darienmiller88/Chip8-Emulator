#include "Chip8.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <conio.h>

Chip8::Chip8() : rnGen(std::chrono::system_clock::now().time_since_epoch().count()), programCounter(0x200),
delayTimer(0), soundTimer(0), opcode(0), rombufferSize(0){
    graphics.fill(0);
    memory.fill(0);
    registers.fill(0);
    
    for (int i = 0; i < stack.size(); i++){
        stack.push(0);
    }

    //The original machines running Chip-8 interpreters used 4x4 hex key controllers, so mimic the 4x4 key setup
    //by using keys [1, 2, 3, 4], [Q, W, E, R], [A, S, D, F], [Z, X, C, V]
    keyMapping = {
        //Map the 1, 2, 3, and 4 keys to 0x1, 0x2, 0x3 and 0xC
        {sf::Keyboard::Num1, 0x1}, {sf::Keyboard::Num2, 0x2}, {sf::Keyboard::Num3, 0x3}, {sf::Keyboard::Num4, 0xC},
        
        //Map the Q, W, E, and R keys to 0x4, to 0x5, 0x6, 0xD respectively.
        {sf::Keyboard::Q,    0x4}, {sf::Keyboard::W,    0x5}, {sf::Keyboard::E,    0x6}, {sf::Keyboard::R,    0xD},
        
        //Map the A, S, D, and f keys to 0x7, 0x8, 0x9, and 0xE
        {sf::Keyboard::A,    0x7}, {sf::Keyboard::S,    0x8}, {sf::Keyboard::D,    0x9}, {sf::Keyboard::F,    0xE},
        
        //Map the Z, X, C, and V keys to 0xA, 0x0, 0xC, 0xF
        {sf::Keyboard::Z,    0xA}, {sf::Keyboard::X,    0x0}, {sf::Keyboard::C,    0xB}, {sf::Keyboard::V,    0xF},
    };

    //Instead of having a giant switch statement for each opcode, index an hexadecimal from 0-15 into a map, 
    //and retireve the code for each respective opcode instead.
    opcodeMapping = {
        {0x0, [&](){ opcodes.op0NNN(graphics, programCounter, stack); }}, 
        {0x1, [&](){ opcodes.op1NNN(programCounter); }},  
        {0x2, [&](){ opcodes.op2NNN(programCounter, stack); }},  
        {0x3, [&](){ opcodes.op3KKK(programCounter, registers); }},  
        {0x4, [&](){ opcodes.op4XKK(programCounter, registers); }},  
        {0x5, [&](){ opcodes.op5XY0(programCounter, registers); }},
        {0x6, [&](){ opcodes.op6XKK(registers); }},  
        {0x7, [&](){ opcodes.op7XKK(registers); }},  
        {0x8, [&](){ opcodes.op8XYN(registers); }},  
        {0x9, [&](){ opcodes.op9XY0(programCounter, registers); }},  
        {0xA, [&](){ opcodes.opANNN(indexRegister); }},  
        {0xB, [&](){ opcodes.opBNNN(programCounter, registers); }},
        {0xC, [&](){ opcodes.opCXNN(registers); }},  
        {0xD, [&](){ opcodes.opDXYN(registers, graphics, memory, indexRegister); }},  
        {0xE, [&](){ opcodes.opEXNN(programCounter, registers, keyMapping, currentKey); }},  
        {0xF, [&](){ opcodes.opFXNN(programCounter, delayTimer, soundTimer, registers, memory, keyStates, indexRegister); }}
    };
    
    chip8Fontset = { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x81  // F
    };

    //Copy the Chip-8 font into the first 80 slots of memory.
    std::copy_n(chip8Fontset.begin(), chip8Fontset.size(), memory.begin());
}

void Chip8::loadRom(const std::string &romName){
    for (std::ifstream infile(romName, std::ios::binary); infile.good(); rombufferSize++){
        memory[rombufferSize + programCounter] = infile.get();
    }

    std::cout << "buffer: " << rombufferSize << "\n";
}

void Chip8::drawSprites(sf::RenderWindow &window, float pixelWidth){
    int index = 0;

	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 64; j++){
			sf::RectangleShape rect({pixelWidth, pixelWidth});
			
			(graphics[index] == 1) ? rect.setFillColor(sf::Color::White) : rect.setFillColor(sf::Color::Black);
			rect.setPosition({j * pixelWidth, i * pixelWidth});
			window.draw(rect);
            index++;
		}		
	}
}

void Chip8::handleKeyClick(const sf::Event &e){
    if (keyMapping.find(e.key.code) != keyMapping.end()){
        currentKey = e.key.code;
        keyStates[keyMapping[e.key.code]] = true;
    }   
}

void Chip8::handleKeyRelease(const sf::Event &e){
    if (keyMapping.find(e.key.code) != keyMapping.end()){
        currentKey = sf::Keyboard::Unknown;
        keyStates[keyMapping[e.key.code]] = false;
    }
}

void Chip8::cpuCycle(){
    opcode = (memory[programCounter] << 8) | memory[programCounter + 1];
    programCounter += 2;

    if (opcode != 0){
        printf("0x%04x\n", opcode);
    }
   
    uint8_t firstOpCodeNibble = opcode >> 12;
    
    opcodes.setOpcode(opcode);
    opcodeMapping[firstOpCodeNibble]();

    if (delayTimer > 0){
        delayTimer--;
    }

    if(soundTimer > 0){
        soundTimer--;
        std::cout << "BEEP\n";
    }
}