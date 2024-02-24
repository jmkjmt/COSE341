#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define my_stack_push 335
#define my_stack_pop 336
//return a^x
int power(int a, int x){
	int value = 1;
	for(int i=0;i<x;i++){
		value *= a;
	}
	return value;
}

//change string number to integer
int char2int(char * integer){
	int size=0;
	while(integer[size]!='\0'){
		size++;
	}
	int value =0;
	for(int i=0;i<size;i++){
		value+= (integer[i]-48) * power(10, size-i-1) ;
	}
	return value;
}


int main(void){
	char inst[5];//string of instruction(push or pop)
	char integer[4];// integer to push(type string)
	int x;//integer to push(type integer)
	while(1){
		scanf("%s", inst);
		if(!strcmp(inst,"Push")){
			scanf("%s",integer);
			x=char2int(integer);
			syscall(my_stack_push, x);
		}
		else if(!strcmp(inst,"Pop")){
			int r = syscall(my_stack_pop);
			if(r==-1){
				printf("Pop Error");
				break;
			}
			printf("%d\n",r);
		}
		else{
			printf("unvalid instruction!\n");
		}
	}
}
