#pragma once

#include "rule.h"
#include "earley_parser.h"

#include <string>
#include <limits>

/*
 * Класс позволяет
 * - Работать с данными 
 * - Запускать Earley parser на данных 
 */
class TSolver {
public:
    /*
     * TRUE           - слово лежит в языке
     * FALSE          - слово не лежит в языке
     * INCORRECT_DATA - некорректные данные
     */
    enum TResult {
        TRUE,
        FALSE,
        INCORRECT_DATA
    };

    /*
     * Читает данные из input_stream  
     */
    TData readData(std::istream& input_stream) const;

    /*
     * Запускает Earley parser на данных data 
     */
    TResult solve(const TData& data) const;

    /*
     * Преобразует TResult в строку
     */
    std::string resultToString(const TResult& result) const;

private:
	/*
	 * Обозначение пустого символа
	 */
    static const TSymbol _eps = '#';

    /*
     * Обозначение некорректного символа
     */
    static const TSymbol _NAN = std::numeric_limits<TSymbol>::max();

    /*
     * Проверяет данные на корректность
     */
    bool _checkDataCorrectness(const TData& data) const;

    /*
     * Формирует массив символов из строки
     */
    TWord _parseWord(const std::string& str) const;
    
    /*
     * Формирует множество символов из строки
     */
    TSymbolSet _parseSymbolSet(const std::string& str) const;
    
    /*
     * Формирует правило по строке
     */
    TRule _parseRule(const std::string& str) const;
};