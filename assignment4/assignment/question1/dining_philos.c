#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>

pthread_mutex_t fork1,fork2,fork3,fork4,fork5;

void eating(long philos_num){
    printf("philospher number:%ld is eating\n",philos_num);
    if(philos_num==1){
        pthread_mutex_lock(&fork1);
        pthread_mutex_lock(&fork5);
        sleep(2);
        pthread_mutex_unlock(&fork1);
        pthread_mutex_unlock(&fork5);

    }else if(philos_num==2){
        pthread_mutex_lock(&fork2);
        pthread_mutex_lock(&fork1);
        sleep(2);
        pthread_mutex_unlock(&fork2);
        pthread_mutex_unlock(&fork1);

    }else if(philos_num==3){
        pthread_mutex_lock(&fork3);
        pthread_mutex_lock(&fork2);
        sleep(2);
        pthread_mutex_unlock(&fork3);
        pthread_mutex_unlock(&fork2);

    }else if(philos_num==4){
        pthread_mutex_lock(&fork4);
        pthread_mutex_lock(&fork3);
        sleep(2);
        pthread_mutex_unlock(&fork4);
        pthread_mutex_unlock(&fork3);

    }else if(philos_num==5){
        pthread_mutex_lock(&fork4);
        pthread_mutex_lock(&fork5);
        sleep(2);
        pthread_mutex_unlock(&fork4);
        pthread_mutex_unlock(&fork5);
    }else{
        printf("SOME ERROR\n");
    }
}

void thinking(long philos_num){
    printf("philospher number:%ld is thinking\n",philos_num);
    sleep(2);
}

void *philospher(void *args){
    long philospher_num=(long)(args);
    printf("philospher number:%ld\n",philospher_num);
    thinking(philospher_num);
    eating(philospher_num);
}

int main(int argc, char *argv[]){
    for(int i=0;i<5;i++){
        printf("------------------------RUNNING ANOTHER LOOP-----------------\n");
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

