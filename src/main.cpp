#include "solver.h"

#include <iostream>

void solve() {
    TSolver solver;
    const auto data = solver.readData(std::cin);
    const auto result = solver.solve(data);
    std::cout << solver.resultToString(result) << '\n';
}

int main() {
    solve();
}