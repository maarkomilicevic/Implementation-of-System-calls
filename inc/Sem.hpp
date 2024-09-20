//
// Created by os on 8/19/23.
//

#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP

#include "list.hpp"
#include "tcb.hpp"

class Sem {
public:
    Sem ( int init) : val(init){}

    void wait();
    void signal();

    int value() const {return val;}

protected:
    void block ();
    void unblock();

private:
    int val;
    List<TCB> blocked;
    friend int _sem_close(sem_t);

};


#endif //PROJECT_BASE_V1_1_SEM_HPP
