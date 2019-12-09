#include "solver.h"
#include <iostream>

TSymbolSet TSolver::_parseSymbolSet(const std::string& str) const {
    return { str.begin(), str.end() }; // отрицательные?
}

TRule TSolver::_parseRule(const std::string& str) const {
    TRule rule;
    rule.nonterminal = str[0];
    for (size_t i = 3; i < str.size(); ++i) {
        if (str[i] != _eps) {
            rule.result.push_back(str[i]);
        }
    }
    return rule;
}

TWord TSolver::_parseWord(const std::string &str) const {
    return { str.begin(), str.end() };
}

TData TSolver::readData(std::istream& input_stream) const { // чек корректность
    TData data;
    std::string line;

    input_stream >> line;
    data.word = _parseWord(line);

    input_stream >> line;
    data.nonterminals = _parseSymbolSet(line);

    input_stream >> line;
    data.terminals = _parseSymbolSet(line);

    input_stream >> line;
    data.start_symbol = line[0];

    while (input_stream >> line) {
        data.rules.push_back(_parseRule(line));
    }

    return data;
}

bool TSolver::solve(const TData &data) const {
    TEarleyParser earley_parser;
    return earley_parser.solve(data);
}