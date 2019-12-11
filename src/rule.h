#pragma once

#include <vector>
#include <unordered_set>

typedef std::size_t          TSymbol;
typedef std::vector<TSymbol> TWord;
typedef std::unordered_set<TSymbol>    TSymbolSet;

/*
 * Структура правила
 * nonterminal - нетерминал в левой части правила
 * result      - правая часть правила
 */
struct TRule {
    TSymbol nonterminal;
    TWord   result;
};

typedef std::vector<TRule>   TRules;

/*
 * Структура данных
 * terminals    - множество терминальных символов
 * nonterminals - множество нетерминалов
 * start_symbol - стартовый нетерминал
 * rules        - массив правил
 * word         - слово, полученное на проверку принадлежности языку
 */
struct TData {
    TSymbolSet terminals;
    TSymbolSet nonterminals;
    TSymbol    start_symbol;
    TRules     rules;
    TWord      word;
};