#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include <string>
#include <functional>
#include <memory>
#include <sys/syscall.h>  
#include <unistd.h>

#define gettid() syscall(SYS_gettid)
namespace frdmu {
    class Thread {
        public:
            typedef std::shared_ptr<Thread> ptr;
            Thread() = default;
            Thread(std::function<void()> cb, std::string name);
            static void *run(void* args); // 为什么必须是static?
            
            void join();

            ~Thread();
        
            pid_t getId() const {return m_id;}
            std::string getName() const {return m_name;}
            
        public: 
            static pid_t GetId(); 
            static const std::string &GetName(); 
        private:
            pid_t m_id;
            pthread_t m_thread;
            std::string m_name;
            std::function<void()> m_cb;
    };
}


#endif
