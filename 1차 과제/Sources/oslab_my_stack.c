#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

int stack[30];
int top = 0;

//function to show stack element
void showstack(void){
	printk("Stack Top ---------------------------"    );
    int i;
	for(i=top-1;i>=0;i--){
        printk("%d\n", stack[i]);
    }
    printk("Stack bottom-------------------------"    );
}
//stack push
SYSCALL_DEFINE1(os2023_push, int, a){
	if(top>30){
		printk("no memory to push");
		return;
	}
	int i;
	for(i=0;i<top;i++){
        if(a==stack[i])
            return;
    }
    stack[top++]=a;
    showstack();
}
//stack pop
SYSCALL_DEFINE0(os2023_pop){
	if(top==0){
		printk("no element to pop");
		return -1;
	}
    int value = stack[--top];
    showstack();
    
    return value;
}
