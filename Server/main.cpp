/** @file
* @author Андреев И.В.
* @version 1.0
* @date 17.08.2024
* @copyright ИБСТ ПГУ
* @brief Главный файл проекта
*/
#include "ui.h"
#include "read_base.h"
#include "server.h"
#include "logger.h"
#include "error.h"
int main(int argc, char* argv[])
{
    UI interface (argc,argv);
    server SERV;
    SERV.work(interface);
    return 0;
}
