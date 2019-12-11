#include "earley_parser.h"

#include <algorithm>

bool TEarleyParser::solve(const TData &data) {
    _init(data);

    for (size_t i = 0; i < _states_array.size(); ++i) {
        _resetMemoizationData();
        _scan(i, data.word);
        size_t saved_size;
        do {
            saved_size = _states_array[i].size();
            _predict(i, data);
            _complete(i);
        } while (saved_size != _states_array[i].size());
    }

    _fake_state.readSymbol();
    return _containsState(data.word.size(), _fake_state);
}

/*
 * Инициализирует _fake_state и _word_length
 * Добавляет _fake_state в множество с номером 0
 */
void TEarleyParser::_init(const TData &data) {
    const auto _fake_nonterminal = (*std::max_element(data.nonterminals.begin(), data.nonterminals.end())) + 1;
    _fake_state = { _fake_nonterminal, { data.start_symbol }, 0, 0, data.rules.size() };
    _word_length = data.word.size() + 1;
    _initArrays(data);
    _insertState(0, _fake_state);
}

/* 
 * Инициализирует _states_array и _used_states
 * _used_states[index][state_number][rule_position] - трехмерный массив:
 *  - index         - номер множества в _states_array 
 *  - state_number  - в этом измерении зашиты сразу два параметра - rule_number, word_pos
 *    - rule_number - номер правила в массиве data.rules 
 *    - word_pos    - длина префикса ситуации state.prefix_len 
 *  - rule_position - позиция в правиле
 */
void TEarleyParser::_initArrays(const TData &data) {
    _states_array.resize(_word_length);
    _used_states.resize(_word_length);
    const auto calcStateNumber = [this](size_t rule_number, size_t word_pos) {
        return rule_number * _word_length + word_pos;
    };

    const auto rules_count = data.rules.size() + 1;
    _max_state_number = _word_length * rules_count;

    for (size_t index = 0; index < _word_length; ++index) {
        _used_states[index].resize(rules_count * _word_length);
        for (size_t word_pos = 0; word_pos < _word_length; ++word_pos) {
            for (size_t rule_number = 0; rule_number < rules_count - 1; ++rule_number) {
                _used_states[index][calcStateNumber(rule_number, word_pos)].resize(data.rules[rule_number].result.size() + 1, false);
            }
            // отдельно учитываем дополнительное состояние _fake_state
            _used_states[index][calcStateNumber(rules_count - 1, word_pos)].resize(_fake_state.result.size() + 1, false);
        }
    }
}

/*
 * Сбрасывает данные, используемые для запоминания в _predict и _complete, на новой итерации внешнего цикла
 */
void TEarleyParser::_resetMemoizationData() {
    _last_processed_state = 0;
    _last_processed_states.assign(_max_state_number, 0);
}

void TEarleyParser::_scan(size_t index, const TWord& word) {
    if (index == 0) {
        return;
    }
    for (auto&& state : _states_array[index - 1]) {
        if (state.rule_position != state.result.size()) {
            const auto current_symbol = state.getNextSymbol();
            if (current_symbol == word[index - 1]) {
                auto new_state = state;
                new_state.readSymbol();
                _insertState(index, new_state);
            }
        }
    }
}

void TEarleyParser::_predict(size_t index, const TData& data) {
    for (; _last_processed_state < _states_array[index].size(); ++_last_processed_state) {
        const auto current_symbol = _states_array[index][_last_processed_state].getNextSymbol();
        if (!data.nonterminals.count(current_symbol)) {
            continue;
        }
        for (size_t rule_number = 0; rule_number < data.rules.size(); ++rule_number) {
            const auto& rule = data.rules[rule_number];
            if (rule.nonterminal == current_symbol) {
                _TState new_state = { rule.nonterminal, rule.result, 0, index, rule_number };
                _insertState(index, new_state);
            }
        }
    }
}

void TEarleyParser::_complete(size_t index) {
    for (size_t i = 0; i < _states_array[index].size(); ++i) {
        const auto state = _states_array[index][i];
        if (state.rule_position != state.result.size()) {
            continue;
        }
        for (; _last_processed_states[i] < _states_array[state.prefix_len].size(); ++_last_processed_states[i]) {
            const auto& current_state = _states_array[state.prefix_len][_last_processed_states[i]];
            if (current_state.getNextSymbol() == state.nonterminal) {
                auto new_state = current_state;
                new_state.readSymbol();
                _insertState(index, new_state);
            }
        }
    }
}

/*
 * Добавляет ситуацию state в множество _states_array[index], если ее там еще не было
 */
void TEarleyParser::_insertState(size_t index, const _TState &state) {
    const auto state_number = _calcStateNumber(state);
    if (!_used_states[index][state_number][state.rule_position]) {
        _states_array[index].emplace_back(state);
        _used_states[index][state_number][state.rule_position] = true;
    }
}

/*
 * Проверяет лежит ли ситуация state в множествe _states_array[index]
 */
bool TEarleyParser::_containsState(size_t index, const _TState &state) const {
    const auto state_number = _calcStateNumber(state);
    return _used_states[index][state_number][state.rule_position];
}

/*
 * Вычисляет номер ситуации state по state.rule_number и state.prefix_len
 */
size_t TEarleyParser::_calcStateNumber(const _TState &state) const {
    return state.rule_number * _word_length + state.prefix_len;
}