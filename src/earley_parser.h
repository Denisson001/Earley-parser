#pragma once

#include "rule.h"

#include <limits>

/*
 * Реализация Earley parser 
 */
class TEarleyParser {
public:
    /*
     * Запускает Earley parser на данных data 
     */ 
    bool solve(const TData& data);

private:
    /*
     * Внутреннее представление ситуации
     * Наследуется от структуры правила, добавляя следующие поля:
     *   rule_position - позиция в правиле
     *   prefix_len    - длина префикса, который можно вывести до нетерминала из левой части правила
     *   rule_number   - номер правила в массиве правил rules из data
     */
    struct _TState : public TRule {
        const static TSymbol _NAN = std::numeric_limits<TSymbol>::max();
        size_t rule_position;
        size_t prefix_len;
        size_t rule_number;

        _TState() {}
        _TState(TSymbol, const TWord&, size_t, size_t, size_t);
        
        /*
         * Возвращает символ из правой части правила, на который указывает rule_position
         * Или _NAN, если указатель вышел из границ
         */
        TSymbol getNextSymbol() const;


        /*
         * Передвигает rule_position на следующий символ
         */
        void    readSymbol();
    };

    typedef std::vector< std::vector< std::vector<bool> > > _TUsedStates;
    typedef std::vector< std::vector<_TState> >             _TStatesArray;

    /*
     * Массив множеств правил, аналог множеств D из описания алгоритма
     */
    _TStatesArray _states_array;


    /*
     * Массив флагов для _states_array для быстрой проверки принадлежности состояния множеству
     */
    _TUsedStates  _used_states;

    /*
     * Дополнительное правило и соответствующее состояние, которое используется в инициализации
     */
    _TState       _fake_state;

    /*
     * Длина слова word из data
     */
    size_t        _word_length;

    /*
     * Инициализация
     */
    void _init(const TData& data);
    void _initArrays(const TData& data);
    
    /*
     * Добавляет ситуацию state в множество _states_array[index] 
     */
    void _insertState(size_t index, const _TState& state);


    /*
     * Проверяет лежит ли ситуация state в множествe _states_array[index] 
     */
    bool _containsState(size_t index, const _TState& state) const;
    

    /*
     * Вычисляет номер ситуации state, используется для работы с _used_states 
     */
    size_t _calcStateNumber(const _TState& state) const;

    /*
     * Методы из алгоритма
     */
    void _scan(size_t index, const TWord& word);
    void _predict(size_t index, const TData& data);
    void _complete(size_t index);
};