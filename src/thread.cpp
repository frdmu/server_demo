#include "thread.h"
#include <iostream>
namespace frdmu {
    thread_local pid_t thread_id = -1;
    thread_local std::string thread_name = "UNKNOW";
    
    Thread::Thread(std::function<void()> cb, std::string name): m_cb(cb), m_name(name) {
        if (name.empty()) {
            m_name = "UNKNOW";
        } 
        int ret = pthread_create(&m_thread, nullptr, run, this);
        if (ret != 0) {
            std::cout << "construct thread failed" << std::endl;
        }
        m_sem.wait();
    }
    
    void *Thread::run(void* args) {
        Thread *thread = (Thread *)args; 
        thread->m_id = gettid();
        
        // 这里容易出错 thread_local，在每个线程里赋值   
        thread_id = thread->m_id;
        thread_name = thread->m_name; 
       
        thread->m_sem.notify();
        thread->m_cb();

        return nullptr;
    }
    //
    // join 与deatch 的区别：
    // join 阻塞
    // deatch 非阻塞
    // 都是用来回收系统资源
    //
    void Thread::join() {
        if (m_thread) {
            int ret = pthread_join(m_thread, nullptr);
            if (ret != 0) {
                std::cout << "join failed" << std::endl;
            }
            m_thread = 0; // 这里很重要，线程回收成功的标志 
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
