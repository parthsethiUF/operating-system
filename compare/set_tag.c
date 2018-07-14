#include <lib.h>      // provides _syscall and message
#include <unistd.h>   // provides function prototype 

int set_tag(int PID, int newvalue) {
    message m;      // Minix message to pass parameters to a system call
    
    m.m1_i1 = PID;  // set first integer of message to PID
    m.m1_i2 = newvalue; // set second integer of message to newValue
    m.m1_i3 = 0; //is used to differentiate between set tag and get tag, third integer is 0 for set_tag
    return _syscall(PM_PROC_NR, TAG, &m);  // invoke underlying system call
}