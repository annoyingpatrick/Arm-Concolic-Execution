//#include <z3++.h>
#include "ACE_Engine.h"

/*
*   Interpreter Class
*
*/

const std::unordered_set<std::string> arith = {"add", "sub", "mul", "div", "lsl", "lsr", "and", "orr", "eor"};

ACE_Engine::ACE_Engine()
{
    resetProcState();
    // set the const tools
    reg2index =
    {
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

    validInstructions = {
        "mov", "add", "bl", "bx", "push", "pop",
        "sub", "mul", "div", "orr", "and", "eor",
        "lsl", "lsr", "cmp", "bne", "beq", "bge",
        "blt", "bgt", "ble", "b", "ldr", "str",
        "ldrb", "strb", "ldm", "stm", "mvn",
        "ace", "err", "out"
    };
}


/*
 * Check if instruction is valid
 *
 *
 */
bool ACE_Engine::isInstructionValid(std::string instruction)
{
    // Split the instruction into parts
    std::vector<std::string> parts = split(instruction, ' ');

    // Check the instruction type
    std::string type = parts[0];


    if (validInstructions.find(type) != validInstructions.end())
        return true;

    // if (type == "mov" || type == "add" || type == "bl" || type == "bx" || type == "push" || type == "pop" || type == "sub" || type == "mul" || type == "div" || type == "orr" || type == "and" || type == "eor" || type == "lsl" || type == "lsr" || type == "cmp" || type == "bne" || type == "beq" || type == "bge" || type == "blt" || type == "bgt" || type == "ble" || type == "b" || type == "ldr" || type == "str")
    // {
    //     // These are valid instructions
    //     return true;
    // }
    if (type[type.size() - 1] == ':')
    {
        // This is a label
        if (parts.size() > 1)
        {
            // This is an invalid label
            return false;
        }
        else
        {
            // Add the label to the symbol2index map
            symbol2index[type.substr(0, type.size() - 1)] = instructions.size();
            return false;
        }
    }
    else if (type[0] == '.')
    {
        // This is a valid directive
        return false;
    }
    else
    {
        // Unknown instruction
        return false;
    }
    print_debug("IDK", "Idk how we got here wbut we should fix lol");
    return true;
}


/*
* Execute code file
*
*
*
*/
void ACE_Engine::execute()
{
    terminated = false;
    resetProcState();

    // execute each line
    // for (auto& line : code)
    // {
    //     // Previous preprocessing here
    //     if (line.empty())
    //         continue;
    //
    //     // Parse the instruction and add it to the instructions vector
    //     std::istringstream iss(line);
    //     std::vector<std::string> tokens{
    //         std::istream_iterator<std::string>{iss},
    //         std::istream_iterator<std::string>{}
    //     };
    //     if (!tokens.empty() && isInstructionValid(tokens[0]))
    //     {
    //         // clean the operands
    //         for (int i = 1; i < tokens.size(); i++)
    //         {
    //             if (tokens[i].back() == '}' || tokens[i].back() == ',' || tokens[i].back() == ']')
    //             {
    //                 tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
    //             }
    //             if (tokens[i][0] == '{' || tokens[i][0] == '[')
    //             {
    //                 tokens[i] = tokens[i].substr(1);
    //             }
    //         }
    //         instructions.push_back({tokens[0], std::vector<std::string>(tokens.begin() + 1, tokens.end())});
    //     }
    // }

    // print the instructions
    std::cout << "Instructions" << std::endl;
    int cnt = 0;
    for (auto& instruction : instructions)
    {
        std::cout << cnt++ << ": " << instruction.type << " ";
        for (auto& operand : instruction.operands)
        {
            std::cout << operand.getString() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // print the symbol2index
    std::cout << "Symbol to index mapping" << std::endl;
    for (auto& [symbol, index] : symbol2index)
    {
        std::cout << symbol << " " << index << std::endl;
    }
    std::cout << std::endl;

    /*
     *
     * Execute parsed instructions
     *
     *
    */
    std::cout << std::endl;
    std::cout << "Executing instructions" << std::endl;
    PC = symbol2index["main"];


    while (PC < instructions.size() && PC >= 0)
    {
        std::cout << "PC: " << PC << " ";
        executeInstruction(instructions[PC]);
        if (terminated)
        {
            std::cout << "TERMINATED";
            break;
        }
    }
    std::cout << std::endl;
}

/*
* return ther first element of the operand
*/
std::string ACE_Engine::Operand::getString() const
{
    if (elements.size() == 0) {
        std::cout << "This must be an ERROR" << std::endl;
        return "";
    }
    std::string ret = "";
    for (int i = 0; i < elements.size(); i++) {
        if (i > 0)
            ret += ',';
        ret += elements[i];
    }
    return ret;
}

/*
* Evaluate an operand
*/

int ACE_Engine::evaluateOperand(const Operand& Op)
{
    // Literal value
    if (Op.elements.size() == 1) {
        if (Op.elements[0][0] == '#') {
            return std::stoi(Op.elements[0].substr(1));
        }
        else if (reg2index.find(Op.elements[0]) != reg2index.end()) {
            return registers[reg2index[Op.elements[0]]];
        }
        else {
            std::cout << "Error: Invalid operand" << std::endl;
            return -1;
        }
    } else if (Op.elements.size() == 2) {
        int val1, val2;

        if (Op.elements[0][0] == '#') {
            val1 = std::stoi(Op.elements[0].substr(1));
        } else if (reg2index.find(Op.elements[0]) != reg2index.end()) {
            val1 = registers[reg2index[Op.elements[0]]];
        } else {
            std::cout << "Error: Invalid operand" << std::endl;
            return -1;
        }

        if (Op.elements[1][0] == '#') {
            val2 = std::stoi(Op.elements[1].substr(1));
        } else if (reg2index.find(Op.elements[1]) != reg2index.end()) {
            val2 = registers[reg2index[Op.elements[1]]];
        } else {
            std::cout << "Error: Invalid operand" << std::endl;
            return -1;
        }
        if (Op.write_back) {
            // Op.elements[0] has to be a register
            registers[reg2index[Op.elements[0]]] = val1 + val2;
        }
        return val1 + val2;
    } else {
       std:: cout << "Error: Invalid operand" << std::endl;
        return -1;
    }
}

/*
* Print an instruction
*/

void ACE_Engine::Instruction::print() const
{
    std::cout << type << " ";
    for (auto& operand : operands)
    {
        std::cout << operand.getString() << " ";
    }
    std::cout << std::endl;
}

/*
*  Execute an instruction
*
*
*/
void ACE_Engine::executeInstruction(const Instruction& instruction)
{
    //std::cout << "EXECUTING: " << instruction.type << std::endl;
    //std::string x = instruction.operands[1];
    //int g = getOperandValue(x);
    const std::string opcode = instruction.type;


    /*
     *
     * Special operations = {ERR, ACE, OUT}
     *
    */
    // terminate instruction
    if (opcode == "err")
    {
        /*  Terminate */
        terminated = true;
        return;
    }
    // make register symbolic
    else if (opcode == "ace")
    {
        //is_register_symbolic[instruction.operands[0]] = 1;
    }
    // output a register to std out
    else if (opcode == "out")
    {
        std::cout << "OUT " << instruction.operands[0].getString() << " == value ==> " << registers[reg2index[instruction.operands[0].getString()]] << std::endl;
    }
    // Arithmetic operations = {ADD, SUB, MUL, DIV} + {LSL LSR} + {AND, ORR, EOR}
    else if (arith.find(opcode) != arith.end())
    {
        int op1 = evaluateOperand(instruction.operands[1]);
        int op2 = evaluateOperand(instruction.operands[2]);
        instruction.print();

        if (opcode == "add")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 + op2;
        }
        else if (instruction.type == "sub")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 - op2;
        }
        else if (instruction.type == "mul")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 * op2;
        }
        else if (instruction.type == "div")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 / op2;
            //std::cout << "DIV " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << std::endl;
            std::cout << "DIV" << std::endl;
        }
        else if (instruction.type == "orr")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 | op2;
            //std::cout << "ORR " << instruction.operands[0] << " " << instruction.operands[1] << " " << instruction.operands[2] << std::endl;
            std::cout << "ORR" << std::endl;
        }
        else if (instruction.type == "and")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 & op2;
        }
        else if (instruction.type == "eor")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 ^ op2;
        }
        else if (instruction.type == "lsl")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 << op2;
        }
        else if (instruction.type == "lsr")
        {
            registers[reg2index[instruction.operands[0].getString()]] = op1 >> op2;
        }
    }
    // Branching = {B, BX, BL, BNE, BEQ, BGE, BLT, BGT, BLE}
    else if (opcode[0] == 'b')
    {
        instruction.print();
        if (opcode == "b")
        {
            // Similar handling for b
            PC = symbol2index[instruction.operands[0].getString()];
            return;
        }

        else if (instruction.type == "bne")
        {
            // Similar handling for bne
            if (cmp_valid && CPRS['Z'] == 0)
            {
                PC = symbol2index[instruction.operands[0].getString()];
                return;
            }
        }
        else if (instruction.type == "beq")
        {
            // Similar handling for beq
            if (cmp_valid && CPRS['Z'] == 1)
            {
                PC = symbol2index[instruction.operands[0].getString()];
                return;
            }
        }
        else if (instruction.type == "bge")
        {
            // Similar handling for bge
            instruction.print();
            if (cmp_valid && CPRS['Z'] == 0 && CPRS['N'] == CPRS['V'])
            {
                PC = symbol2index[instruction.operands[0].getString()];
                return;
            }
        }
        else if (instruction.type == "blt")
        {
            // Similar handling for blt
            if (cmp_valid && CPRS['N'] != CPRS['V'])
            {
                PC = symbol2index[instruction.operands[0].getString()];
                return;
            }
        }
        else if (instruction.type == "bgt")
        {
            // Similar handling for bgt
            if (cmp_valid && CPRS['Z'] == 0 && CPRS['N'] == CPRS['V'])
            {
                PC = symbol2index[instruction.operands[0].getString()];
                return;
            }
        }
        else if (instruction.type == "ble")
        {
            // Similar handling for ble
            if (cmp_valid && CPRS['Z'] == 1 && CPRS['N'] != CPRS['V'])
            {
                PC = symbol2index[instruction.operands[0].getString()];
                return;
            }
        }
        else if (instruction.type == "bx")
        {
            // simply set the PC to the value in the lr register
            PC = registers[14];
            return;
        }
        else if (instruction.type == "bl")
        {
            // Similar handling for bl
            registers[14] = PC + 1;
            PC = symbol2index[instruction.operands[0].getString()];
            return;
        }
    }
    // Register = {MOV}
    else if (instruction.type == "mov")
    {
        int val = evaluateOperand(instruction.operands[1]);
        registers[reg2index[instruction.operands[0].getString()]] = val;
        instruction.print();
    }
    else if (instruction.type == "mvn")
    {
        int val = evaluateOperand(instruction.operands[1]);
        registers[reg2index[instruction.operands[0].getString()]] = ~val;
        instruction.print();
    }
    // Stack = {PUSH, POP}
    else if (instruction.type == "push")
    {
        // Similar handling for push
        instruction.print();

        // decrement then push onto the stack.
        for (auto& operand : instruction.operands)
        {
            registers[13] -= 4;
            stack.push_back(registers[reg2index[operand.getString()]]);
            // decrease the stack pointer, but we are not actually pushing the values to the stack
            
        }
    }
    else if (instruction.type == "pop")
    {
        // Similar handling for pop
        instruction.print();

        // pop from the stack then increment stack pointer
        for (int i = instruction.operands.size() - 1; i >= 0; i--)
        {
            registers[reg2index[instruction.operands[i].getString()]] = stack.back();
            stack.pop_back();
            registers[13] += 4;
        }
    }
    // Comparison = {CMP}
    else if (instruction.type == "cmp")
    {
        // Similar handling for cmp
        instruction.print();
        int op1 = evaluateOperand(instruction.operands[0]);
        int op2 = evaluateOperand(instruction.operands[1]);

        cmp_op1 = op1;
        cmp_op2 = op2;
        cmp_valid = 1;
        // update CPRS
        CPRS['N'] = (op1 - op2) < 0;
        CPRS['Z'] = (op1 - op2) == 0;
        CPRS['C'] = op1 >= op2;
        CPRS['V'] = (op1 < 0 && op2 >= 0 && (op1 - op2) >= 0) || (op1 >= 0 && op2 < 0 && (op1 - op2) < 0);

    }
    // Load and store = {LDR, STR}
    else if (instruction.type == "ldr")
    {
        // Similar handling for ldr
        instruction.print();
        int address = evaluateOperand(instruction.operands[1]);
        // read 4 bytes from the memory
        registers[reg2index[instruction.operands[0].getString()]] =
            (memory[address] << 24) | (memory[address + 1] << 16) | (memory[address +
                2] << 8) | memory[
                address + 3];
    }
    else if (instruction.type == "ldrb")
    {
        // Similar handling for ldr
        instruction.print();
        int address = evaluateOperand(instruction.operands[1]);
        // read 4 bytes from the memory
        registers[reg2index[instruction.operands[0].getString()]] = memory[address] & 0xFF;
    }
    else if (instruction.type == "ldm")
    {
        instruction.print();
        int base = evaluateOperand(instruction.operands[0]);
        for (int i = 1; i < instruction.operands.size(); i++) {
            registers[reg2index[instruction.operands[i].getString()]] =
                (memory[base] << 24) | (memory[base + 1] << 16) | (memory[base +
                    2] << 8) | memory[
                    base + 3];
            base += 4;
        }
    }
    else if (instruction.type == "str")
    {
        // Similar handling for str
        instruction.print();
        int address = evaluateOperand(instruction.operands[1]);
        //memory[address] = registers[reg2index[instruction.operands[0]]];
        // store 4 bytes to the memory
        memory[address] = (registers[reg2index[instruction.operands[0].getString()]] >> 24) & 0xFF;
        memory[address + 1] = (registers[reg2index[instruction.operands[0].getString()]] >> 16) & 0xFF;
        memory[address + 2] = (registers[reg2index[instruction.operands[0].getString()]] >> 8) & 0xFF;
        memory[address + 3] = (registers[reg2index[instruction.operands[0].getString()]]) & 0xFF;
    }
    else if (instruction.type == "strb")
    {
        // Similar handling for str
        instruction.print();
        int address = evaluateOperand(instruction.operands[1]);
        //memory[address] = registers[reg2index[instruction.operands[0]]];
        // store 4 bytes to the memory
        memory[address] = registers[reg2index[instruction.operands[0].getString()]] & 0xFF;
    }
    else if (instruction.type == "stm")
    {
        instruction.print();
        int base = evaluateOperand(instruction.operands[0]);
        for (int i = 1; i < instruction.operands.size(); i++) {
            // TODO: make this a function
            memory[base] = (registers[reg2index[instruction.operands[i].getString()]] >> 24) & 0xFF;
            memory[base + 1] = (registers[reg2index[instruction.operands[i].getString()]] >> 16) & 0xFF;
            memory[base + 2] = (registers[reg2index[instruction.operands[i].getString()]] >> 8) & 0xFF;
            memory[base + 3] = (registers[reg2index[instruction.operands[i].getString()]]) & 0xFF;
            base += 4;
        }
    }
    else
    {
        std::cout << "Unknown instruction" << std::endl;
    }
    //Add other instructions as needed
    PC++;
}

