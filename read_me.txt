PARTH SETHI | UFID: 7213-6121 | COP4600: Operating Systems, Spring 2017
Report: Project 1, System Call
Minix System Call
To make a system call following steps were followed (along with explanations why to change various files, What done and Why done):
1. Add a table entry, which assigns a call number to the system call. Number 69 was unused so it was used and system call was given name do_tag.
   So I had to edit the file /usr/src/servers/pm/table.c  and replace the line:
    no_sys,             /* 69 = unused  */         
		with
    do_tag,           /* 69 = do_tag  */
2. Define a macro for system call, so that there is no need of call number. For this file           
     “/usr/src/include/minix/callnr.h” was edited, one extra line was typed in
      #define TAG   69
     So now if user wants to access this system call, user will access it by the name TAG, it corresponds to the call that was defined in step 1.
3. Defining a function prototype:
   To add declaration of function prototype, In the file “/usr/src/servers/pm/proto.h” 
   add this line at the “misc.c” end part
      int do_tag(void);
      
   Making do_tag.c file:
   To make do_tag.c, I made a new file in “/usr/src/servers/pm” where all of code will be written for this code that sets or gets the tag. Initially just a 
   Print "System call do_printmsg called” to assure successful system call later all the final code was written. Now to add this c file to be compiled add “do_tag.c”  in 
   /usr/src/servers/pm/Makefile inside SRCS(at the end) so after rebuilding, this c file is added and  compiled.
   To compile the c file made, in /usr/src/releasetools  type in make services and after that completes type in make install and then a reboot.    
4. Defining function prototype:
   Add these lines; “int set_tag(int PID, int newvalue); 
                     int get_tag(int PID)”;                         
   just before _END_minix in  
   /usr/src/include/unistd.h 
   so that when user includes unistd header file, user can use these two functions.

   Making those functions:
   In  /usr/src/lib/libc/sys-minix create a C files as get_tag.c and set_tag.c
   Add set_tag.c and get_tag.c to the list of SRCS in /usr/src/lib/libc/sys-minix/Makefile.inc 
   so that when we rebuild OS theses files are included in it too.
5. In /usr/src type in make build (that can take 20 minutes) and after that reboot.
6. Add in /usr/src/servers/pm/mproc.h “int tag;” ; it is the variable that will be set or get from our function.
7. Run the c test files provided on canvas and try all different test cases.
**********************************************************************************
Changes made to existing files:
1. in /usr/src/servers/pm/table.c added an entry for system call that assigns it a  call number.
2. in /usr/src/include/minix/callnr.h added a call number definition (TAG for number 69) for the table entry.
3. in /usr/src/servers/pm/proto.h defined the system call's function prototype                          (int do_tag(void)).
4. In /usr/src/servers/pm/Makefile added do_tag.c
5. In /usr/src/include/unistd.h added the function prototypes for get_tag and set_tag.
6. In /usr/src/servers/pm/Makefile added “get_tag.c” and “set_tag.c”
7. In /usr/src/servers/pm/mproc.h added “int tag;”.
Explanations why these modifications were made are explained earlier. 
**********************************************************************************
Testing:
Two files were given on canvas which accessed those function calls,
Following steps were followed:
Logged in as root user:
Accessed executable files gettag.o and settag.o;  executable file call and outputs are shown below 
================================
$ ./gettag 123
0
$ ./settag 123 5
5
$ ./gettag 123
5
================================
Logged in as non root user “foo”
Accessed executable files gettag.o and settag.o; executable file call and outputs are shown below (949 will be the process reading its own PID below)
================================
$ ./gettag 123
get_tag failed
Usage: gettag<PID>
$ ./settag 123 5
Not root user
5
$ ./gettag 949
0
================================
p1test.c was also tested with all the test successful
**********************************************************************************
Bugs:
There was only one found 
When non root user call sat_tag though output is “Not root user” but newValue though not set anywhere, 
is just displayed, like there was 5 printed in test case shown above.

