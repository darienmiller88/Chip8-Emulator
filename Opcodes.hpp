#pragma once
#include <array>
#include <stack>
#include <cstdint>

class Opcodes{
    public:
        Opcodes();
        void setOpcode(uint16_t opcode);
        void op0E00(std::array<uint8_t, 64 * 32> &graphics, uint16_t &programCounter, std::stack<uint16_t> &stack);
        void op00EE();
        void op1NNN();
        void op2NNN();
        void op3KKK();
        void op4XKK();
        void op5XY0();
        void op6XKK();
        void op7XKK();
        void op8XYN();
        void op9XY0();
        void opANNN();
        void opBNNN();
        void opCXNN();
        void opDXYN();
        void opEXNN();
        void opFXNN();
    private:
        uint16_t opcode;
};
