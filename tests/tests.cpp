#include "../src/solver.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

/*
 * Структура конфига тестов
 * dirs     - директории с тестами
 * test_cnt - число тестов в директории
 * */
struct TCfg {
    std::vector<std::string> dirs;
    std::vector<uint32_t>    test_cnt;
};

/*
 * Считывает конфиг
 * */
TCfg readCfg() {
    std::ifstream cfg_file("tests.cfg");
    TCfg cfg;

    std::string line;
    while (getline(cfg_file, line)) {
        std::stringstream cfg_stream(line);
        std::string header;
        std::string item;
        cfg_stream >> header;
        while(cfg_stream >> item) {
            if (header == "dirs:") {
                cfg.dirs.push_back(item);
            } else { // test_count:
                cfg.test_cnt.push_back(std::stoi(item));
            }
        }
    }

    cfg_file.close();
    return cfg;
}

/*
 * Создает путь до файла с тестом по директории и номеру теста
 * Например "base/01"
 * */
std::string createFileName(std::string dir, uint32_t test_num) {
    std::string result = dir + "/";
    if (test_num < 10) {
        result += "0";
    }
    result += std::to_string(test_num);
    return result;
}

/*
 * Запускает solver на тесте из test_file
 * */
std::string runTest(const std::string& test_file) {
    std::ifstream file(test_file);
    TSolver solver;
    const auto data = solver.readData(file);
    file.close();
    const auto result = solver.solve(data);
    return solver.resultToString(result);
}

/*
 * Возвращает ответ на тест из test_file
 * */
std::string readTestResult(const std::string& test_file) {
    std::ifstream file(test_file);
    std::string result;
    getline(file, result);
    file.close();
    return result;
}

/*
 * Запускает solver на всех тестах, описанных в конфиге.
 * */
void runTests(const TCfg& cfg) {
    uint32_t total_test_count = 0;
    uint32_t correct_tests    = 0;

    for (uint8_t dir_num = 0; dir_num < cfg.dirs.size(); ++dir_num) {
        const auto& dir = cfg.dirs[dir_num];
        for (uint32_t test_num = 1; test_num <= cfg.test_cnt[dir_num]; ++test_num) {
            const auto test_file = createFileName(dir, test_num);
            const auto result = runTest(test_file + ".in");
            const auto correct_result = readTestResult(test_file + ".out");
            
            if (result == correct_result) {
                ++correct_tests;
            }

            const std::string test_result = result == correct_result ? "OK" : "FAIL";
            std::cout << "Test " << test_file << ": " << test_result << "\n";
            std::cout << "Result:         " << result << "\n";
            std::cout << "Correct result: " << correct_result << "\n";
        }
        total_test_count += cfg.test_cnt[dir_num];
    }

    for (uint8_t i = 0; i < 25; ++i) {
        std::cout << '=';
    }
    std::cout << "\nTotal: " << correct_tests << "/" << total_test_count << "\n";
}

int main() {
    const auto cfg = readCfg();
    runTests(cfg);
}
