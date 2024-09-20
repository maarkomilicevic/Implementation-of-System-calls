#include "../h/syscall_cpp.hpp"  // Include the header file where declarations are defined
#include "../h/tcb.hpp"  // Include the header file where declarations are defined


Thread::Thread(void (*body)(void*), void* arg) : body(body), arg(arg) {}

Thread::~Thread() {}

int Thread::start() {
    thread_create(&myHandle, body, (void*) arg);
    return 0;
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    time_sleep(t);
    return 0;
}

void Thread::threadWraper(void* thread) {
    ((Thread*)thread)->run();
}

Thread::Thread() {
    body = &threadWraper;
    arg = this;
}


Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle,  init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);

}

int Semaphore::signal() {
    return sem_signal(myHandle);

}



PeriodicThread::PeriodicThread(time_t period) : Thread(pWrapper, (void*) this), period(period), cond(true)  {

}

void PeriodicThread::terminate() {
    cond = false;
}

void PeriodicThread::pWrapper(void * t) {
    PeriodicThread* thread = (PeriodicThread*) t;
    while(thread->cond){
        thread->periodicActivation();
        sleep(thread->period);
    }
}


char Console::getc() {

    return  ::getc();;
}

void Console::putc(char c) {
    ::putc(c);
}
