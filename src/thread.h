#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include <string>
#include <functional>
#include <memory>
#include <sys/syscall.h>  
#include <unistd.h>
#include <semaphore.h>

#define gettid() syscall(SYS_gettid)
namespace frdmu {
    class Semaphore {
        public:
            Semaphore(unsigned int value = 0) {sem_init(&m_sem, 0, value);}
            ~Semaphore() {sem_destroy(&m_sem);}
            void wait() {sem_wait(&m_sem);}
            void notify() {sem_post(&m_sem);}
        private:
            sem_t m_sem;
    };

    class Mutex {
        public:
            Mutex() {pthread_mutex_init(&m_mutex, nullptr);}
            ~Mutex() {pthread_mutex_destroy(&m_mutex);}
            void lock() {pthread_mutex_lock(&m_mutex);}
            void unlock() {pthread_mutex_unlock(&m_mutex);}
        private:
            pthread_mutex_t m_mutex;
    };
    // 定义了Mutex 类(互斥锁)
    // 因为有许多种锁，所以可以定义一种模板类，构造时加锁，析构是解锁
    template <typename T>
    class LockVar {
        public:
            LockVar(T &lock): m_lock(lock) {
                m_lock.lock(); 
                lock_flag = true;
            }
            ~LockVar() {
                m_lock.unlock();
                lock_flag = false;
            }
            void lock() {
                if (!lock_flag) {
                    m_lock.lock();
                    lock_flag = true;
                }
            }
            void unlock() {
                if (lock_flag) {
                    m_lock.unlock();
                    lock_flag = false;
                }
            }
        private:
            T &m_lock;
            bool lock_flag;
    };
    typedef LockVar<Mutex> Lock;


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
            Semaphore m_sem;
    };
}


#endif
