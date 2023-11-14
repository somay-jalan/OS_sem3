#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>
#include <semaphore.h> 

sem_t passing,left_go,right_go,left_done,right_done;
int car_left,car_right;
int done=0;
void passing_func(){
    sleep(5);
}

void *left(void* args){
    long car_num=(long)(args);
    sem_wait(&left_go);
    sem_wait(&passing);
    int car_on_bridge;
    sem_getvalue(&left_go,&car_on_bridge);
    printf("Car number %ld on left is passing.\n",car_num);
    // printf("Number of cars on bridge are:%d\n",5-car_on_bridge);
    done++;
    // printf("Car number %ld on left has passed.\n",car_num);
    if(done==car_left){
        sem_post(&left_done);
    }
    sem_post(&passing);
    passing_func();
    sem_post(&left_go);
    
}

void *right(void* args){
    long car_num=(long)(args);
    sem_wait(&right_go);
    passing_func();
    sem_wait(&passing);
    int car_on_bridge;
    sem_getvalue(&right_go,&car_on_bridge);
    printf("Car number %ld on right is passing.\n",car_num);
    // printf("Number of cars on bridge are:%d\n",5-car_on_bridge);
    done++;
    // printf("Car number %ld on right has passed.\n",car_num);
    if(done==car_right){
        sem_post(&right_done);
    }
    sem_post(&passing);
    sem_post(&right_go);
    
}

int main(int argc, char *argv[]){
    pthread_t left_thread,right_thread;
    printf("Enter the number on left side of the bridge:");
    scanf("%d",&car_left);
    printf("Enter the number on right side of the bridge:");
    scanf("%d",&car_right);
    sem_init(&passing,0,0);
    sem_init(&left_go,0,5);
    sem_init(&left_done,0,0);
    sem_init(&right_go,0,5);
    sem_post(&passing);
    int rc;
    printf("LEFT SIDE:\n");
    for(int i=0;i<car_left;i++){
        rc=pthread_create(&left_thread,NULL,left,(void*)(long)i+1);
    }
    sem_wait(&left_done);
    printf("LEFT SIDE DONE.\n");
    printf("RIGHT SIDE:\n");
    done=0;
    for(int i=0;i<car_right;i++){
        rc=pthread_create(&right_thread,NULL,right,(void*)(long)i+1);
    }
    sem_wait(&right_done);
    printf("RIGHT SIDE DONE.\n");
    return 0;
}