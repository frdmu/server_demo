#include "config.h"
#include <iostream>
int main(int argc, char *argv[]) {
    // user defiend 
    auto tmp_ptr = frdmu::ConfigMgr::lookup("server.ip", std::string("192.168.9.9"), "server ip"); 
    auto tmp2_ptr = frdmu::ConfigMgr::lookup("server.port", 9999, "server port"); 
    auto tmp3_ptr = frdmu::ConfigMgr::lookup("server.user", std::string("frdmu"), "server user"); 
    
    frdmu::ConfigMgr::loadFromYaml("./config.yaml");
    std::cout << tmp_ptr->toString() << std::endl;
    std::cout << tmp2_ptr->toString() << std::endl;
    std::cout << tmp3_ptr->toString() << std::endl;
    return 0;
}