void ACE_Engine::printRegisters() const
{
    for (int i = 0; i < registers.size(); ++i)
    {
        std::cout << "R" << i << ": " << registers[i] << std::endl;
    }
}
 
void ACE_Engine::resetProcState()
{
    memset(memory.data(), 0, memory.size()); //reset memory
    registers.fill(0); //reset registers
    registers[13] = memory.size();      // top of' stack (stack pointer points at first item on stack)
    registers[14] = -1; // link registes
    PC = 0; // Program counter == R15
    stack.clear();
    terminated = false;

    // Set the condition
    CPRS['N'] = 0; // negative
    CPRS['Z'] = 0; // zero
    CPRS['C'] = 0; // carry
    CPRS['V'] = 0; // overflow
    cmp_valid = 0;
}

bool ACE_Engine::loadProgram(std::string path)
{
    // get the file name from command line arg
    // open a file in read mode. and read it line by line.
    std::vector<std::string> code;
    std::ifstream file(path);
    std::string line;
    while (getline(file, line))
    {
        code.push_back(line);
    }

    for (auto& line : code)
    {
        // Previous preprocessing here
        if (line.empty())
            continue;

        // Parse the instruction and add it to the instructions vector
        std::istringstream iss(line);
        std::vector<std::string> tokens{
            std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}
        };
        if (tokens.empty() || !isInstructionValid(tokens[0]) || tokens[0][0]=='@' || tokens[0][0]=='/')
            continue;
            // FIX HERE
        Instruction instr;
        if (tokens[0] == "push" || tokens[0] == "pop") {
            for (int i = 1; i < tokens.size(); i++) {
                Operand op;
                if (tokens[i][0] == '{' || tokens[i][0] == '[') {
                    tokens[i] = tokens[i].substr(1);
                }
                if (tokens[i].back() == '}' || tokens[i].back() == ',' || tokens[i].back() == ']') {
                    tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
                }
                op.elements.push_back(tokens[i]);
                instr.operands.push_back(op);
            }
        } else if (tokens[0] == "ldm" || tokens[0] == "stm") {
            for (int i = 1; i < tokens.size(); i++) {
                Operand op;
                if (tokens[i][0] == '{' || tokens[i][0] == '[') {
                    tokens[i] = tokens[i].substr(1);
                }
                if (tokens[i].back() == '}' || tokens[i].back() == ',' || tokens[i].back() == ']') {
                    tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
                }
                op.elements.push_back(tokens[i]);
                instr.operands.push_back(op);
            }
        } else {// Other instructions
            Operand op;
            bool isInBrackets = false;
            for (int i = 1; i < tokens.size(); ++i) {
                if (tokens[i].back() == ',') {
                    tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
                }
                if (tokens[i][0] == '[' && tokens[i].back() == ']') {
                    tokens[i] = tokens[i].substr(1);
                    tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
                    op.elements.push_back(tokens[i]);
                } else if (tokens[i][0] == '[') {
                    tokens[i] = tokens[i].substr(1);
                    op.elements.push_back(tokens[i]);
                    isInBrackets = true;
                } else if (tokens[i].back() == ']') {
                    tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
                    op.elements.push_back(tokens[i]);
                    instr.operands.push_back(op);
                    op.elements.clear();
                    isInBrackets = false;
                } else if (tokens[i].back() == '!') {
                    tokens[i] = tokens[i].substr(0, tokens[i].size() - 2); // Remove ']!'
                    op.elements.push_back(tokens[i]);
                    op.write_back = true;
                    instr.operands.push_back(op);
                    isInBrackets = false;
                } else {
                    op.elements.push_back(tokens[i]);
                    if (!isInBrackets) {
                        instr.operands.push_back(op);
                        op.elements.clear();
                    }
                }
            }
        
        }    
            // clean the operands
        // Keep tokenp[0] as type, Update the rest as Operand
        instr.type = tokens[0];
        instructions.push_back(instr);
        //instructions.push_back({tokens[0], std::vector<std::string>(tokens.begin() + 1, tokens.end())});
    }
    std::cout << "instructions size: " << instructions.size() << std::endl;
    return true;
}


/*
*   Takes string input (operand), and returns the value it represents
*
*/
/*
int getOperandValue(std::string &op)
{
    std::cout << "GETOP "  << op << "\n";

    // Literal value
    if(op[0] == '#'){
        std::cout << op.substr(1);
        return stoi(op.substr(1));

    }

    // Register value
    if(op[0] > 'a' && op[0] < 'z')
    {
        int x = registers[reg2index[op]];
        std::cout <<x;
        return  registers[reg2index[op]];
    }

    // Addressing modes
    if(op[0] == '[')
    {
        std::vector<std::string> toks = split(op.substr(1, op.length()-2), ',');
        std::cout << "ADDR : <";
        for(auto t: toks)
            std::cout <<"|"<<t<<"|" << " ";
        std::cout << ">\n";


        // register indirect addressing
        if(toks.size() == 1){
            return memory[registers[reg2index[toks[0]]]];
        }

    }

    return -1;
}
*/
