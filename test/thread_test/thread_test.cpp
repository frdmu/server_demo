#include "thread.h"
#include <vector>
#include <iostream>
#include <unistd.h>
void func() {
    std::cout << " tid: "  << frdmu::Thread::GetId()
              << " name: " << frdmu::Thread::GetName()
              << std::endl;
}
int main(int argc, char **argv) {
    std::vector<frdmu::Thread::ptr> thvec;
    
    for (int i = 0; i < 5; ++i) {
        frdmu::Thread::ptr thptr(new frdmu::Thread(func, "name" + std::to_string(i)));
        sleep(1);
        thvec.push_back(thptr);
    }
    
    frdmu::Thread::ptr thptr(new frdmu::Thread(func, ""));
    thvec.push_back(thptr);
    
    for (int i = 0; i < 6; ++i) {
        thvec[i]->join();
    }

    return 0;
}
