#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

/* p1test.c
 * by : R. Newman
 * last mod: 2/2/2017
 * 
 * Test project 1 - set_tag() and get_tag() calls.
 * should compile with (modified) system library
 * 
 * After compiling, make the executable owned by root and setuid
 * When testing, run the setuid executable as a non-root user
 *
 * Note that this test program does not verify that the tag is 
 * always initialized to 0.
 */

/* Test Cases
	root = euid == 0
	user = euid != 0 (non-root user)
	self = same PID as caller
	other = different PID than caller
	none = invalid PID or PID not in use
	valid = newtag >= 0
	invalid = newtag < 0
	any = newtag can have any value

	get_tag:
		caller	target	result
	 1. 	root	self	success
	 2. 	root	other	success
	 3.	root	none	failure
	 4.	user	self	success
	 5. 	user	other	failure
	 6.	user	none	failure

	set_tag:
		caller	target	newtag	result
	 7. 	root	self	valid	success
	 8. 	root	other	valid	success
	 9.	root	none	valid	failure
	10.	root	self	invalid	failure
	11. 	root	other	invalid	failure
	12.	root	none	invalid	failure
	13.	user	self	any 	failure
	14.	user	other	any	failure
	15.	user	none	any	failure

	for successful set_tag, there should be a get_tag that
		follows verifying that it took effect correctly

	Strategy:
	Make program setuid owned by root
	Fork first child process, have parent wait for child to die
	First child dies (so PID is known to be not in use)
	Fork second child process, have parent wait for child to die
	Child has euid = root so can do tests 1-3, 7-9, 10-12 
		using its own PID, its parent's PID, and dead sib's PID
	Have child setuid back to user and do tests 4-6 and 13-15
		using its own PID, its parent's PID, and dead sib's PID
	Have child display results and die
	After child is dead, parent dies
 */

enum proc {SELF, OTHER, NONE};

#define GTESTS 6
#define NTESTS 15
enum result {SUCCESS, FAILURE, NOTEST};
char *resStr[] = { "Success", "Failure" , "Test not performed" };
char *description[] = {
	"Initial tag value 0      - ", 
	"get tag by root on self  - ",
	"get tag by root on other - ",
	"get tag by root on none  - ",
	"get tag by user on self  - ",
	"get tag by user on other - ",
	"get tag by user on none  - ",
	"set tag by root on self  - ",
	"set tag by root on other - ",
	"set tag by root on none  - ",
	"set bad tag by root on self  - ",
	"set bad tag by root on other - ",
	"set bad tag by root on none  - ",
	"set tag by user on self  - ",
	"set tag by user on other - ",
	"set tag by user on none  - "
};

void printResults(int n, int a[]) {
    int i;

    for (i=0; i<=n; ++i) {
	printf("  Test %2d: %s %s\n", i, description[i],  resStr[a[i]]);
    }
}

void fillArray(int n, int value, int a[]) {
    int i;

    for (i=0; i<n; ++i) {
	a[i] = value;
    }
}


int test(int testnum, int PID, int tag, int fail, int a[]) {
    int rv, call_failed; 
    int weird = 0;

    printf("Test %2d - ", testnum);

    if (testnum <= GTESTS) { // get_tag
           	printf("get_tag for PID %d ", PID);
	if (call_failed = ((rv = get_tag(PID)) == -1) ) {
           	    printf("failed. ");
	}
	else printf("succeeded. ");
	if ( (call_failed && fail) || (!call_failed && !fail) ) {
	    a[testnum] = SUCCESS;
	    printf("SUCCESS\n");
	    return SUCCESS;
	}
	else {
	    a[testnum] = FAILURE;
	    printf("FAILURE\n");
	    return FAILURE;
	}
    }

    else { // set_tag
           	printf("set_tag to %d for PID %d ", tag, PID);
	rv = set_tag(PID,tag);

	if (call_failed = (rv == -1) ) {
           	    printf("failed. ");
	}
	else if (rv == tag) {
	    printf("succeeded. ");
	}
	else {
	    printf("returned %d! ", rv);
	    a[testnum] = FAILURE;
	    printf("FAILURE\n");
	    return FAILURE;
	}
	if ( (call_failed && fail) || (!call_failed && !fail) ) {
	    a[testnum] = SUCCESS;
	    printf("SUCCESS\n");
	    return SUCCESS;
	}
	else {
	    a[testnum] = FAILURE;
	    printf("FAILURE\n");
	    return FAILURE;
	}
    }
}

int initCheck() {	// should be run immediately after fork()
    int tag, me;

        printf("Checking my tag (init to 0): ");
        me = getpid();
        printf("PID = %d, ", me);
        tag = get_tag(me);
        printf("my tag = %d ", tag);
        if (tag == 0) {
	    printf("- correct\n");
	    return SUCCESS;
	}
        else {
	    printf("- incorrect; should be 0\n");
	    return FAILURE;
	}
}

