#include "solver.h"

#include <iostream>

void solve() {
    TSolver solver;
    const auto data = solver.readData(std::cin);
    std::cout << solver.solve(data);
}

int main() {
    solve();
}