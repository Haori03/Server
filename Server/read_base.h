/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля read_base
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "logger.h"
#include "error.h"
/** @brief Класс базы данных
* @details Осуществляется работа с базой данных. Разбор данных, получение логинов и паролей
*/
class base_reader{
    private:
        /** @brief Вектор для хранения логинов
        */
        std::vector<std::string> logins;
        /** @brief Вектор для хранения паролей
        */
        std::vector<std::string> passwords;
        /** @brief Вектор для хранения логинов/паролей
        */
        std::vector<std::string> clients;
        /**
        * @brief Метод разбора логинов
        * @details Из строки логин:пароль выбирается логин
        * @param [in] logins Вектор для хранения логинов
        */
        std::vector<std::string> handle_id_s(std::vector<std::string> logins);
        /**
        * @brief Метод разбора паролей
        * @details Из строки логин:пароль выбирается пароль
        * @param [in] pswds Вектор для хранения паролей
        */
        std::vector<std::string> handle_password_s(std::vector<std::string> pswds);
    public:
        /**
        * @brief Объекта класса loger для записей в журнал
        */
        logger log;
        /**
        * @brief Расположение лог файла
        */
        std::string log_location;
        /**
        * @brief Объект потока ifstream для чтения базы данных
        */
        std::ifstream cl_base;
        /**
        * @brief Геттер вектора с логинами
        */
        std::vector<std::string> get_logins();
        /**
        * @brief Геттер вектора с паролями
        */
        std::vector<std::string> get_passwords();
        /**
        * @brief Метод чтения базы данных
        * @details Построчно считывается содержимое базы данных в формате логин:пароль
        * @param [in] base Объект потока ifstream с открытым файлом базы данных
        * @throw critical_error, если содержимое базы данных не соответствует формату логин:пароль
        */
        void read_base(std::ifstream &b);
        /**
        * @brief Конструктор инициализации
        * @details Открывается файл базы данных, при удачном открытии происходит чтение содерджимого файла
        * @param [in] base_loc Расположения файла базы данных
        * @param [in] log_loc Расположения лог файла
        * @throw critical_error, если не удалось открыть файл с базой данных
        */
        base_reader(std::string base_loc,std::string log_loc);
        /**
        * @brief Деструктор модуля
        * @details Закрывается файл с базой данных
        */
        ~base_reader();
};