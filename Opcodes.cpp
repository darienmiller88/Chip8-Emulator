#include "Opcodes.hpp"

Opcodes::Opcodes(){
    opcode = 0x200;
}

void Opcodes::setOpcode(uint16_t opcode){
    this->opcode = opcode;
}

void Opcodes::op0E00(std::array<uint8_t, 64 * 32> &graphics, uint16_t &programCounter, std::stack<uint16_t> &stack){
    if (opcode == 0x00E0){//Clear the graphics array
        graphics.fill(0);
    }else if(opcode == 0x00EE){
        programCounter = stack.top();//Set the program counter to the address at the top of the stack.
        stack.pop();
        // std::cout << "program counter in instruction 0x0: " << programCounter << "\n";
    }       
}

void Opcodes::op00EE(){

}

void Opcodes::op1NNN(){

}

void Opcodes::op2NNN(){

}

void Opcodes::op3KKK(){

}

void Opcodes::op4XKK(){

}

void Opcodes::op5XY0(){

}

void Opcodes::op6XKK(){

}

void Opcodes::op7XKK(){

}

void Opcodes::op8XYN(){

}

void Opcodes::op9XY0(){

}

void Opcodes::opANNN(){

}

void Opcodes::opBNNN(){

}

void Opcodes::opCXNN(){

}

void Opcodes::opDXYN(){

}

void Opcodes::opEXNN(){

}

void Opcodes::opFXNN(){

}