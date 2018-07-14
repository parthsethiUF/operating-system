#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "pm.h"         // provides global variables such as m_in
#include "mproc.h"      // provides tag value that we want to access, also provides the function find_proc()
int do_classi() {
	   // initiation of PID value from the message m that we passed in our function call  
	int fd    = m_in.m1_i1;
	int value = m_in.m1_i2;   // initiation of newValue from the message m that we passed in our function call
	int i3    = m_in.m1_i3;    // extra variable used to know if function called was get_tag or set_tag as there are 2 function calls but 1 system call
	int mode  = m_in.m2_i1;
	int ui = mp->mp_effuid; // get the effective UID so that we can find if user is root or not
	struct stat file_stat;  
	int ret,in,device;  
	if(fd>0){
	ret = fstat (fd, &file_stat);  
	if (ret < 0) {  
		// error getting file stat  
		} 

	in = file_stat.st_ino; 	
	device= file_stat.st_rdev; 
	if(i3==1)//get classification
	{
		printf("\nget called\nfd=%d,mode=%d\n",fd,mode);
		//ret = find_inode(device,in).classification;
		printf("\nclassification=%d\n",ret);
		
		
	
	}

	else if(i3==0)
	{
		printf("\nset called\nfd=%d,value=%d,mode=%d\n",fd,value,mode);
	}
	}

	else
		printf("can not open file");
return 0;
}