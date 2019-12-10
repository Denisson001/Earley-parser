#pragma once

#include "rule.h"

#include <limits>

class TEarleyParser {
public:
    bool solve(const TData& data);

private:
    struct _TState : public TRule {
        const static TSymbol _NAN = std::numeric_limits<TSymbol>::max();
        size_t rule_position;
        size_t prefix_len;
        size_t rule_number;

        _TState() {}
        _TState(TSymbol, const TWord&, size_t, size_t, size_t);
        TSymbol getNextSymbol() const;
        void    readSymbol();
    };

    typedef std::vector< std::vector< std::vector<bool> > > _TUsedStates;
    typedef std::vector< std::vector<_TState> >             _TStatesArray;

    _TStatesArray _states_array;
    _TUsedStates  _used_states;
    _TState       _fake_state;
    size_t        _word_length;

    void _init(const TData& data);
    void _initArrays(const TData& data);
    void _insertState(size_t index, const _TState& state);
    bool _containsState(size_t index, const _TState& state) const;
    size_t _calcStateNumber(const _TState& state) const;

    void _scan(size_t index, const TWord& word);
    void _predict(size_t index, const TData& data);
    void _complete(size_t index);
};