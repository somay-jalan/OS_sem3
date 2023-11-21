#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>

#define sleeptime 1
#define no_of_loops 1
 
pthread_mutex_t fork1,fork2,fork3,fork4,fork5;
pthread_mutex_t bowl1,bowl2;
int cvcheck=0;

long arr[2];

void eating(long philos_num){
    if(philos_num==1){
        pthread_mutex_lock(&fork1);
        printf("Philosopher 1 has fork 1\n");
        pthread_mutex_lock(&fork5);
        printf("Philosopher 1 has fork 5\n");
        int var;
        for(int i=0;i<2;i++){
            if(arr[i]==-1){
                arr[i]=philos_num;
                var=i;
                break;
            }
        }
        printf("Philosoher numner %ld and %ld have the bowl.(-1 means that bowl is empty)\n",arr[0],arr[1]);
        // printf("")
        printf("philospher number:%ld is eating\n",philos_num);
        sleep(sleeptime);
        arr[var]=-1;
        pthread_mutex_unlock(&fork1);
        pthread_mutex_unlock(&fork5);
        cvcheck--;

    }else if(philos_num==2){
        pthread_mutex_lock(&fork2);
        printf("Philosopher 2 has fork 2\n");
        pthread_mutex_lock(&fork1);
        printf("Philosopher 2 has fork 1\n");
        int val;
        for(int i=0;i<2;i++){
            if(arr[i]==-1){
                arr[i]=philos_num;
                val=i;
                break;
            }
        }
        printf("Philosoher numner %ld and %ld have the bowl.(-1 means that bowl is empty)\n",arr[0],arr[1]);
        printf("philospher number:%ld is eating\n",philos_num);
        sleep(sleeptime);

        arr[val]=-1;
        pthread_mutex_unlock(&fork2);
        pthread_mutex_unlock(&fork1);
    

    }else if(philos_num==3){
        pthread_mutex_lock(&fork3);
        printf("Philosopher 3 has fork 3\n");
        pthread_mutex_lock(&fork2);
        printf("Philosopher 3 has fork 2\n");
        int val;
        for(int i=0;i<2;i++){
            if(arr[i]==-1){
                arr[i]=philos_num;
                val=i;
                break;
            }
        }
        printf("Philosoher numner %ld and %ld have the bowl.(-1 means that bowl is empty)\n",arr[0],arr[1]);
        printf("philospher number:%ld is eating\n",philos_num);
        sleep(sleeptime);

        arr[val]=-1;
        pthread_mutex_unlock(&fork3);
        pthread_mutex_unlock(&fork2);

    }else if(philos_num==4){
        pthread_mutex_lock(&fork4);
        printf("Philosopher 4 has fork 4\n");
        pthread_mutex_lock(&fork3);
        printf("Philosopher 4 has fork 3\n");
        int val;
        for(int i=0;i<2;i++){
            if(arr[i]==-1){
                arr[i]=philos_num;
                val=i;
                break;
            }
        }
        printf("Philosoher numner %ld and %ld have the bowl.(-1 means that bowl is empty)\n",arr[0],arr[1]);
        printf("philospher number:%ld is eating\n",philos_num);
        sleep(sleeptime);

        arr[val]=-1;
        pthread_mutex_unlock(&fork4);
        pthread_mutex_unlock(&fork3);
        

    }else if(philos_num==5){
        pthread_mutex_lock(&fork4);
        printf("Philosopher 5 has fork 4\n");
        pthread_mutex_lock(&fork5);
        printf("Philosopher 5 has fork 5\n");
        int val;
        for(int i=0;i<2;i++){
            if(arr[i]==-1){
                arr[i]=philos_num;
                val=i;
                break;
            }
        }
        printf("Philosoher numner %ld and %ld have the bowl.(-1 means that bowl is empty)\n",arr[0],arr[1]);
        printf("philospher number:%ld is eating\n",philos_num);
        sleep(sleeptime);

        arr[val]=-1;
        pthread_mutex_unlock(&fork4);
        pthread_mutex_unlock(&fork5);
    }else{
        printf("SOME ERROR\n");
    }
}

void thinking(long philos_num){
    printf("philospher number:%ld is thinking\n",philos_num);
    sleep(sleeptime);
}

void *philospher(void *args){
    long philospher_num=(long)(args);
    printf("philospher number:%ld\n",philospher_num);
    thinking(philospher_num);
    eating(philospher_num);
}

int main(int argc, char *argv[]){
    arr[0]=-1;
    arr[1]=-1;
    for(int i=0;i<no_of_loops;i++){
        printf("------------------------RUNNING ANOTHER LOOP-----------------\n");
        printf("LOOP NUMBER:%d\n",i+1);
        pthread_t phil1,phil2,phil3,phil4,phil5;
        int rc;
        rc=pthread_create(&phil1,NULL,philospher,(void*)1);assert(rc==0);
        rc=pthread_create(&phil2,NULL,philospher,(void*)2);assert(rc==0);
        rc=pthread_create(&phil3,NULL,philospher,(void*)3);assert(rc==0);
        rc=pthread_create(&phil4,NULL,philospher,(void*)4);assert(rc==0);
        rc=pthread_create(&phil5,NULL,philospher,(void*)5);assert(rc==0);
        rc=pthread_join(phil1,NULL);assert(rc==0);
        rc=pthread_join(phil2,NULL);assert(rc==0);
        rc=pthread_join(phil3,NULL);assert(rc==0);
        rc=pthread_join(phil4,NULL);assert(rc==0);
        rc=pthread_join(phil5,NULL);assert(rc==0);
    }
    return 0;
}