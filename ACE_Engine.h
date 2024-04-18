//
// Created by karn on 4/16/24.
//

#ifndef ACE_ENGINE_H
#define ACE_ENGINE_H

#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include "helpers.h"


// Declare the ACE_Engine class

class ACE_Engine
{
public:
    struct Instruction
    {
        std::string type; // ex MOV
        std::vector<std::string> operands; // rest operands
        //int opcode; // ex MOV --> 2
        // void clean() {
        //     for (auto& operand : operands) {
        //         if (operand[0] == '{') {
        //             operand = operand.substr(1);
        //         }
        //     }
        // }
    };

    ACE_Engine();

    bool loadProgram(std::string path);

    bool isInstructionValid(std::string instruction);
    void execute();
    void executeInstruction(const Instruction& instruction);
    void printRegisters() const;

private:
    // Computing Resources
    struct proc_state
    {
        std::array<uint8_t, 2048> memory;
        std::array<int, 16> registers;
        std::vector<int> stack;
        std::unordered_map<char, int> CPRS;

        int PC;
    } proc_state;

    // Helper function
    void resetProcState();

    // Per Program
    std::vector<Instruction> instructions;

    // Helper Tools
    // std::unordered_map<std::string, int> is_register_symbolic;
    std::unordered_set<std::string> validInstructions;
    std::unordered_map<std::string, int> symbol2index;
    std::unordered_map<std::string, int> reg2index;

    long long cmp_op1, cmp_op2;
    int cmp_valid;
    bool terminated;
};

#endif //ACE_ENGINE_H
