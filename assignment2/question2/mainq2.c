#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sched.h> 
#include<sys/types.h>
#include<sys/wait.h>
#include<math.h>
#include <time.h>

int main(){
    int parent_pid=getpid();
    //sched_setaffinity
    for (int i=0;i<3;i++){
        if(getpid()==parent_pid){
            pid_t child=fork();
            if (i==0){//first child
                if(child<0){
                    perror;
                    exit(1);
                }
                else if (child==0){ //case we need
                    struct timespec timestart,timeend;
                    clock_gettime(CLOCK_REALTIME,&timestart );
                    //clock starts idhar
                    printf("Child A starts executing\n");
                    // struct sched_param* parameter;
                    //parameter->sched_priority=0;
                    // sched_setscheduler(child,SCHED_OTHER,parameter);//set scheduler
                    execl("./one","./one",NULL);
                    pid_t a=getppid();
                    int status;
                    
                    waitpid(a,&status,0);
                    printf("status%d\n",status);
                    // if (waitpid(a,&status,1)==a){
                    //     clock_gettime( CLOCK_REALTIME, &timeend );
                    // }
                    printf("FOR OTHER%ld\n",timeend.tv_nsec-timestart.tv_nsec);
                }
                
            }

            else if (i==1){//second child
                if(child<0){
                    perror;
                    exit(1);
                }

                else if(child==0){//case we need
                struct timespec timestart,timeend;
                clock_gettime(CLOCK_REALTIME,&timestart );
                printf("Child B starts executing\n");
                struct sched_param* parameter;
                //default value
                //parameter->sched_priority=1;
                sched_setscheduler(child,SCHED_RR,parameter);//set scheduler
                execl("./one","./one",NULL);
                pid_t b=getppid();
                int status;
                waitpid(b,&status,0);
                printf("status%d\n",status);
                // if (waitpid(a,&status,1)==a){
                //     clock_gettime( CLOCK_REALTIME, &timeend );
                // }
                printf("FOR RR%ld\n",timeend.tv_nsec-timestart.tv_nsec);
                }
            }

            else if (i==2){//third child
                if(child<0){
                    perror;
                    exit(1);
                }

                else if(child==0){//case we need
                struct timespec timestart,timeend;
                clock_gettime(CLOCK_REALTIME,&timestart );
                printf("Child C starts executing\n");
                struct sched_param* parameter;
                //default value 
                //parameter->sched_priority=1;
                sched_setscheduler(child,SCHED_FIFO,parameter);//set scheduler
                execl("./one","./one",NULL);
                pid_t c=getppid();
                int status;
                waitpid(c,&status,0);
                printf("status%d\n",status);
                // if (waitpid(a,&status,1)==a){
                //     clock_gettime( CLOCK_REALTIME, &timeend );
                // }
                printf("FOR FIFO%ld\n",timeend.tv_nsec-timestart.tv_nsec);
                }
            
            }        
        }
    }
    return 0;
}
