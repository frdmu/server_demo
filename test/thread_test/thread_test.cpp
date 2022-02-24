#include "thread.h"
#include <vector>
#include <iostream>
#include <unistd.h>

int cnt = 0;
frdmu::Mutex mutex;

void func() {
    std::cout << " tid: "  << frdmu::Thread::GetId()
              << " name: " << frdmu::Thread::GetName()
              << std::endl;
    for (int i = 0; i < 100000; ++i) {
        frdmu::Lock lock(mutex); 
        ++cnt;
    }
}
int main(int argc, char **argv) {
    std::vector<frdmu::Thread::ptr> thvec;
    
    for (int i = 0; i < 5; ++i) {
        frdmu::Thread::ptr thptr(new frdmu::Thread(func, "name" + std::to_string(i)));
        thvec.push_back(thptr);
    }
    
    frdmu::Thread::ptr thptr(new frdmu::Thread(func, ""));
    thvec.push_back(thptr);
    
    for (int i = 0; i < 6; ++i) {
        thvec[i]->join();
    }
    
    std::cout << "cnt:" << cnt << std::endl;
    return 0;
}
