#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
    pid_t child;
    child=fork();
    
 switch(child){
        case -1:
        perror("Error in fork"); // should i put perror????
        exit(1);

        case 0: //child
        //calc factorial of 4
        printf("Child Process started\n");
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
        printf("Parent Process started\n");
        int a=0;
        int b=1;
        int fib[16];
        fib[0]=a;
        fib[1]=b;
        for (int i=2;i<16;i++){
            int sum=a+b;
            a=b;
            b=sum;
            fib[i]=sum;
        }
        printf("The first 16 elements of the Fibonacci series are: ");
        for (int i=0;i<16;i++){
            printf("%d ",fib[i]);
        }

        wait(NULL);
        printf("\nParent process has terminated");
        break;
    }

    return 0;

}