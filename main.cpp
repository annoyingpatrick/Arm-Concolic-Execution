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


    // Create interpreter, and then execute code
    ACE_Engine myEngine;
    print_header("loading program: " + std::string(argv[1]));
    if (!myEngine.loadProgram(argv[1])) {

        return -1;
    }
    // print_header("Executing program");
    // myEngine.execute();
    // print_header("returning");

    print_header("Concolically Executing program");
    myEngine.concolic();
    print_header("returning");

    // random z3 tester
    //z3tester();

    return 0;
}
