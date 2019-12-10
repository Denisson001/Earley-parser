#include "earley_parser.h"

TEarleyParser::_TState::_TState(TSymbol nonterminal, const TWord& result, size_t rule_position, size_t prefix_len, size_t rule_number) :
        TRule{nonterminal, result},
        rule_position(rule_position),
        prefix_len(prefix_len),
        rule_number(rule_number)
{}

TSymbol TEarleyParser::_TState::getNextSymbol() const {
    if (rule_position >= result.size()) {
        return _NAN;
    }
    return result[rule_position];
}

void TEarleyParser::_TState::readSymbol() {
    ++rule_position;
}