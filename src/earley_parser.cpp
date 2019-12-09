#include "earley_parser.h"

#include <limits>

TSymbol TEarleyParser::_TState::getNextSymbol() const {
    if (rule_position >= result.size()) {
        return _NAN;
    }
    return result[rule_position];
}

void TEarleyParser::_TState::readSymbol() {
    ++rule_position;
}

#include <iostream>

bool TEarleyParser::solve(const TData &data) {
    _init(data);

    for (size_t i = 0; i < _states_array.size(); ++i) {
        _scan(i, data.word);
        while (true) {
            /*std::cout << i << std::endl;
            for (const auto state : _states_array[i]) {
                std::cout << state.nonterminal << " -> ";
                for (auto x : state.result) std::cout << x << ' ';
                std::cout << "    ";
                std::cout << state.rule_position;
                std::cout << std::endl;
            }*/
            const auto saved_size = _states_array[i].size();
            _predict(i, data);
            _complete(i);
            if (saved_size == _states_array[i].size()) {
                break;
            }
        }
    }

    _TState finish_state;
    finish_state.nonterminal   = _fake_nonterminal;
    finish_state.result        = { data.start_symbol };
    finish_state.rule_position = 1;
    finish_state.prefix_len    = 0;
    return _containsState(data.word.size(), finish_state);
}

void TEarleyParser::_init(const TData &data) {
    _states_array.resize(data.word.size() + 1);
    _TState start_state;
    start_state.nonterminal   = _fake_nonterminal;
    start_state.result        = { data.start_symbol };
    start_state.rule_position = 0;
    start_state.prefix_len    = 0;
    _states_array[0].push_back(start_state);
}

void TEarleyParser::_scan(size_t index, const TWord& word) {
    if (index == 0) {
        return;
    }
    for (const auto& state : _states_array[index - 1]) {
        if (state.rule_position != state.result.size()) {
            const auto current_symbol = state.result[state.rule_position];
            if (current_symbol == word[index - 1]) {
                auto new_state = state;
                new_state.readSymbol();
                _insertState(index, new_state);
            }
        }
    }
}

void TEarleyParser::_predict(size_t index, const TData& data) {
    const auto saved_size = _states_array[index].size();
    for (size_t i = 0; i < saved_size; ++i) {
        const auto state = _states_array[index][i]; // КОПИЯ
        if (data.nonterminals.count(state.getNextSymbol())) {
            for (const auto& rule : data.rules) {
                if (rule.nonterminal == state.getNextSymbol()) {
                    _TState new_state;
                    new_state.nonterminal   = rule.nonterminal;
                    new_state.result        = rule.result;
                    new_state.rule_position = 0;
                    new_state.prefix_len    = index;
                    _insertState(index, new_state);
                }
            }
        }
    }
}

void TEarleyParser::_complete(size_t index) {
    const auto saved_size = _states_array[index].size();
    for (size_t i = 0; i < saved_size; ++i) {
        const auto state = _states_array[index][i]; // КОПИЯ
        if (state.rule_position == state.result.size()) {
            for (size_t j = 0; j < _states_array[state.prefix_len].size(); ++j) { // вроде не плохо
                //std::cout << j << ' ' << _states_array[state.prefix_len].size() << ' ' << state.prefix_len << " sfdsadfasdf\n";
                const auto& current_state = _states_array[state.prefix_len][j];
                if (current_state.getNextSymbol() == state.nonterminal) {
                    auto new_state = current_state;
                    new_state.readSymbol();
                    _insertState(index, new_state);
                }
            }
        }
    }
}

void TEarleyParser::_insertState(size_t index, const TEarleyParser::_TState &state) { // == вынести
    for (const auto& current_state : _states_array[index]) {
        if (state.nonterminal   == current_state.nonterminal   &&
            state.result        == current_state.result        &&
            state.rule_position == current_state.rule_position &&
            state.prefix_len    == current_state.prefix_len) {
            return;
        }
    }
    _states_array[index].emplace_back(state);
}

bool TEarleyParser::_containsState(size_t index, const TEarleyParser::_TState &state) const {
    for (const auto& current_state : _states_array[index]) {
        if (state.nonterminal   == current_state.nonterminal   &&
            state.result        == current_state.result        &&
            state.rule_position == current_state.rule_position &&
            state.prefix_len    == current_state.prefix_len) {
            return 1;
        }
    }
    return 0;
}