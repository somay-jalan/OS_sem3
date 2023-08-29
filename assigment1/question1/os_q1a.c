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

        case 0: //child process
        printf("C)Child is having ID <%d> \n",getpid());
        printf("D)My Parent ID is <%d> \n",getppid());
        break;

        default: //parent process
        printf("A)Parent (P) is having ID <%d> \n",getpid());
        wait(NULL);
        printf("B) ID of P's Child is <%d> \n",child);
        break;
    }

    return 0;

}


