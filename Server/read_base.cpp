/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл модуля read_base
*/
#include "read_base.h"
base_reader::base_reader(std::string base_loc,std::string log_loc){
    log_location=log_loc;
    cl_base.open(base_loc, std::ios::in);
    if(cl_base.is_open()){
        log.write_log(log_location,"Base_reader: База данных успешно прочитана");
        read_base(cl_base);
        
    }
    else{
        log.write_log(log_location,"Base_reader: Ошибка чтения базы данных");
        std::cout<<"Такого файла базы данных клиента не существует"<<std::endl;
        throw critical_error ("Не удалось прочитать базу данных");
    }
}
void base_reader:: read_base(std::ifstream & b){
    if(b.eof()){
        throw critical_error("Файл с базой данных пустой");
    }
    while(!b.eof()){
        std::string temp;
        getline(b,temp);
        if (temp.empty()){
            continue;
        }
        if(std::count(temp.begin(),temp.end(),':')>1 or std::count(temp.begin(),temp.end(),':')<1){
            log.write_log(log_location,"Base_reader: Неверный формат данных о пользователе");
            throw critical_error("Неверный формат данных о пользователе");
        }
        else{
            clients.push_back(temp);
        }
    }
    logins=handle_id_s(clients);
    passwords=handle_password_s(clients);
}
std::vector<std::string> base_reader::handle_id_s(std::vector<std::string> logins){
    std::vector<std::string> temp;
    for (int i = 0; i < logins.size(); i++)
    {
        logins[i].erase(logins[i].find(":"));
        temp.push_back(logins[i]);
    }
    return temp;
}
std::vector<std::string> base_reader::handle_password_s(std::vector<std::string> pswds){
    std::vector<std::string> temp1;
    for (int i = 0; i < pswds.size(); i++)
    {
        temp1.push_back(pswds[i].substr(pswds[i].find(":")+1));
    }
    return temp1;
}
std::vector <std::string> base_reader::get_logins(){
    return logins;
}
std::vector <std::string> base_reader::get_passwords(){
    return passwords;
}
base_reader::~base_reader(){
    cl_base.close();
}