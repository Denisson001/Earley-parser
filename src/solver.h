#pragma once

#include "rule.h"
#include "earley_parser.h"

#include <string>

/*
 * Класс позволяет
 * - Работать с данными 
 * - Запускать Earley parser на данных 
 */
class TSolver {
public:
    /*
     * Читает данные из input_stream  
     */
    TData readData(std::istream& input_stream) const;
    
    /*
     * Запускает Earley parser на данных data 
     */
    bool solve(const TData& data) const;

private:
	/*
	 * Обозначение пустого символа
	 */
    static const TSymbol _eps = '#';

    /*
     * Формирует массив символов из строки
     */
    TWord      _parseWord(const std::string& str)      const;
    
    /*
     * Формирует множество символов из строки
     */
    TSymbolSet _parseSymbolSet(const std::string& str) const;
    
    /*
     * Формирует правило по строке
     */
    TRule      _parseRule(const std::string& str)      const;
};