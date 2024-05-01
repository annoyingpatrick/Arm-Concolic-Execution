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

#include <z3++.h>

#include "helpers.h"


// Declare the ACE_Engine class

class ACE_Engine
{
public:
    struct Operand
    {
        std::vector<std::string> elements;
        std::string getString() const;
        bool write_back = false;
    };
    struct Instruction
    {
        std::string type; // ex MOV
        std::vector<Operand> operands; // rest operands
        void print() const;
    };


    ACE_Engine();

    bool loadProgram(std::string path);

    bool isInstructionValid(std::string instruction);
    void execute();
    int evaluateOperand(const Operand& operand);
    void executeInstruction(const Instruction& instruction);
    void printRegisters() const;
    uint8_t readByte(uint32_t address) const;
    uint32_t readWord(uint32_t address) const;
    void writeByte(uint32_t address, uint8_t value);
    void writeWord(uint32_t address, uint32_t value);

    


    // Concolic
    void concolic(const std::string &output_path);

private:
    // Computing Resources
    std::array<uint8_t, 2048> memory;
    std::array<int, 16> registers;
    std::unordered_map<char, int> CPRS;
    int PC;

    std::array<uint8_t, 2048> old_memory;
    std::array<int, 16> old_registers;
    std::vector<int> old_stack;
    std::unordered_map<char, int> old_CPRS;
    int old_PC;

    // Helper function
    void resetProcState();
    void saveProcState();
    void revertProcState();

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
    bool isConcolic;

    /* Concolic */
    // Symbolic state
    //std::array<z3::expr, 2048> symbolicMemory;            // symbolic memory
    //std::array<z3::expr, 16> symbolicRegisters;          // symbolic registers
    //std::unordered_map<char, z3::expr> symbolicCPRS;     // symbolic condition codes
    //std::vector<z3::expr> symbolicStack;                // symbolic stack

    int inputRegisters[4];
    //std::vector<z3::expr> path_constraints;     //path
    //z3::context ctx;
    //z3::solver solver;


};

#endif ACE_ENGINE_H
