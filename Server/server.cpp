/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля server
*/
#include "server.h"
void server::work(UI &intf){
    try{
    port=intf.get_port();
    base_location=intf.get_base_loc();
    log_location=intf.get_log_loc();
    if(base_location=="" or log_location=="" or port == 1){
            exit(1);
        }
    start();
    while (true)
    {
        connect_to_cl();
        log.write_log(log_location,"Server: Сервер установил соединение с клиентом: "+cl_id);
        int flag = client_auth();
        if(flag==1){
                log.write_log(log_location,"Server: Серверу не удалось аутентифицировать клиента "+cl_id);
                close_sock();
            }
        else{
            log.write_log(log_location,"Server: Аутентификация прошла успешно, начало фазы вычислений");
            if(handle_data()==1){
                std::cout<<"Клиент прислал неверные данные"<<std::endl;
                close_sock();
                continue;
            }
            close_sock();
            log.write_log(log_location,"Server: Работа с клиентом "+cl_id+" успешно завершена"+"\n");
            std::cout << "Сервер завершил работу для пользователя: "<<cl_id << std::endl;

        }
    }
    } catch (po::error& e) {
        std::cout << e.what() << std::endl;
    }
    catch(critical_error &e){
        std::cout<<"Критическая ошибка: "<<e.what()<<std::endl;
    }
}
void server::start(){
    base_reader base(base_location,log_location);
    cl_ids=base.get_logins();
    cl_passes=base.get_passwords();
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        log.write_log(log_location,"Server: Ошибка при создании сокета сервера");
        throw critical_error ("Сокет не был создан");
    }
    log.write_log(log_location,"Server: Сокет для сервера создан");
    std::cout << "Сокет создан" << std::endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        log.write_log(log_location,"Server: Ошибка при привязке сокета");
        throw critical_error ("Сокет не был привязан");
    }
    log.write_log(log_location,"Server: Cокет привязан");
    std::cout << "Сокет привязан" << std::endl;
}
void server::connect_to_cl(){
    if (listen(serverSocket, 10) == 0) {
        std::cout << "Сервер слушает..." << std::endl;
        log.write_log(log_location,"Server: Сервер встал на прослушку порта");
    } else {
        log.write_log(log_location,"Server: Ошибка при прослушивании порта");
        std::cout << "Ошибка при прослушивании" << std::endl;
        throw critical_error ("Север не встал на прослушку порта");
    }
    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
    if(clientSocket<0) {
        log.write_log(log_location,"Server: Ошибка при аутентификации: ошибка принятия соединения клиента");
        std::cout<<"Ошибка принятия соединения клиента"<<std::endl;
        close(clientSocket);
    } else {
        log.write_log(log_location,"Server: Соединение с клиентом установлено");
        std::cout << "Соединение установлено" << std::endl;
    }
    cl_id=recv_data("Server: Ошибка при приеме айди клиента");
    std::cout<<"Подсоединился пользователь: "+cl_id<<std::endl;
}
int server::client_auth(){
    auto result = std::find(cl_ids.begin(),cl_ids.end(),cl_id);
    std::string salt=SALT_generate();
    std::string flag="ERR";
    if(result==cl_ids.end()){
        std::cout<<"Клиент с таким айди не найден"<<std::endl;
        log.write_log(log_location,"Server: Клиента с айди: "+cl_id+" нет в базе данных");
        send_data(flag,"Server: Ошибка при отправке ERR");
        return 1;
    }
    log.write_log(log_location,"Server: Начало фазы аутентификации для пользователя: "+cl_id);
    std::cout << "Фаза аутентификации для пользователя: "<<cl_id<< std::endl;
    auto pos = std::distance(cl_ids.begin(), result); 
    send_data(salt,"Server: Ошибка при отправке соли");
    log.write_log(log_location,"Server: Отправлена соль");
    std::string cl_hash=recv_data("Ошибка при приеме хеша");
    log.write_log(log_location,"Server: От клиента принят хеш");
    std::string serv_hash=hash_gen(salt,cl_passes[pos]);
    if(serv_hash!=cl_hash){
        log.write_log(log_location,"Server: Ошибка при аутентификации: хеши не соответствуют друг другу");
        std::cout<<"Пароль неверный"<<std::endl;
        send_data(flag,"Server: Ошибка при отправке ERR");
        return 1;
    }
    std::cout<<"Пользователь "<<cl_id<<" аутентифицорован"<<std::endl;
    send_data("OK","Server: Ошибка при отправке OK");
    return 0;
}  
std::string server::recv_data(std::string messg){
    int rc=0;
    while (true) {
        buffer = std::unique_ptr<char[]>(new char[buflen]); 
        rc = recv(clientSocket, buffer.get(), buflen, MSG_PEEK);
        if (rc==0){
            close_sock();
            log.write_log(log_location,"Server: Клиент закрыл соединение");
        }
        else if (rc<0){
            close_sock();
            log.write_log(log_location,messg);
        }
        if (rc < buflen) 
            break; 
        buflen *= 2; 
    }
    std::string msg(buffer.get(), rc);
    recv(clientSocket, nullptr, rc, MSG_TRUNC);
    return msg;
}
std::string server::SALT_generate()
{
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
    std::mt19937_64 rnd(seed);
    uint64_t int_salt=rnd();
    std::string hex_salt=convert_to_hex(int_salt);
    while(hex_salt.length()<16) {
        hex_salt="0"+hex_salt;
    }
   log.write_log(log_location,"Server: Соль сгенерирована");
    return hex_salt;

}
std::string server::convert_to_hex(uint64_t x)
{
    std::string hex;
    do {
        hex.insert (0, digits[x % 16]);
        x /= 16;
    } while (x!=0);
    return hex;
}
void server::close_sock(){
    close(clientSocket);
    log.write_log(log_location,"Server: Разорвано соединение с клиентом");
}
std::string server::hash_gen(std::string &salt,std::string &password){
    CryptoPP::Weak::MD5 hash;
    std::string serv_hash;
    CryptoPP::StringSource(salt+ password, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(serv_hash))));
    return serv_hash;
}
void server::send_data(std::string data,std::string msg){
    std::unique_ptr<char[]> temp {new char[data.length()+1]};
    strcpy(temp.get(), data.c_str());
    buffer = std::move(temp);
    int sb = send(clientSocket, buffer.get(), data.length(), 0);
    if(sb<0){
        log.write_log(log_location,msg);
        close_sock();
    }
}
int64_t server::calculation(int64_t number1, int64_t number2){
    int64_t res;
    if(number2>=std::numeric_limits<int64_t>::max()){
            res=std::numeric_limits<int64_t>::max();
            return res;
        }
    else if(number2<=std::numeric_limits<int64_t>::min()) {
            res = std::numeric_limits<int64_t>::min();
            return res;
        }
    res=number1+number2;
    if(res>=std::numeric_limits<int64_t>::max()){
            return std::numeric_limits<int64_t>::max();
        }
    else if(res<=std::numeric_limits<int64_t>::min()) {
            return std::numeric_limits<int64_t>::min();;
        }
    return res;
}
int server::handle_data(){
    std::chrono::milliseconds duration(100);
    int recv_b = recv(clientSocket,&nums,sizeof nums,0);
    std::this_thread::sleep_for(duration);
    for (uint32_t i = 0; i < nums; i++)
    {
        int64_t res=0;
        uint32_t size_of_vec;
        int recv_b = recv(clientSocket,&size_of_vec,sizeof size_of_vec,0);
        if(recv_b<0){
                log.write_log(log_location,"Server.data_handler: Ошибка принятия размера вектора");
                return 1;
            }
        log.write_log(log_location,"Server.data_handler: Получен размер вектора: "+std::to_string(size_of_vec));
        for(uint32_t j =0;j<size_of_vec;j++){
            int64_t vec;
            std::chrono::milliseconds duration(100);
            std::this_thread::sleep_for(duration);
            int rec = recv(clientSocket,&vec,sizeof vec,MSG_DONTWAIT);
            if(rec!=8){
                log.write_log(log_location,"Server.data_handler: Получен неверный тип данных");
                return 1;
            }
            log.write_log(log_location,"Server.data_handler: Получены данные вектора");
            res=calculation(res,vec);
        }
        int send_b=send(clientSocket,&res,sizeof res,0);
        if(send_b<0){
                log.write_log(log_location,"Server.data_handler: Ошибка отправки результата вычислений");
                return 1;
            }
        log.write_log(log_location,"Server.data_handler: Отправлен результат вычислений для вектора: "+std::to_string(res));
    }
    
} 