int main() {
    int PID[NONE+1];
    int deadPID, myPID, otherPID; 
    int tag;
    int myUID, myEUID;
    int retval;
    int target[NTESTS+1];		// SELF, OTHER, or NONE
    int newtag[NTESTS+1];		// tag value for set_tag
    int expected[NTESTS+1];		// is call expected to succeed?
    int results[NTESTS+1];		// was test passed?
    int i;
    char c;

    fillArray(NTESTS, NOTEST, results);

    printf("\n*********** Project 1 Test ***********\n");

    myPID = getpid();
    printf("Parent PID = %d\n", myPID);
    myUID = getuid();
    printf("Parent UID = %d\n", myUID);
    myEUID = geteuid();
    printf("Parent EUID = %d\n", myEUID); 
    if (! ((myEUID == 0) && (myUID != 0)) ) { 
    	printf("\tWarning: For complete test EUID = 0 and UID != 0\n"); 
    	printf("\tMake program setuid owned by root and run from nonroot\n"); 
    }

    results[0] = initCheck();	// initial tag value should always be 0

    if ( (deadPID = fork()) == -1) {
    	printf("Fork 1 failed.\n");
	return 0;
    }
    else {
    	printf("Fork 1 suceeded.\n");
    }
    PID[NONE] = deadPID;

    if (deadPID == 0) { // first child
    	printf("\n    Child 1 \n");
        initCheck();	// initial tag value should always be 0

	return 0;		// die immediately
    }
	// else parent - wait for first child to die
    retval = wait();

    if ( (otherPID = fork()) == -1) {
    	printf("Fork 2 failed.\n");
	return 0;
    }
    else {
    	printf("Fork 2 suceeded.\n");
    }

    if (otherPID == 0) { // second child - should have euid == 0, uid != 0
    	printf("\n    Child 2 \n");

        if (results[0] == SUCCESS) { // don't change if parent failed test
		printf("child 22\n");
        	results[0] = initCheck();	// both have to pass test
	}
	else initCheck();	// run it for child 2 anyway and ignore result

    	myUID = getuid();
    	printf("Child UID = %d\n", myUID);
    	myEUID = geteuid();
    	printf("Child EUID = %d\n", myEUID);
   	myPID = getpid();
    	printf("Child: My PID = %d\n", myPID);
	PID[SELF] = myPID;

	// get parent's PID
   	otherPID = getppid();
	PID[OTHER] = otherPID;

    	printf("Child: Parent PID = %d\n", otherPID);
    	printf("Child: Dead sibling's PID = %d\n", deadPID);

/*
	get_tag:
		caller	target	result
	 1. 	root	self	success
	 2. 	root	other	success
	 3.	root	none	failure
	 4.	user	self	success
	 5. 	user	other	failure
	 6.	user	none	failure

	set_tag:
		caller	target	newtag	result
	 7. 	root	self	valid	success
	 8. 	root	other	valid	success
	 9.	root	none	valid	failure
	10.	root	self	invalid	failure
	11. 	root	other	invalid	failure
	12.	root	none	invalid	failure
	13.	user	self	any 	failure
	14.	user	other	any	failure
	15.	user	none	any	failure
*/

	for (i=1; i<=NTESTS; i+=3) {
		target[i] = SELF;
		target[i+1] = OTHER;
		target[i+2] = NONE;
	}

	for (i=0; i<=NTESTS; ++i) {
		expected[i] = FAILURE;
		newtag[i] = i;
	}
	expected[1] = expected[2] = expected[4] = expected[7] = 
		expected[8] = SUCCESS;		// only times call should work
	for (i=10; i<=12; ++i) newtag[i] *= -1;	// invalid tags
	
	if (myEUID != 0) {	// can't do root tests
	    printf("Skipping root tests\n");
	}
	else {			// do root tests	
	    printf("Performing root tests\n");
	    for (i=1; i<=3; ++i) 
	        test( i, PID[target[i]], newtag[i], expected[i], results) ;
	    for (i=7; i<=12; ++i) 
	        test( i, PID[target[i]], newtag[i], expected[i], results) ;
	}

	// ****************** change UID to calling user ****************

	retval = setuid(myUID);
    	myEUID = geteuid();
    	printf("Now Child EUID = %d\n", myEUID);

/*
	get_tag:
		caller	target	result
	 1. 	root	self	success
	 2. 	root	other	success
	 3.	root	none	failure
	 4.	user	self	success
	 5. 	user	other	failure
	 6.	user	none	failure

	set_tag:
		caller	target	newtag	result
	 7. 	root	self	valid	success
	 8. 	root	other	valid	success
	 9.	root	none	valid	failure
	10.	root	self	invalid	failure
	11. 	root	other	invalid	failure
	12.	root	none	invalid	failure
	13.	user	self	any 	failure
	14.	user	other	any	failure
	15.	user	none	any	failure
*/

	if (myEUID == 0) {	// can't do non-root tests! Still root!
	    printf("Skipping non-root tests\n");
	}
	else {			// do non-root tests	
	    printf("Performing non-root tests\n");
	    for (i=4; i<=6; ++i) 
	        test( i, PID[target[i]], newtag[i], expected[i], results) ;
	    for (i=13; i<=15; ++i) 
	        test( i, PID[target[i]], newtag[i], expected[i], results) ;
	}

	printf("\n \t RESULTS \n");
	printResults(NTESTS, results);
	return 0;
    }

    else { // ************************* parent *************************
	
	// wait for child to die 
	retval = wait();

	retval = kill(otherPID,0);	// check that child is dead
	if (retval == -1) {	// should be 0 = no error
	}
	else {
	    printf("Parent: child not dead - KILL KILL KILL!\n");
	    retval = kill(otherPID,SIGKILL);	// OK, kill child 
	    // should check for success or that errno = ESRCH
	}
	retval = wait();
	return 0;
    }

    return 0;	// just in case
}

