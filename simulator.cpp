#include <iostream>
#include <array>
#include <cstring> // For memset
#include <sstream>
#include <vector>
#include <regex>
#include <fstream>
#include <z3++.h>

using namespace std;

void z3tester() {
    z3::context c;
    z3::solver s(c);

    // Define variables
    z3::expr x = c.int_const("x");
    z3::expr y = c.int_const("y");

    // Add constraints to the solver
    s.add(x > 2);
    s.add(y < 10);
    s.add(x + 2 * y == 7);

    // Attempt to solve the problem
    auto result = s.check();
    if (result == z3::sat) {
        std::cout << "Solution found:\n";
        z3::model m = s.get_model();
        std::cout << "x = " << m.eval(x) << "\n";
        std::cout << "y = " << m.eval(y) << "\n";
    } else if (result == z3::unsat) {
        std::cout << "No solution exists.\n";
    } else {
        std::cout << "Could not determine if a solution exists.\n";
    }
}
  


std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

struct Instruction {
    std::string type;
    std::vector<std::string> operands;
    void clean() {
        for (auto& operand : operands) {
            if (operand[0] == '{') {
                operand = operand.substr(1);
            }
        }
    }
};

class ARMInterpreter {
public:
    ARMInterpreter() {
        memset(memory.data(), 0, memory.size());
        registers.fill(0);
        // Set the stack pointer to the end of the memory for simplicity
        registers[13] = memory.size();
        registers[14] = -1;
        PC = 0;
        instructions.clear();
        stack.clear();
        CPRS['N'] = 0;
        CPRS['Z'] = 0;
        CPRS['C'] = 0;
        CPRS['V'] = 0;
        cmp_valid = 0;
    }
    bool valid(std::string instruction) {
    // Split the instruction into parts
    std::vector<std::string> parts = split(instruction, ' ');

    // Check the instruction type
    std::string type = parts[0];

    if (type == "mov" || type == "add" || type == "bl" || type == "bx" || type == "push" || type == "pop" || type == "sub" || type == "mul" || type == "div" || type == "orr" || type == "and" || type == "eor" || type == "lsl" || type == "lsr" || type == "cmp" || type == "bne" || type == "beq" || type == "bge" || type == "blt" || type == "bgt" || type == "ble" || type == "b" || type == "ldr" || type == "str") {
        // These are valid instructions
        return true;
    } else if (type[type.size() - 1] == ':') {
        // This is a label
        if (parts.size() > 1) {
            // This is an invalid label
            return false;
        } else {
            // Add the label to the symbol2index map
            symbol2index[type.substr(0, type.size() - 1)] = instructions.size();
            return false;
        } 
    } else if (type[0] == '.') {
        // This is a valid directive
        return false;
    } else {
        // Unknown instruction
        return false;
    }
}
    void execute(const vector<string>& code) {

        for (auto& line : code) {
            // Previous preprocessing here
            if (line.empty()) continue;

            // Parse the instruction and add it to the instructions vector
            std::istringstream iss(line);
            std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                            std::istream_iterator<std::string>{}};
            if (!tokens.empty() && valid(tokens[0])) {
                // clean the operands
                for (int i = 1; i < tokens.size(); i++) {
                    if (tokens[i].back() == '}' || tokens[i].back() == ',' || tokens[i].back() == ']') {
                        tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
                    }
                    if (tokens[i][0] == '{' || tokens[i][0] == '[') {
                        tokens[i] = tokens[i].substr(1);
                    }
                }
                instructions.push_back({tokens[0], std::vector<std::string>(tokens.begin() + 1, tokens.end())});
            }
        }

        // print the instructions
        cout << "Instructions" << endl;
        int cnt = 0;
        for (auto& instruction : instructions) {
            cout << cnt++ << ": "<<instruction.type << " ";
            for (auto& operand : instruction.operands) {
                cout << operand << " ";
            }
            cout << endl;
        }
        cout << endl;

        // print the symbol2index
        cout << "Symbol to index mapping" << endl;
        for (auto& [symbol, index] : symbol2index) {
            cout << symbol << " " << index << endl;
        }
        cout << endl;

        // Execute parsed instructions
        cout << endl;
        cout << "Executing instructions" << endl;
        PC = symbol2index["main"];

