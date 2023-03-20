#pragma once
#include <cstdint>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <array>
#include <stack>
#include <string>
#include <random>

class Chip8{
    public:
        Chip8();

        //Method will take in the name of the Chip8 ROM to load into the emulator
        void loadRom(const std::string &romName);

        //The CPU cycle method will emulate one cycle of the machine in which the Chip-8 virtual machine would 
        //run on. It represents the four stages of a standard CPU cycle.
        //1. Fetch - The instruction, or in this case the opcode pointed to by the program counter in memory will 
        //be retrieved and decoded.
        //
        //2. Decode - After retrieving the opcode from memory, decode the opcode to see which one it is.
        //
        //3. Execute - Execute the instruction asccociated with each specific opcode.
        void cpuCycle();
        void handleKeyClick(const sf::Event &e);
        void handleKeyRelease(const sf::Event &e);

        //Drawing the spirtes, will add more comments later
        void drawSprites(sf::RenderWindow &window, float pixelWidth);
        
    private:
        bool drawFlag;

        const int CHIP8_WINDOW_WIDTH = 64, CHIP8_WINDOW_HEIGHT = 32;

        //Array to store which hex key is currently being pressed. Really only used for opcode 0xFx0A.
        std::array<bool, 16> keyStates;

        //memory - Memory to be used for the Chip-8 ROMs. According to the documentation, the original 
        //Chip-8 virtual machines ran on machines that only had 4KB of space, with each slot in memory holding
        //8 bits. The first 512 spots in the array, 0(0x00) - 511(0X1FF) were reserved for the intepreter itself, 
        //but since modern machines have far more than 4kb in memory, the first 512 spots can be used to store
        //the Chip-8 fonts.
        std::array<uint8_t, 4096> memory;        

        //registers - The Chip-8 emulator will make use of 16, 8-bit data registers to "V0" to "VF". VF is the 
        //carry flag and thus will not be used.
        std::array<uint8_t, 16> registers;

        //graphics - The array that will hold 2,048 8-bit numbers that will be used to draw sprites to the screen
        //Each spot will hold a 1 or 0 signaling whether or not a pixel should be drawn.
        std::array<uint8_t, 64 * 32> graphics;

        //stack - We need to implement a stack to keep track of the current location before a jump is performed.
        //The value of the programCounter will stored in the stack before jumping to another position in memory.
        std::stack<uint16_t> stack;

        //chip8Fontset - These values represent the fonts that will be used by Chip-8 ROMs to draw letters and 
        //numbers to the screen. These sprites are 8-bits wide, and 5-bits tall, and the '1' bit in each "row"
        //will represent a pixel to be drawn to the screen.
        std::array<uint8_t, 80> chip8Fontset;

        //keyMapping - This variable will map the keys on modern keys to the hex keys on older machines running
        //Chip-8 intrepreters. 
        std::unordered_map<sf::Keyboard::Key, uint8_t> keyMapping;

        std::unordered_map<char, uint8_t> charKeyMapping;

        //delayTimer - This timer is intended to be used for timing the events of games. Its value can be
        //set and read, which will be done during the execution phase.
        uint8_t delayTimer;

        //soundTimer - This timer is used for sound effects. When its value is nonzero, a beeping sound is made.
        uint8_t soundTimer;

        //indexRegister - this is a 16-bit value to store memory addresses. Only 12 bits will be used. 
        uint16_t indexRegister;

        //The program counter will be used to point at the memory index currently being used.
        uint16_t programCounter;

        //opcode - This variable is a 16-bit vlaue that will be used to store the opcodes retrieved from memory.
        uint16_t opcode;
        
        //rombufferSize - This will represent how big the rom is, i.e. How many slots in memory the it takes up.
        int rombufferSize;

        sf::Keyboard::Key currentKey;

        //C++ random number generation is atrocious lol.
        std::random_device seed;
		std::default_random_engine rnGen;
};
