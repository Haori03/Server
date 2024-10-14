/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл модуля server
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <random>
#include <chrono>
#include <thread>
#include <limits>
#include "read_base.h"
#include "logger.h"
#include "error.h"
#include "ui.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
/** @brief Класс сервера
* @details Порт, расположение логфайла и базы данных устанавливаются в методе work с помощью методов объекта класса UI(Передается по ссылке) \n Работу сервера начинает метод work
* Сетевое взаимодействие реализовано через методы client_auth, connect_to_cl, handle_data, recv_data, send_data
*/
class server{
    private:
        /**
        * @brief Структуры адреса сервера и клиента
        */
        struct sockaddr_in serverAddr, clientAddr;
        /**
        * @brief Размер адреса
        */
        socklen_t addr_size;
        /**
        * @brief Расположение базы данных
        */
        std::string base_location;
        /**
        * @brief Вектора с логинами и паролями
        */
        std::vector<std::string> cl_ids,cl_passes;
        /**
        * @brief Размер буффера
        */
        size_t buflen =1024;
        /**
        * @brief Unique_ptr для приема и отправки сообщений
        */
        std::unique_ptr <char[]> buffer{new char[buflen]};
        /**
        * @brief Порт
        */
        uint port;
        /**
        * @brief Набор 16-ричных символов для генерации SALT
        */
        std::string digits[16] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
    public:
        /**
        * @brief Объекта класса loger для записи сообщений в журнал
        */
        logger log;
        /**
        * @brief Количество векторов
        */
        uint32_t nums;
        /**
        * @brief Переменная для сокетов сервера и клиента
        */
        int serverSocket, clientSocket;
        /**
        * @brief Айди клиента, расположение лог файла
        */
        std::string cl_id,log_location;
        /**
        * @brief Соединение с клиентом
        * @details Осуществляется установление соединения с клиентом. Сервер ожидает подключения
        * @return Функция ничего не возвращает
        * @throw critical_error, если не удалось встать в режим ожидания подключения
        */
        void connect_to_cl();
        /**
        * @brief Аутентификация клиента
        * @details Проверка присланного хеша со сгенерированным на основе присланного логина и соответствующего пароля из базы данных
        * @return Функция возвращает 1, если не удалось аутентифицировать клиента
        */
        int client_auth();
        /**
        * @brief Генерация соли
        * @details Генерируется 64-разрядное число, затем конвертируется в 16-ричное с дополнением 0 до длины 16
        * @return Сгенерированная соль
        */
        std::string SALT_generate();
        /**
        * @brief Конвертирование числа в 16-ричный формат
        * @details Число в цикле конвертируется в 16-ричное
        * @param [in] d_salt Случайно сгенерированное чилсо для конвертации
        * @return Функция возвращает сгенерированное 64-разрядное число
        */
        std::string convert_to_hex(uint64_t);
        /**
        * @brief Отправка сообщения клиенту
        * @details Осуществляется отправка данных клиенту с помощью unique_ptr
        * @param [in] data Строка, которую необходимо отправить
        * @param [in] log_msg Сообщение,записываемое в журнал при возникновении ошибки
        * @throw В случае ошибки при отправке закрывается соединение с клиентом
        */
        void send_data(std::string data,std::string msg);
        /**
        * @brief Прием данных от клиента
        * @details Внутри в цикле обрабатывается ситуация когда размер присылаемого сообщенния больше буффера
        * @param [in] msg Строка для записи принятого сообщения
        * @throw В случае ошибки при приеме закрывается соединение с клиентом
        */
        std::string recv_data(std::string messg);
        /**
        * @brief Генерация хеша
        * @details Хеш генерируется на основе соли и пароля
        * @param [in] salt Соль
        * @param [in] pswd Пароль клиента
        * @return Функция возвращает сгенерированный хеш
        */
        std::string hash_gen(std::string &salt,std::string &password);
        /**
        * @brief Метод закрытия соединения с клиентом
        */
        void close_sock();
        /**
        * @brief Запуск сервера
        * @details Создание сокета сервера и его привязка к локальному адресу, чтение базы данных клиентов
        * @throw critical_error, если не удалось создать или привязать сокет
        */
        void start();
        /**
        * @brief Калькулятор
        * @details Суммирует элементы вектора
        * @param [in] number1 Первое слагаемое int64_t
        * @param [in] number2 Второе слагаемое int64_t
        * @warning В случае переполнения при сумме возвращается максимум или минимум типа данных, так же если второе слагаемое имеет значение больше или меньше лимита типа данных
        * @return Функция возвращает сумму двух чисел
        */
        int64_t calculation(int64_t number1, int64_t number2);
        /**
        * @brief Обработка данных от клиента
        * @details Прием количества векторов и их значение, выполнение суммы элементов векторов
        * @warning В случае приема неверного типа данных соединение с клиентом обрывается
        */
        int handle_data();
        /**
        * @brief Работа сервера
        * @details Осуществляется запуск сервера, происходит соединение с клиентом, аутентификация и обработка данных, присланных от клиента
        * @param [in] intf Объект класса UI для получения расположения лог файла, базы данных и порта при разборе комстроки
        * @throw В случае ошибки на всех этапах работы сервера: аутентификация, старт, соединение с клиентом, обработка данных
        */
        void work(UI &intf);
};
