
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define NUMTESTS 24
	
enum TestResult{SUCCESS,FAILURE} TestResults[NUMTESTS];
	
void printresults(){
	for(int i = 0; i < NUMTESTS; i++){
		if(TestResults[i] == SUCCESS){
			printf("TEST %d : SUCCESS\n",i);
		}
		else if(TestResults[i] == FAILURE){
			printf("TEST %d : FAILURE\n",i);
		}
		else{
			printf("TEST %d : UNDEFINED\n",i);
		}
	}
}

int main(int argc, char** argv) {
	
	int result;
	
	// 0. Create sets tag to process's tag
	// 1. Errno displays no error
	// 2. FD invalid fails
	// 3. Errno displays error
	// 4. Negative classification fails
	// 5. Errno displays error
	// 6-14. Test read downs and write ups
	// 14-23. Change tag then read down and write up
	printf("0.0");
	int currentProcessPid = getpid();
	printf("0");
	set_tag(currentProcessPid,8);
	printf("1");
	FILE* fd = fopen("./log/log.txt","a");
	FILE* fd1 = fopen("./log/log1.txt","a");
	FILE* fd2 = fopen("./log/log2.txt","a");
	FILE* fd3 = fopen("./log/log3.txt","a");
	FILE* fd4 = fopen("./log/log4.txt","a");
	FILE* fd5 = fopen("./log/log5.txt","a");
	FILE* failfd = fopen("./log/log6.txt","a");
	printf("2");
	
	result = get_class(fd1);
	
	if(result == 8){
		TestResults[0] = SUCCESS;
	}
	else{
		TestResults[0] = FAILURE;
	}
	
	if(errno != -1){
		TestResults[1] = SUCCESS;
	}
	else{
		TestResults[1] = FAILURE;
	}
	
	result = set_class(failfd,9999);
	
	if(result == -1){
		TestResults[2] = SUCCESS;
	}
	else{
		TestResults[2] = FAILURE;
	}
	
	if(errno == -1){
		TestResults[3] = SUCCESS;
	}
	else{
		TestResults[3] = FAILURE;
	}
	
	result = set_class(fd1,-99);
	
	
	if(result == -1){
		TestResults[4] = SUCCESS;
	}
	else{
		TestResults[4] = FAILURE;
	}
	
	if(errno == -1){
		TestResults[5] = SUCCESS;
	}
	else{
		TestResults[5] = FAILURE;
	}
	
	set_tag(currentProcessPid,3);
	
	set_class(fd1,1);
	set_class(fd2,2);
	set_class(fd3,3);
	set_class(fd4,4);
	set_class(fd5,5);
	
	fd1 = fopen("./log/log1.txt","r");
	
	if(fd1 != NULL){
		TestResults[6] = SUCCESS;
	}
	else{
		TestResults[6] = FAILURE;
	}
	
	fd1 = fopen("./log/log1.txt","w");
	
	if(fd1 == NULL){
		TestResults[7] = SUCCESS;
	}
	else{
		TestResults[7] = FAILURE;
	}
	
	fd1 = fopen("./log/log1.txt","w+");
	
	if(fd1 == NULL){
		TestResults[8] = SUCCESS;
	}
	else{
		TestResults[8] = FAILURE;
	}
	
	fd3 = fopen("./log/log3.txt","r");
	
	if(fd3 != NULL){
		TestResults[9] = SUCCESS;
	}
	else{
		TestResults[9] = FAILURE;
	}
	
	fd3 = fopen("./log/log3.txt","w");
	
	if(fd3 != NULL){
		TestResults[10] = SUCCESS;
	}
	else{
		TestResults[10] = FAILURE;
	}
	
	fd3 = fopen("./log/log3.txt","w+");
	
	if(fd3 != NULL){
		TestResults[11] = SUCCESS;
	}
	else{
		TestResults[11] = FAILURE;
	}
	
	fd5 = fopen("./log/log5.txt","r");
	
	if(fd5 == NULL){
		TestResults[12] = SUCCESS;
	}
	else{
		TestResults[12] = FAILURE;
	}
	
	fd5 = fopen("./log/log5.txt","w");
	
	if(fd5 != NULL){
		TestResults[13] = SUCCESS;
	}
	else{
		TestResults[13] = FAILURE;
	}
	
	fd5 = fopen("./log/log5.txt","w+");
	
	if(fd5 == NULL){
		TestResults[14] = SUCCESS;
	}
	else{
		TestResults[14] = FAILURE;
	}
	
	set_tag(currentProcessPid,4);
	
	fd3 = fopen("./log/log3.txt","r");
	
	if(fd3 != NULL){
		TestResults[15] = SUCCESS;
	}
	else{
		TestResults[15] = FAILURE;
	}
	
	fd3 = fopen("./log/log3.txt","w");
	
	if(fd3 == NULL){
		TestResults[16] = SUCCESS;
	}
	else{
		TestResults[16] = FAILURE;
	}
	
	fd3 = fopen("./log/log3.txt","w+");
	
	if(fd3 == NULL){
		TestResults[17] = SUCCESS;
	}
	else{
		TestResults[17] = FAILURE;
	}
	
	
	fd4 = fopen("./log/log4.txt","r");
	
	if(fd4 != NULL){
		TestResults[18] = SUCCESS;
	}
	else{
		TestResults[18] = FAILURE;
	}
	
	fd4 = fopen("./log/log4.txt","w");
	
	if(fd4 != NULL){
		TestResults[19] = SUCCESS;
	}
	else{
		TestResults[19] = FAILURE;
	}
	
	fd4 = fopen("./log/log4.txt","w+");
	
	if(fd4 != NULL){
		TestResults[20] = SUCCESS;
	}
	else{
		TestResults[20] = FAILURE;
	}
	
	fd5 = fopen("./log/log5.txt","r");
	
	if(fd5 == NULL){
		TestResults[21] = SUCCESS;
	}
	else{
		TestResults[21] = FAILURE;
	}
	
	fd5 = fopen("./log/log5.txt","w");
	
	if(fd5 != NULL){
		TestResults[22] = SUCCESS;
	}
	else{
		TestResults[22] = FAILURE;
	}
	
	fd5 = fopen("./log/log5.txt","w+");
	
	if(fd5 == NULL){
		TestResults[23] = SUCCESS;
	}
	else{
		TestResults[23] = FAILURE;
	}
	
	printresults();
	
	return 0;
}

