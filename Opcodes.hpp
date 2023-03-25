#pragma once
#include <array>
#include <stack>
#include <cstdint>
#include <random>
#include <unordered_map>
#include <SFML/Graphics.hpp>

// In order to refactor my code, I opted to create an Opcode class in order to seperate out the operations
//from the chip8 class to reduce the amount of code in that class. Hindsight however, I don't think was a 
//good idea lol. A lot of the functions for each opcode requires many more arguments than I feel comfortable
//providing to a function, with 0xF requiring 7! In the future, I might consider moving these functions back
//into the Chip8 class.
class Opcodes{
    public:
        Opcodes();
        void setOpcode(uint16_t opcode);
        void op0NNN(std::array<uint8_t, 64 * 32> &graphics, uint16_t &programCounter, std::stack<uint16_t> &stack);
        void op1NNN(uint16_t &programCounter);
        void op2NNN(uint16_t &programCounter, std::stack<uint16_t> &stack);
        void op3KKK(uint16_t &programCounter, std::array<uint8_t, 16> &registers);
        void op4XKK(uint16_t &programCounter, std::array<uint8_t, 16> &registers);
        void op5XY0(uint16_t &programCounter, std::array<uint8_t, 16> &registers);
        void op6XKK(std::array<uint8_t, 16> &registers);
        void op7XKK(std::array<uint8_t, 16> &registers);
        void op8XYN(std::array<uint8_t, 16> &registers);
        void op9XY0(uint16_t &programCounter, std::array<uint8_t, 16> &registers);
        void opANNN(uint16_t &indexRegister);
        void opBNNN(uint16_t &programCounter, std::array<uint8_t, 16> &registers);
        void opCXNN(std::array<uint8_t, 16> &registers);

        void opDXYN(
            std::array<uint8_t, 16> &registers, std::array<uint8_t, 64 * 32> &graphics, 
            std::array<uint8_t, 4096> &memory, uint16_t indexRegister
        );

        void opEXNN(
            uint16_t &programCounter, std::array<uint8_t, 16> &registers, 
            std::unordered_map<sf::Keyboard::Key, uint8_t> &keyMapping, sf::Keyboard::Key currentKey
        );
            
        void opFXNN(
            uint16_t &programCounter, uint8_t delayTimer, uint8_t soundTimer, 
            std::array<uint8_t, 16> &registers, std::array<uint8_t, 4096> &memory, 
            std::array<bool, 16ULL> &keyStates, uint16_t &indexRegister
        );
    private:
        uint16_t opcode;
        std::random_device seed;
		std::default_random_engine rnGen;
};
