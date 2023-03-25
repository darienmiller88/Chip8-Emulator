#include "Opcodes.hpp"
#include <iostream>
#include <chrono>

Opcodes::Opcodes() : rnGen(std::chrono::system_clock::now().time_since_epoch().count()){
    opcode = 0x200;
}

void Opcodes::setOpcode(uint16_t opcode){
    this->opcode = opcode;
}

void Opcodes::op0NNN(std::array<uint8_t, 64 * 32> &graphics, uint16_t &programCounter, std::stack<uint16_t> &stack){
    if (opcode == 0x00E0){//Clear the graphics array
        graphics.fill(0);
    }else if(opcode == 0x00EE){
        programCounter = stack.top();//Set the program counter to the address at the top of the stack.
        stack.pop();
        // std::cout << "program counter in instruction 0x0: " << programCounter << "\n";
    }       
}

void Opcodes::op1NNN(uint16_t &programCounter){
    programCounter = opcode & 0x0FFF;
}

void Opcodes::op2NNN(uint16_t &programCounter, std::stack<uint16_t> &stack){
    stack.push(programCounter);
    programCounter = opcode & 0x0FFF;
}

void Opcodes::op3KKK(uint16_t &programCounter, std::array<uint8_t, 16> &registers){
    uint8_t x = (opcode & 0x0F00) >> 8; //retrieve the second nibble from the opcode
    uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);//Get the last 2 nibbles from the opcode

    if (registers[x] == nn){
        programCounter += 2;
    }
}

void Opcodes::op4XKK(uint16_t &programCounter, std::array<uint8_t, 16> &registers){
    uint8_t x = (opcode & 0x0F00) >> 8; //retrieve the second nibble from the opcode
    uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);//Get the last 2 nibbles from the opcode

    if (registers[x] != nn){
        programCounter += 2;
    }
}


void Opcodes::op5XY0(uint16_t &programCounter, std::array<uint8_t, 16> &registers){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (registers[x] == registers[y]){
        programCounter += 2;
    }
}

void Opcodes::op6XKK(std::array<uint8_t, 16> &registers){
    uint8_t x  = (opcode & 0x0F00) >> 8;
    uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);

    registers[x] = nn;
}

void Opcodes::op7XKK(std::array<uint8_t, 16> &registers){
    uint8_t x  = (opcode & 0x0F00) >> 8;
    uint8_t nn = static_cast<uint8_t>(opcode & 0x00FF);

    registers[x] += nn;
}


void Opcodes::op8XYN(std::array<uint8_t, 16> &registers){
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
            //By ANDing Vx and 0x80, we will retrieve the most significant bit, which will result in a 0 or 128
            registers[0xF] = registers[x] & 0x80;
            registers[x] <<= 1;
        default:
            break;
    }
}


void Opcodes::op9XY0(uint16_t &programCounter, std::array<uint8_t, 16> &registers){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (registers[x] != registers[y]){
        programCounter += 2;
    }
}

void Opcodes::opANNN(uint16_t &indexRegister){
    indexRegister = opcode & 0x0FFF;
}

void Opcodes::opBNNN(uint16_t &programCounter, std::array<uint8_t, 16> &registers){
    programCounter = (opcode & 0x0FFF) + static_cast<uint16_t>(registers[0]);
}

void Opcodes::opCXNN(std::array<uint8_t, 16> &registers){
    std::uniform_int_distribution<int> dist(0, 256);
    uint8_t x  = (opcode & 0x0F00) >> 8;
    uint8_t nn = (opcode & 0x00FF);

    registers[x] = nn & static_cast<uint8_t>(dist(rnGen));
}

void Opcodes::opDXYN(std::array<uint8_t, 16> &registers, std::array<uint8_t, 64 * 32> &graphics, 
    std::array<uint8_t, 4096> &memory, uint16_t indexRegister){
    uint8_t x      = (opcode & 0x0F00) >> 8;
    uint8_t y      = (opcode & 0x00F0) >> 4;
    uint8_t height = opcode & 0x000F;
    // std::cout << "x: " << (int)x << " y: " << (int)y << " and height: " << (int)height << "\n";

    //First, set the flag register to 0;
    registers[0xF] = 0;
    for (int row = 0; row < height; row++){
        uint8_t yPosition = (registers[y] % 32) + row;
        uint8_t spriteByte = memory[indexRegister + row];

        //Sprites are 8-bits wide, so iterate over each row 8 times. Start at the most significant bit,
        //and go down to the least significant bit.
        for (int cols = 0; cols < 8; cols++){
            uint8_t xPosition = (registers[x] % 64) + cols;
            
            //translate 2D coordinates (x, y) to a 1D index.
            uint16_t index    = (yPosition * 64) + xPosition;

            //Extract a bit from the row starting from the most significant using AND operator with
            //the power value. 
            uint8_t bit       = static_cast<uint8_t>((0x80 >> cols) & spriteByte);

            //If the bit in the sprite row is on, and the bit in the graphics array at (x, y) converted
            //to a 1D index is on, set the V[F] to on (1), and XOR the pixel at (x, y) with 1.
            if (bit){                 
                if (graphics[index] == 1){
                    registers[0xF] = 1;    
                }
                    
                graphics[index] ^= 1;
            }
        }
    } 
}

void Opcodes::opEXNN(uint16_t &programCounter, std::array<uint8_t, 16> &registers, 
    std::unordered_map<sf::Keyboard::Key, uint8_t> &keyMapping, sf::Keyboard::Key currentKey){
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
}

void Opcodes::opFXNN(
    uint16_t &programCounter, uint8_t delayTimer, uint8_t soundTimer, 
    std::array<uint8_t, 16> &registers, std::array<uint8_t, 4096> &memory, std::array<bool, 16ULL> &keyStates,
    uint16_t &indexRegister
){
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
}
