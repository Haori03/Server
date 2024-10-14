/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модуля UI
*/
#pragma once
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "error.h"
#include "logger.h"
namespace po = boost::program_options;
/** @brief Класс пользовательского интерфейса
* @details Разбор комстроки осуществляется в конструкторе
*/
class UI{
    public:
    /** @brief Объект модуля logger для записи сообщений в лог файл
    */
        logger log;
        /** @brief Описание запрашиваемых параметров в комстроке
        */
        po::options_description desc;
        /** @brief Хранит разобранные значения параметров комстроки
        */
        po::variables_map vm;
        /**
        * @brief Конструктор модуля
        * @details Разбирается комстрока
        * @param [in] argc Количество аргументов комстроки
        * @param [in] argv Значения аргументов комстроки
        */
        UI(int argc, char* argv[]);
        /**
        * @brief Геттер порта
        * @details Поддерживается ввод нескольких значений. Возвращено будет последнее введенное
        * @return Значение порта, введенное пользователем 
        * @throw critical_error, если пользователь ввел системный порт(<1024)
        */
        uint get_port();
        /**
        * @brief Геттер расоложения базы данных
        * @details Поддерживается ввод нескольких значений. Возвращено будет последнее введенное
        * @return Возвращается строка с расположением базы данных
        */
        std::string get_base_loc();
        /**
        * @brief Геттер расоложения лог файла
        * @details Поддерживается ввод нескольких значений. Возвращено будет последнее введенное
        * @return Возвращается строка с расположением файла журнала
        */
        std::string get_log_loc();
};