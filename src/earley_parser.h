#pragma once

#include "rule.h"

#include <limits>

class TEarleyParser {
public:
    bool solve(const TData& data);

private:
    const static TSymbol _fake_nonterminal = std::numeric_limits<TSymbol>::max() - 1;

    struct _TState : public TRule {
        const static TSymbol _NAN = std::numeric_limits<TSymbol>::max();
        size_t rule_position;
        size_t prefix_len;
        TSymbol getNextSymbol() const;
        void readSymbol();
    };

    typedef std::vector<_TState> _TStates;

    std::vector<_TStates> _states_array;

    void _init(const TData& data);
    void _scan(size_t index, const TWord& word);
    void _predict(size_t index, const TData& data);
    void _complete(size_t index);
    void _insertState(size_t index, const _TState& state);
    bool _containsState(size_t index, const _TState& state) const;
};