        while (PC < instructions.size() && PC >= 0) {
            cout << "PC: " << PC << " ";
            executeInstruction(instructions[PC]);
        }
        cout << endl;
    }

    void executeInstruction(const Instruction& instruction) {
        if (instruction.type == "mov") {
            int val;
            if (instruction.operands[1][0] == '#') {
                val = std::stoi(instruction.operands[1].substr(1));
            } else {
                val = registers[reg2index[instruction.operands[1]]];
            }
            registers[reg2index[instruction.operands[0]]] = val;
            cout << "MOV " << instruction.operands[0] << " " << instruction.operands[1] << endl;
        } else if (instruction.type == "push") {
            // Similar handling for push
            cout << "PUSH ";
            for (int i = 0; i < instruction.operands.size(); i++) {
                cout << instruction.operands[i] << " ";
            } 
            cout << endl;
            
            for (auto& operand : instruction.operands) {
                stack.push_back(registers[reg2index[operand]]);
                // decrease the stack pointer, but we are not actually pushing the values to the stack
                registers[13] -= 4;
            }
        } else if (instruction.type == "pop") {
            // Similar handling for pop
            cout << "POP ";
            for (int i = 0; i < instruction.operands.size(); i++) {
                cout << instruction.operands[i] << " ";
            }
            cout << endl;
            // pop from the stack, the top of the stack is the last element
            for (int i = instruction.operands.size() - 1; i >= 0; i--) {
                registers[reg2index[instruction.operands[i]]] = stack.back();
                stack.pop_back();
                registers[13] += 4;
            }    
        } else if (instruction.type == "bx") {
            cout << "BX " << instruction.operands[0] << endl;
            // simply set the PC to the value in the lr register
            PC = registers[14];
            return;
        } else if (instruction.type == "bl") {
            // Similar handling for bl
            cout << "BL " << instruction.operands[0] << endl;
            registers[14] = PC + 1;
            PC = symbol2index[instruction.operands[0]];
            return;
        } else if (instruction.type == "add") {
            // Similar handling for add
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 + op2;
            cout << "ADD " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "sub") {
            // Similar handling for sub
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 - op2;
            cout << "SUB " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "mul") {
            // Similar handling for mul
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 * op2;
            cout << "MUL " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "div") {
            // Similar handling for div
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 / op2;
            cout << "DIV " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "orr") {
            // Similar handling for orr
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 | op2;
            cout << "ORR " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "and") {
            // Similar handling for and
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 & op2;
            cout << "AND " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "eor") {
            // Similar handling for eor
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 ^ op2;
            cout << "EOR " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "lsl") {
            // Similar handling for lsl
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 << op2;
            cout << "LSL " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "lsr") {
            // Similar handling for lsr
            int op1 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            int op2 = (instruction.operands[2][0] == '#') ? std::stoi(instruction.operands[2].substr(1)) : registers[reg2index[instruction.operands[2]]];

            registers[reg2index[instruction.operands[0]]] = op1 >> op2;
            cout << "LSR " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << endl;
        } else if (instruction.type == "cmp") {
            // Similar handling for cmp
            int op1 = (instruction.operands[0][0] == '#') ? std::stoi(instruction.operands[0].substr(1)) : registers[reg2index[instruction.operands[0]]];
            int op2 = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];

            cmp_op1 = op1;
            cmp_op2 = op2;
            cmp_valid = 1;            
            // update CPRS
            CPRS['N'] = (op1 - op2) < 0;
            CPRS['Z'] = (op1 - op2) == 0;
            CPRS['C'] = op1 >= op2;
            CPRS['V'] = (op1 < 0 && op2 >= 0 && (op1 - op2) >= 0) || (op1 >= 0 && op2 < 0 && (op1 - op2) < 0);
            cout << "CMP " << instruction.operands[0] << " " << instruction.operands[1] << endl;
        } else if (instruction.type == "bne") {
            // Similar handling for bne
            cout << "BNE " << instruction.operands[0] << endl;
            if (cmp_valid && CPRS['Z'] == 0) {
                PC = symbol2index[instruction.operands[0]];
                return;
            }
        } else if (instruction.type == "beq") {
            // Similar handling for beq
            cout << "BEQ " << instruction.operands[0] << endl;
            if (cmp_valid && CPRS['Z'] == 1) {
                PC = symbol2index[instruction.operands[0]];
                return;
            }
        } else if (instruction.type == "bge") {
            // Similar handling for bge
            cout << "BGE " << instruction.operands[0] << endl;
            if (cmp_valid && CPRS['Z'] == 0 && CPRS['N'] == CPRS['V']) {
                PC = symbol2index[instruction.operands[0]];
                return;
            }
        } else if (instruction.type == "blt") {
            // Similar handling for blt
            cout << "BLT " << instruction.operands[0] << endl;
            if (cmp_valid && CPRS['N'] != CPRS['V']) {
                PC = symbol2index[instruction.operands[0]];
                return;
            }
        } else if (instruction.type == "bgt") {
            // Similar handling for bgt
            cout << "BGT " << instruction.operands[0] << endl;
            if (cmp_valid && CPRS['Z'] == 0 && CPRS['N'] == CPRS['V']) {
                PC = symbol2index[instruction.operands[0]];
                return;
            }
        } else if (instruction.type == "ble") {
            // Similar handling for ble
            cout << "BLE " << instruction.operands[0] << endl;
            if (cmp_valid && CPRS['Z'] == 1 && CPRS['N'] != CPRS['V']) {
                PC = symbol2index[instruction.operands[0]];
                return;
            }
        } else if (instruction.type == "b") {
            // Similar handling for b
            PC = symbol2index[instruction.operands[0]];
            cout << "B " << instruction.operands[0] << endl;
            return;
        } else if (instruction.type == "ldr") {
            // Similar handling for ldr
            cout << "LDR " << instruction.operands[0] << " " << instruction.operands[1] << endl;
            int address = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            // read 4 bytes from the memory
            registers[reg2index[instruction.operands[0]]] = (memory[address] << 24) | (memory[address + 1] << 16) | (memory[address + 2] << 8) | memory[address + 3];
        } else if (instruction.type == "str") {
            // Similar handling for str
            cout << "STR " << instruction.operands[0] << " " << instruction.operands[1] << endl;
            int address = (instruction.operands[1][0] == '#') ? std::stoi(instruction.operands[1].substr(1)) : registers[reg2index[instruction.operands[1]]];
            //memory[address] = registers[reg2index[instruction.operands[0]]];
            // store 4 bytes to the memory
            memory[address] = (registers[reg2index[instruction.operands[0]]] >> 24) & 0xFF;
            memory[address + 1] = (registers[reg2index[instruction.operands[0]]] >> 16) & 0xFF;
            memory[address + 2] = (registers[reg2index[instruction.operands[0]]] >> 8) & 0xFF;
            memory[address + 3] = (registers[reg2index[instruction.operands[0]]]) & 0xFF;
        } else {
            cout << "Unknown instruction" << endl;
        }
        //Add other instructions as needed
        PC++;
    }

    void printRegisters() const {
        for (int i = 0; i < registers.size(); ++i) {
            std::cout << "R" << i << ": " << registers[i] << std::endl;
        }
    }

