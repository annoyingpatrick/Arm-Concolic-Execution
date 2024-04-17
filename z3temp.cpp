//
// Created by karn on 4/17/2024.
//

//#include <z3++.h>

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