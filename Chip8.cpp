#include "Chip8.h"
#include <algorithm>
#include <iostream>
#include <fstream>

Chip8::Chip8(){
    programCounter = 0x200;//Chip-8 programs start at position 0x200, or 512 in memory.
    stackPointer   = 0;
    delayTimer     = 0;
    soundTimer     = 0;
    opcode         = 0;

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
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    //Copy the Chip-8 font into the first 80 slots of memory.
    std::copy_n(chip8Fontset.begin(), chip8Fontset.size(), memory.begin());

    // for (int i = 0; i < chip8Fontset.size(); i++){
    //     std::cout << "slot: " << (int) memory[i] << "\n";
    // }
}

void Chip8::loadRom(const std::string &romName){
    std::ifstream infile(romName);
    int bufferSize = 0;
    uint8_t val;

    if (!infile){
        std::cout << "File could not be opened!\n";
        return;
    }
    
    while (infile >> val){
        memory[programCounter + bufferSize] = val;//offset by the program counter to start at position 0x200(512)
        bufferSize++;
    }
    
    std::cout << "buffer: " << bufferSize << "\n";
    infile.close();
}

void Chip8::cpuCycle(){
    opcode = memory[programCounter] << 8 | memory[programCounter + 1];
    programCounter += 2;

    uint8_t firstOpCodeByte = opcode >> 12;
    switch (firstOpCodeByte){
        case 0x0:
            if (opcode == 0x00E0){//Clear the graphics array
                graphics.fill(0);
            }else if(opcode == 0x00EE){
                programCounter = stack.top();//Set the program counter to the address at the top of the stack.
                stackPointer--;
            }else{
                std::cout << "0nnn: " << opcode << "\n";
            }
            
            break;
        case 0x1: {
            uint16_t nnn = opcode & 0x0FFF;//Obtain the last 3 significant nibbles from the opcode. 

            //Jump to position nnn in the memory array by setting the program counter to that address.
            programCounter = nnn;
            break;
        }
        //In the case of the first nibble being a two, we will perform the following:
        //1. Increment the stack pointer by one
        //2. Push the current value of the programCounter onto the stack.
        //3. Set the value of the program counter to 2nnn.
        case 0x2:
            stackPointer++;
            stack.push(programCounter);
            programCounter = opcode & 0x0FFF;

            break;
        //In the case of the first nibble being a three, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0x3XNN.
        //2. Obtain the value of nn, which is the last two nibbles in 0x3Xnn.
        //3. Check to see if they're equal to each other. If so, increment program counter by 2.
        case 0x3: {
            uint16_t x  = (opcode & 0x0F00) >> 8; //retrieve the second nibble from the opcode
            uint16_t nn = opcode & 0x00FF;//Get the last 2 nibbles from the opcode

            if (registers[x] == nn){
                programCounter += 2;
            }

            break;
        }
        //In the case of the first nibble being a four, we will perform the exact same instruction as with three,
        //but increment the program counter by two if x and nn are NOT equal.
        case 0x4: {
            uint16_t x  = (opcode & 0x0F00) >> 8; //retrieve the second nibble from the opcode
            uint16_t nn = opcode & 0x00FF;//Get the last 2 nibbles from the opcode

            if (registers[x] != nn){
                programCounter += 2;
            }

            break;
        }
        case 0x5: {
            uint16_t x = (opcode & 0x0F00) >> 8;
            uint16_t y = (opcode & 0x00F0) >> 4;

            if (registers[x] == registers[y]){
                programCounter++;
            }
            
            break;
        }
        case 0x6: {
            //6xnn
            uint16_t x  = (opcode & 0x0F00) >> 8;
            uint16_t nn = opcode & 0x00FF;

            break;
        }
        case 0x7:

            break;
        case 0x8:

            break;
        case 0x9:

            break;
        case 0xA:

            break;
        case 0xB:

            break;
        case 0xC:

            break;
        case 0xD:

            break;
        case 0xE:

            break;
        case 0xF:

            break;
        default:
            break;
    }

}