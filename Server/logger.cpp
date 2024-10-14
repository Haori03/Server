#include "logger.h"
int logger::write_log(std::string log_loc,std::string message){
    if(!boost::filesystem::exists(log_loc)){
        std::cout<<"Такого лог файла не существует"<<std::endl;
        throw critical_error ("Не удалось открыть лог файл");
    }
    log.open(log_loc, std::ios::app | std::ios::out);
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    std::string time = std::ctime(&end_time);
    time.pop_back();
    log<<time<<" / "<<message<<'\n';
    log.flush();
    log.close();
    return 0;
}
