//
// Created by karn on 4/16/24.
//

#ifndef ACE_H
#define ACE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <iostream>
#include <sstream>
#include <iterator>




/*
* Instruction Struct
*
*/
struct Instruction
{
    std::string type;               // ex MOV
    int opcode;                     // ex MOV --> 2
    std::vector<std::string> operands; // rest operands


    // void clean() {
    //     for (auto& operand : operands) {
    //         if (operand[0] == '{') {
    //             operand = operand.substr(1);
    //         }
    //     }
    // }
};


// Declare the ACE_Engine class
class ACE_Engine {
public:
    ACE_Engine();
    bool valid(std::string instruction);
    void execute(const std::vector<std::string>& code);
    void executeInstruction(const Instruction& instruction);
    void printRegisters() const;

private:
    std::array<uint8_t, 2048> memory;
    std::array<int, 16> registers;
    std::vector<int> stack;
    int PC;
    std::vector<Instruction> instructions;
    std::unordered_map<std::string, int> is_register_symbolic;
    std::unordered_map<std::string, int> symbol2index;
    const std::unordered_set<std::string> validInstructions;
    std::unordered_map<std::string, int> reg2index;
    std::unordered_map<char, int> CPRS;
    long long cmp_op1, cmp_op2;
    int cmp_valid;
    bool terminated;
};

#endif //ACE_H
