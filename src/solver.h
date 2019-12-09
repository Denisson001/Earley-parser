#pragma once

#include "rule.h"
#include "earley_parser.h"

#include <string>

class TSolver {
public:
    TData readData(std::istream& input_stream) const;
    bool solve(const TData& data) const;

private:
    static const TSymbol _eps = '#';

    TWord      _parseWord(const std::string& str)      const;
    TSymbolSet _parseSymbolSet(const std::string& str) const;
    TRule      _parseRule(const std::string& str)      const;
};