#include "thread.h"
#include <iostream>
namespace frdmu {
    static thread_local pid_t thread_id = -1;
    static thread_local std::string thread_name = "UNKNOW";
    
    Thread::Thread(std::function<void()> cb, std::string name): m_cb(cb), m_name(name) {
        if (name.empty()) {
            m_name = "UNKNOW";
        } 
        int ret = pthread_create(&m_thread, nullptr, run, this);
        if (ret != 0) {
            std::cout << "construct thread failed" << std::endl;
        } 
    }
    
    void *Thread::run(void* args) {
        Thread *thread = (Thread *)args; 
        thread->m_id = gettid();
        
        // 这里容易出错 thread_local   
        thread_id = thread->m_id;
        thread_name = thread->m_name; 
        
        thread->m_cb();

        return nullptr;
    }
            
    void Thread::join() {
        if (m_thread) {
            int ret = pthread_join(m_thread, nullptr);
            if (ret != 0) {
                std::cout << "join failed" << std::endl;
            }
            m_thread = 0; // 这里很重要
        }    
    }

    Thread::~Thread() {
        if (m_thread) // join()中，若回收完毕，m_thread = 0
            pthread_detach(m_thread);
    }

    pid_t Thread::GetId() {
        return thread_id;
    }

    const std::string &Thread::GetName() {
        return thread_name;
    }
}
