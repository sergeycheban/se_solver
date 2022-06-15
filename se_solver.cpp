#include "se_solver_solver.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    try {
        if (((std::size_t)(argc - 1)) % 3) {
            std::cerr << "Unexpected number of arguments" << std::endl;
            return 1;
        }

        std::size_t taskCount = ((std::size_t)(argc - 1)) / 3;
        std::vector<std::string> results(taskCount);

        std::vector<Solver> solvers(std::min(
            std::thread::hardware_concurrency(), 1U));

        for (std::size_t i = 0; i < taskCount; ++i) {
            int a = std::stoi(argv[3 * i + 1], nullptr, 10);
            int b = std::stoi(argv[3 * i + 2], nullptr, 10);
            int c = std::stoi(argv[3 * i + 3], nullptr, 10);
            solvers[i % solvers.size()].addTask(&results[i], a, b, c);
        }

        for (auto&& solver : solvers) {
            solver.wait();
        }

        for (auto&& result : results) {
            std::cout << result << '\n';
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;
    }
}

