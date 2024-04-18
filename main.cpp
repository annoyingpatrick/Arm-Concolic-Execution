//
// Created by karn on 4/16/24.
//
#include "ACE_Engine.h"
//#include <z3++.h>


int main(int argc, char* argv[])
{
    // Make sure we have program to analyze
    if (argc > 1)
    {
        std::cout << "First argument: " << argv[1] << std::endl;
    }
    else
    {
        std::cout << "No arguments provided" << std::endl;
        return 1;
    }

    // get the file name from command line arg
    // open a file in read mode. and read it line by line.
    // std::vector<std::string> code;
    // std::ifstream file(argv[1]);
    // std::string line;
    // while (getline(file, line))
    // {
    //     code.push_back(line);
    // }

    // Create interpreter, and then execute code
    print_debug("", "myengine");
    ACE_Engine myEngine;
    print_debug("","loading");
    if (!myEngine.loadProgram(argv[1])) {

        return -1;
    }
    print_debug("", "executing");
    myEngine.execute();
    print_debug("", "returning");

    // random z3 tester
    //z3tester();

    return 0;
}
