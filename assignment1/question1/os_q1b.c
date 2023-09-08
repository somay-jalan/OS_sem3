#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
    pid_t child;
    child=vfork();
    
 switch(child){
        case -1:
        perror("Error in fork"); 
        exit(1);

        case 0: //child
        //calc factorial of 4
        printf("Child process has started\n");
        int fac=1;
        for(int i=1;i<5;i++){
            fac*=i;
        }
        printf("The Factorial of 4 is %d",fac);
        printf("\nChild process has terminated\n");
        exit(2);
        break;

        default: //parent
        // calculate fibonacci till 16
        printf("Parent process has started\n");
        wait(NULL);
        int a=0;
        int b=1;
        int fib[16];
        fib[0]=a;
        fib[1]=b;
        printf("The first 16 elements of the Fibonacci series are: ");
        printf("[1]:%d", fib[0]);
        printf("[2]:%d", fib[1]);
        for (int i=2;i<16;i++){
            int sum=a+b;
            a=b;
            b=sum;
            fib[i]=sum;
            printf("[%d]:%d\n",i+1,fib[i]);
        }
        printf("\nParent process has terminated");
        break;
    }

    return 0;

}