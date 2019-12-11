#include "earley_parser.h"

/*
 * Copy constructor
 */
TEarleyParser::_TState::_TState(TSymbol nonterminal, const TWord& result, size_t rule_position, size_t prefix_len, size_t rule_number) :
        TRule{nonterminal, result},
        rule_position(rule_position),
        prefix_len(prefix_len),
        rule_number(rule_number)
{}

/*
 * Возвращает символ из правой части правила, на который указывает rule_position
 * Или _NAN, если указатель вышел из границ
 */
TSymbol TEarleyParser::_TState::getNextSymbol() const {
    if (rule_position >= result.size()) {
        return _NAN;
    }
    return result[rule_position];
}

/*
 * Передвигает rule_position на следующий символ
 */
void TEarleyParser::_TState::readSymbol() {
    ++rule_position;
}