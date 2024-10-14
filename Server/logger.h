/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля logger
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>
#include "error.h"
#include <boost/filesystem.hpp>
/** @brief Класс записи сообщений в журнал
* @details Запись сообщений в лог файл в методе write_log
*/
class logger{
    public:
        /** @brief Объект ofstream для открытия файла журнала
        */
        std::ofstream log;
        /**
        * @brief Метод записи сообщения в лог файл
        * @details Получается текущее время, затем сообщение записывается в файл в формате время/сообщение. Файл открывается, 
        * закрывается каждый раз для записи сообщения
        * @param [in] log_loc Расположение лог файла
        * @param [in] message Сообщение для записи
        * @throw critical_error, если введеное расположение файла указывает на несуществующий объект
        */
        int write_log(std::string log_loc,std::string message);
};       