#include <lib.h>      // provides _syscall and message
#include <unistd.h>   // provides function prototype

int get_tag(int PID) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_i1 = PID;  // set first integer of message to PID
    m.m1_i3 = 1;    //is used to differentiate between set tag and get tag, third integer is 1 for get_tag
    return _syscall(PM_PROC_NR, TAG, &m);  // invoke underlying system call
}