#define ROW (100)
#define COL ROW
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <memory.h>
#include <sys/syscall.h>
#include <errno.h>
int proc_num;
int exetime;
int my_pid;
double execute_time;
int count;
struct sched_attr{
	uint32_t size;
	uint32_t sched_policy;
	uint64_t sched_flags;
	int32_t sched_nice;

	uint32_t sched_priority;

	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;
};
static int sched_setattr(pid_t pid, const struct sched_attr* attr, unsigned int flags){
	return syscall(SYS_sched_setattr, pid, attr, flags);
}

int calc(){
	int matrixA[ROW][COL];
	int matrixB[ROW][COL];
	int matrixC[ROW][COL];
	int i,j,k;
	count = 0;
	struct timespec begin, end, entire;
	clock_gettime(CLOCK_MONOTONIC, &begin);
	clock_gettime(CLOCK_MONOTONIC, &entire);

	while(1){
		for(i = 0;i<ROW;i++){
			for(j=0;j<COL;j++){
				for(k=0;k<COL;k++){
					matrixC[i][j] +=matrixA[i][j]*matrixB[i][j];

				}
			}
		}
		count++;
		clock_gettime(CLOCK_MONOTONIC, &end);
		execute_time = (end.tv_sec - entire.tv_sec)*1000+(end.tv_nsec - entire.tv_nsec)/1000000.0;
		double time = (end.tv_sec - begin.tv_sec)*1000+(end.tv_nsec - begin.tv_nsec)/1000000.0;
		if(time>100){
			printf("PROCESS # %02d count = %d time = %f\n",my_pid, count, time);
			clock_gettime(CLOCK_MONOTONIC, &begin);
		}
		if(execute_time > exetime*1000){
			printf("DONE!! PROCESS # %02d totalCount = %d time = %f\n",my_pid,count,execute_time);
			exit(0);
		}
	}
}
void handle_sigint(){
	printf("DONE!! PROCESS # %02d totalCount = %d time = %f\n",my_pid,count,execute_time);
	exit(0);
}
void handle_sigint_parent(){
	for(int i=0;i<proc_num;i++){
		wait(NULL);	
	}
	exit(0);
}
int main(int argc, char* argv[]){
	struct sched_attr attr;

	memset(&attr,0,sizeof(attr));
	attr.size=sizeof(attr);
	attr.sched_priority=10;
	attr.sched_policy = SCHED_RR;
	if(sched_setattr(getpid(),&attr,0)==-1){
		int err=errno;
		char * msg;
		msg=strerror(err);
		printf("sched set error: %s\n",msg);
	}

	proc_num=atoi(argv[1]);
	exetime=atoi(argv[2]);
	signal(SIGINT,handle_sigint_parent);
	pid_t pid;
	for(int i=0;i<proc_num;i++){
		my_pid=i;
		printf("Creating Process: # %d\n",my_pid);
		pid=fork();
		if(pid==0){
			break;
		}
	}
	if(pid==0){
		signal(SIGINT,handle_sigint);
		calc();
	}
	for(int i=0;i<proc_num;i++){
		wait(NULL);
	}
}