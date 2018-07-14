#include <stdio.h>
#include "pm.h"         // provides global variables such as m_in
#include "mproc.h"      // provides tag value that we want to access, also provides the function find_proc()
int do_tag() {
    int PID = m_in.m1_i1;   // initiation of PID value from the message m that we passed in our function call  
    int value = m_in.m1_i2;   // initiation of newValue from the message m that we passed in our function call
    int i3 = m_in.m1_i3;    // extra variable used to know if function called was get_tag or set_tag as there are 2 function calls but 1 system call
    int ui = mp->mp_effuid; // get the effective UID so that we can find if user is root or not
    printf("System call do_settag called with value %d %d\n",mp->mp_pid, ui );
    if(PID < 1)
    {
       return -1;
 		}
    if(i3==1 &&( ui == 0 || PID == mp->mp_pid) )// gettag, i3 = 1 was set by me just to differentiate from set_tag, ui=0 means root, PID=mp->mp_pid means process is accessing its own tag
    {
        if(find_proc(PID)==NULL){
          value = -1; //if invalid PID is there
		    }

        else{
          value = find_proc(PID)->tag; //get the value of the 'process tag' which is acecssed by find_proc(PID)->tag, and saved to local variable value
        }
    }   
    else if(i3==0)//settag, i3=0 differentiates set_tag from get_tag
    {
      if{
	
		}
      if(ui == 0)//check for root user
	      {
    		find_proc(PID)->tag = value; //set tag of process as variable value
		//don't read, extra debugging code...................................
     	        //printf("System call do_settag called with value %d %d\n", PID,value);
		//...................................................................
 	      }
    }
    else{
        value = -1; //if any error occurs return -1
        }
	      //don't read, extra debugging code..........................................
              printf("System call do_settag called with value %d %d\n",mp->mp_pid, ui );
	      //..........................................................................
    return value;// return the variable value
}

