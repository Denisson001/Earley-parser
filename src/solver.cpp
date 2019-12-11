#include "solver.h"

#include <algorithm>
#include <iostream>

/*
 * Формирует множество символов из строки
 */
TSymbolSet TSolver::_parseSymbolSet(const std::string& str) const {
    return { str.begin(), str.end() };
}

/*
 * Формирует правило по строке
 */
TRule TSolver::_parseRule(const std::string& str) const {
    if (str.size() < 4) {
        return { _NAN, {} };
    }

    TRule rule;
    rule.nonterminal = str[0];
    // skip "->"
    for (size_t i = 3; i < str.size(); ++i) {
        if (str[i] != _eps) {
            rule.result.push_back(str[i]);
        }
    }
    return rule;
}

/*
 * Формирует массив символов из строки
 */
TWord TSolver::_parseWord(const std::string &str) const {
    return { str.begin(), str.end() };
}

/*
 * Читает данные из input_stream
 * Формат ввода:
 *   - слово, которое алгоритм проверит на принадлежность языку
 *   - строка - множество всех нетерминалов
 *   - строка - множество всех терминалов
 *   - стартовый символ - нетерминал
 *   - правила в формате {нетерминал}->{последовательность символов}
 */
TData TSolver::readData(std::istream& input_stream) const {
    TData data;
    std::string line;

    input_stream >> line;
    data.word = _parseWord(line);

    input_stream >> line;
    data.nonterminals = _parseSymbolSet(line);

    input_stream >> line;
    data.terminals = _parseSymbolSet(line);

    input_stream >> line;
    data.start_symbol = line.empty() ? _NAN : line[0];

    while (input_stream >> line) {
        data.rules.push_back(_parseRule(line));
    }

    return data;
}

/*
 * Запускает Earley parser на данных data 
 */
TSolver::TResult TSolver::solve(const TData &data) const {
    if (!_checkDataCorrectness(data)) {
        return INCORRECT_DATA;
    }
    TEarleyParser earley_parser;
    const auto result = earley_parser.solve(data);
    return result ? TRUE : FALSE;
}

/*
 * Проверяет данные на корректность
 */
bool TSolver::_checkDataCorrectness(const TData& data) const {
    // стартовый символ не является нетерминалом
    if (!data.nonterminals.count(data.start_symbol)) {
        return false;
    }

    std::vector<TSymbol> common_symbols;
    std::set_intersection(data.terminals.begin(), data.terminals.end(),
                          data.nonterminals.begin(), data.nonterminals.end(),
                          std::back_inserter(common_symbols));

    // множество терминалов и нетерминалов пересекается
    if (!common_symbols.empty()) {
        return false;
    }

    for (auto&& rule : data.rules) {
        // левая часть правила не является нетерминалом
        if (!data.nonterminals.count(rule.nonterminal)) {
            return false;
        }

        for (auto&& symbol : rule.result) {
            // символ в правой части не лежит в множестве символов
            if (!data.terminals.count(symbol) && !data.nonterminals.count(symbol)) {
                return false;
            }
        }
    }

    return true;
}

/*
 * Преобразует TResult в строку
 */
std::string TSolver::resultToString(const TResult& result) const {
    switch (result) {
        case TRUE:
            return "True";
        case FALSE:
            return "False";
        case INCORRECT_DATA:
            return "Incorrect data";
    }
}