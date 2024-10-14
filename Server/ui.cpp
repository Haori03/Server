/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Исполняемый файл для модуля UI
*/
#include "ui.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
UI::UI(int argc, char* argv[])
{
    desc.add_options()
    ("help,h", "Помощь")
    ("Log_location,l", po::value<std::vector<std::string>>()->multitoken(), "Путь для log файла")
    ("Port,p", po::value<std::vector<uint>>()->multitoken(), "Порт сервера")
    ("Base_location,b", po::value<std::vector<std::string>>()->multitoken(), "Путь для базы данных клиентов");
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
            std::cout << desc << std::endl;
            exit(0);  
        }
}
uint UI::get_port()
{
    if (vm.count("Port") and !vm["Port"].as<std::vector<uint>>().empty() ) {
        const std::vector<uint>& ports = vm["Port"].as<std::vector<uint>>();
        if(ports.back()<1024){
            log.write_log(get_log_loc(),"UI: Пользователь ввел системный порт");
            throw critical_error("Выбран системный порт");
            
        }
        return ports.back();
    } else {
        std::cout<<"Ошибка при вводе параметров, воспользуйтесь командой help"<<std::endl;
        exit(1);
        return 1;
    }
}
std::string UI::get_base_loc()
{
    if (vm.count("Base_location") and !vm["Base_location"].as<std::vector<std::string>>().empty()) {
        const std::vector<std::string>& base_loc = vm["Base_location"].as<std::vector<std::string>>();
        return base_loc.back();
    } else {
        std::cout<<"Ошибка при вводе параметров, воспользуйтесь командой help"<<std::endl;
        exit(1);
        return "";
    }
}
std::string UI::get_log_loc()
{
    if (vm.count("Log_location") and !vm["Log_location"].as<std::vector<std::string>>().empty()) {
        const std::vector<std::string>& log_loc = vm["Log_location"].as<std::vector<std::string>>();
        return log_loc.back();
    } else {
        std::cout<<"Ошибка при вводе параметров, воспользуйтесь командой help"<<std::endl;
        exit(1);
        return "";
    }
}