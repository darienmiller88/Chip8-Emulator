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
}

void Chip8::loadRom(const std::string &romName){
    // std::ifstream infile(romName, std::ios::binary);
    //  std::vector<char> bytes( 
    //     std::istreambuf_iterator<char>(infile), 
    //     (std::istreambuf_iterator<char>())
    // );
    // std::copy_n(bytes.begin(), bytes.size(), memory.begin() + programCounter);
    // infile.close();
        
    for (std::ifstream f(romName, std::ios::binary); f.good(); rombufferSize++){
        memory[rombufferSize + programCounter] = f.get();
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

    //    std::cout << "key clicked: "  << currentKey << "\n";
//    std::cout << "program counter: " << programCounter << "\n";
//    if (opcode != 0){
//         printf("0x%04x\n", opcode);
//    }
   
    uint8_t firstOpCodeNibble = opcode >> 12;
    switch (firstOpCodeNibble){
        case 0x0: {
            // std::cout << "instruction 0:\n";
            if (opcode == 0x00E0){//Clear the graphics array
                graphics.fill(0);
            }else if(opcode == 0x00EE){
                programCounter = stack.top();//Set the program counter to the address at the top of the stack.
                stack.pop();
                // std::cout << "program counter in instruction 0x0: " << programCounter << "\n";
            }
            
            break;
        }
        case 0x1: {
            // std::cout << "instruction 1:\n";
            //Obtain the last 3 significant nibbles from the opcode, and then jump to position nnn in the 
            //memory array by setting the program counter to that address.
            programCounter = opcode & 0x0FFF;
            // std::cout << "program counter in 0x1: " << programCounter << "\n";
            break;
        }

        //In the case of the first nibble being a two, we will perform the following:
        //1. Increment the stack pointer by one
        //2. Push the current value of the programCounter onto the stack.
        //3. Set the value of the program counter to nnn.
        case 0x2: {
            // std::cout << "instruction 2:\n";
            stack.push(programCounter);
            programCounter = opcode & 0x0FFF;
            // std::cout << "program counter in 0x2: " << programCounter << "\n";

            break;
        }

        //In the case of the first nibble being a three, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0x3XNN.
        //2. Obtain the value of nn, which is the last two nibbles in 0x3Xnn.
        //3. Check to see if they're equal to each other. If so, increment program counter by 2.
        case 0x3: {
            // std::cout << "instruction 3:\n";
            uint8_t x = (opcode & 0x0F00) >> 8; //retrieve the second nibble from the opcode
            uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);//Get the last 2 nibbles from the opcode

            if (registers[x] == nn){
                programCounter += 2;
            }

            break;
        }

        //In the case of the first nibble being a four, we will perform the exact same instruction as with three,
        //but increment the program counter by two if x and nn are NOT equal.
        case 0x4: {
            // std::cout << "instruction 4:\n";
            uint8_t x = (opcode & 0x0F00) >> 8; //retrieve the second nibble from the opcode
            uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);//Get the last 2 nibbles from the opcode

            if (registers[x] != nn){
                programCounter += 2;
            }

            break;
        }

        //In the case of the first nibble being a five, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0x5XYN.
        //2. Obtain the value of y, which is the third nibble in 0x5XYN.
        //3. Check to see if the value at V[x] and V[y] are equal. If so, increment program counter by 2.
        case 0x5: {
            // std::cout << "instruction 5:\n";
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;

            if (registers[x] == registers[y]){
                programCounter += 2;
            }
            
            break;
        }

        //In the case of the first nibble being a six, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0x6XNN.
        //2. Obtain the value of NN, which is the third and fourth nibble in 0x6XNN, and cast it to 8-bit.
        //3. Assign NN to registers[x].
        case 0x6: {
            // std::cout << "instruction 6:\n";
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);

            registers[x] = nn;
            break;
        }

        //In the case of the first nibble being a seven, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0x7XNN.
        //2. Obtain the value of nn, which is the third nibble in 0x7XNN.
        //3. Add nn to registers[x]
        case 0x7: {
            // std::cout << "instruction 7:\n";
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);

            registers[x] += nn;
            break;
        }

        //In the case of the first nibble being a 8, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0x8XYM.
        //2. Obtain the value of y, which is the third nibble in 0x8XYM.
        //3. Obtain the value of m, which is the fourth nibble in 0XYM  
        //3. Switch on m to see if it 1-7 or E, assign a value to register[x] based on the case.
        case 0x8: {
            // std::cout << "instruction 8:\n";
            uint8_t x = (opcode & 0x0F00) >> 8;//Get second nibble
            uint8_t y = (opcode & 0x00F0) >> 4;//Get third nibble
            uint8_t m = opcode & 0x000F;//Get last nibble

            //Parse "m", which will be a value in the range [0, 1, 2, 3, 4, 5, 6, 7, E].
            switch (m){
                case 0x0:
                    registers[x] = registers[y];
                    break;
                case 0x1:
                    registers[x] |= registers[y];
                    break;
                case 0x2:
                    registers[x] &= registers[y];   
                    break;
                case 0x3:
                    registers[x] ^= registers[y];
                    break;
                case 0x4:
                    registers[0xF] = (registers[x] + registers[y] > 255) ? 1 : 0;
                    registers[x] += registers[y];
                    break;
                case 0x5:
                    registers[0xF] = (registers[x] - registers[y] < 0) ? 1 : 0;
                    registers[x] -= registers[y];
                    break;
                case 0x6:
                    //Check to see if the bit in the 2^1 position is 1 or 0.
                    registers[0xF] = registers[x] & 0x1;
                    registers[x] >>= 1;
                    break;
                case 0x7:
                    registers[0xF] = (registers[y] > registers[x]) ? 1 : 0;
                    registers[x] = registers[y] - registers[x];
                    break;
                case 0xE:
                    //Check to see if the bit in the 2^7 position is a 1 or a 0.
                    registers[0xF] = registers[x] & 0x80;
                    registers[x] <<= 1;
                default:
                    break;
            }

            break;
        }
        case 0x9: {
            // std::cout << "instruction 9:\n";
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;

            if (registers[x] != registers[y]){
                programCounter += 2;
            }
            
            break;
        }
        
        //Set the index register to nnn
        case 0xA: {
            // std::cout << "instruction A:\n";
            indexRegister = opcode & 0x0FFF;
            break;
        }
        case 0xB: {
            // std::cout << "instruction B:\n";
            programCounter = (opcode & 0x0FFF) + static_cast<uint16_t>(registers[0]);
            break;
        }
        case 0xC: {
            // std::cout << "instruction C:\n";
            std::uniform_int_distribution<int> dist(0, 256);
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn = (opcode & 0x00FF);

            registers[x] = nn & static_cast<uint8_t>(dist(rnGen));
            break;
        }

        //In the case of the first nibble being a D, we will perform the following:
        //1. Obtain the value of x, which is the second nibble in 0xDXYH.
        //2. Obtain the value of y, which is the third nibble in 0xDXYH.
        //3. Obtain the sprite height, which is the fourth nibble in 0xDXYH
        case 0xD: {
            uint8_t x      = (opcode & 0x0F00) >> 8;
            uint8_t y      = (opcode & 0x00F0) >> 4;
            uint8_t height = opcode & 0x000F;
            // std::cout << "x: " << (int)x << " y: " << (int)y << " and height: " << (int)height << "\n";
            //Rows, or bytes of the sprite that will be parsed through

            //First, set the flag register to 0;
            registers[0xF] = 0;
            for (int row = 0; row < height; row++){
                uint8_t yPosition = registers[y] + row;
                uint8_t spriteByte = memory[indexRegister + row];

                //Sprites are 8-bits wide, so iterate over each row 8 times. Start at the most significant bit,
                //and go down to the least significant bit.
                for (int cols = 0; cols < 8; cols++){
                    uint8_t xPosition = registers[x] + cols;
                    
                    //translate 2D coordinates (x, y) to a 1D index.
                    uint16_t index    = (yPosition * 64) + xPosition;

                    //Extract a bit from the row starting from the most significant using AND operator with
                    //the power value. 
                    uint8_t bit       = static_cast<uint8_t>((0x80 >> cols) & spriteByte);

                    //If the bit in the sprite row is on, and the bit in the graphics array at (x, y) converted
                    //to a 1D index is on, set the V[F] to on (1), and the pixel at (x, y) to 0.
                    if (graphics[index] == 1 && bit){                    
                        graphics[index] = 0;
                        registers[0xF] = 1;
                    }else if (graphics[index] == 0 && bit){
                        graphics[index] = 1;
                    }
                }
            } 

            break;
        }

        case 0xE: {
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn = (opcode & 0x00FF);

            switch (nn){
                case 0x9E:
                    if (registers[x] == keyMapping[currentKey]){
                        std::cout << (int)keyMapping[currentKey] << " was clicked 9E:\n";
                        programCounter += 2;
                    }
                    
                    break;
                case 0xA1:
                    /* code */
                     if (registers[x] != keyMapping[currentKey]){
                        // std::cout << (int)keyMapping[currentKey] << " was clicked A1:\n";
                        programCounter += 2;
                    }

                    break;
            }
            break;
        }
        case 0xF: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            // std::cout << "instruction F:\n";

            switch (nn){
                case 0x07:
                    registers[x] = delayTimer;
                    break;
                case 0x0A: {
                    bool isKeyPressed = false;

                    for (int i = 0; i < keyStates.size(); i++){
                        if (keyStates[i]){
                            isKeyPressed = true;
                            registers[x] = static_cast<uint8_t>(i);
                            break;
                        } 
                    }
                    
                    //If this instruction is hit, and a key hasn't been hit yet, decrement the program counter,
                    //which will cause the cpu cycle to hit this instruction repeatedly until a key is pressed
                    if (!isKeyPressed){
                        programCounter -= 2;
                        return;
                    }
                    
                    break;
                }

                case 0x15:
                    delayTimer = registers[x];
                    break;
                case 0x18:
                    soundTimer = registers[x];
                    break;
                case 0x1E:
                    indexRegister += registers[x];
                    break;
                case 0x29:
                    //Take the value of registers[x], which will be 0x0-0xF, and multiply it by 5, which is the 
                    //width of a font sprite to get which "row" in the font array to point to.
                    std::cout << "registers[x]: " << (int)registers[x] << "\n";
                    indexRegister = static_cast<uint16_t>(registers[x] * 5);
                    break;
                case 0x33: {
                    memory[indexRegister]     = static_cast<uint8_t>(registers[x] / 100); //hundreds
                    memory[indexRegister + 1] = static_cast<uint8_t>((registers[x] % 100) / 10);//tens
                    memory[indexRegister + 2] = static_cast<uint8_t>(registers[x] % 10);//ones
                    
                    break;
                }
                case 0x55:
                    for (int i = 0; i <= x; i++){
                        memory[indexRegister + i] = registers[i];
                    }

                    break;
                case 0x65:
                    for (int i = 0; i <= x; i++){
                        registers[i] = memory[indexRegister + i];
                    }
                
                    break;
                default:
                    std::cout << "unsupported opcode in 0xF: " << opcode << "\n";
                    break;
            }

            break;
        }
        default:
            printf("unsupported opcode: 0x%04x\n", opcode);
            break;
    }

    if (delayTimer > 0){
        delayTimer--;
    }

    if(soundTimer > 0){
        soundTimer--;
        std::cout << "BEEP\n";
    }
    
}