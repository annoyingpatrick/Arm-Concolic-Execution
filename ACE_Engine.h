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

    // Concolic
    void concolic();

private:
    // Computing Resources
    std::array<uint8_t, 2048> memory;
    std::array<int, 16> registers;
    std::vector<int> stack;
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
    
    int getRegisterNumber(const std::string& reg);

    // Per Program
    std::vector<Instruction> instructions;

    // Helper Tools
    // std::unordered_map<std::string, int> is_register_symbolic;
    std::unordered_set<std::string> validInstructions;
    std::unordered_map<std::string, int> symbol2index;
    std::unordered_map<std::string, int> reg2index;

    long long cmp_op1, cmp_op2;
    int cmp_op1_r, cmp_op2_r;
    
    int cmp_valid;
    bool terminated;
    bool isConcolic;

    /*************** Concolic ***************/
    z3::context ctx;
    /* Symbolic structures*/
    // Symbolic state
    z3::expr_vector symbolicMemory;            // symbolic memory
    z3::expr_vector symbolicRegisters;          // symbolic registers
    inline z3::expr getSymbolicRegister(const int &reg);
    void setSymbolicRegister(const int &reg, const z3::expr &expr);


    std::vector<int> inputRegisters;                                  // contain the 
    std::unordered_set<int> coverage;
    std::vector<int> isRegisterSymbolic;
    z3::expr_vector path_constraints;   
    z3::solver solver;                                      // contain
};

#endif ACE_ENGINE_H
