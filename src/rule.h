#pragma once

#include <vector>
#include <set>

typedef std::size_t          TSymbol;
typedef std::vector<TSymbol> TWord;
typedef std::set<TSymbol>    TSymbolSet;

struct TRule {
    TSymbol nonterminal;
    TWord   result;
};

typedef std::vector<TRule>   TRules;

struct TData {
    TSymbolSet terminals;
    TSymbolSet nonterminals;
    TSymbol    start_symbol;
    TRules     rules;
    TWord      word;
};