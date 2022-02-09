#include "config.h"
#include <iostream>
int main(int argc, char *argv[]) {
    auto tmp_ptr = frdmu::ConfigMgr::lookup("port", 9999, "server port"); 
    std::cout << tmp_ptr->toString() << std::endl;
    tmp_ptr->print();
    
    frdmu::ConfigMgr config_mgr;  
    auto tmp_pptr = config_mgr.lookup<int>("port");
    std::cout << tmp_pptr->toString() << std::endl;
    tmp_pptr->print();
    
    return 0;
}
