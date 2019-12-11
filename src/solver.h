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
     * Проверяет данные на корректность
     */
    TResult checkDataCorrectness(const TData& data) const;

    /*
     * Запускает Earley parser на данных data 
     */
    TResult solve(const TData& data) const;

    /*
     * Преобразует TResult в строку
     */
    void printResult(const TResult& result) const;

private:
	/*
	 * Обозначение пустого символа
	 */
    static const TSymbol _eps = '#';

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