private:
    std::array<uint8_t, 2048> memory;
    std::array<int, 16> registers;
    vector<int> stack;
    int PC;
    std::vector<Instruction> instructions;
    unordered_map<string, int> symbol2index;
    unordered_map<string, int> reg2index = {
        {"r0", 0},
        {"r1", 1},
        {"r2", 2},
        {"r3", 3},
        {"r4", 4},
        {"r5", 5},
        {"r6", 6},
        {"r7", 7},
        {"r8", 8},
        {"r9", 9},
        {"r10", 10},
        {"fp", 11},
        {"ip", 12},
        {"sp", 13},
        {"lr", 14},
        {"pc", 15}
    };
    unordered_map<char, int> CPRS;
    long long cmp_op1, cmp_op2; 
    int cmp_valid;
};

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::cout << "First argument: " << argv[1] << std::endl;
    } else {
        std::cout << "No arguments provided" << std::endl;
        return 1;
    }
    ARMInterpreter interpreter;
    
    vector<string> code;
    // get the file name from command line arg
    // open a file in read mode. and read it line by line.

    ifstream file(argv[1]);
    string line;

    while (getline(file, line)) {
        code.push_back(line);
    }
    
    interpreter.execute(code);
    z3tester();
    return 0;
    interpreter.execute(code4);
    interpreter.printRegisters();
    
    return 0;
}
