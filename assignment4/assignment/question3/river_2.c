#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>
#include <semaphore.h> 

sem_t passing,left_go,right_go,left_done,binary,done;
int car_left,car_right,car_left_left,car_left_right,left_go_count,right_go_count;
void passing_func(){
    sleep(1);
}

void *left(void* args){
    long car_num=(long)(args);
    sem_wait(&left_go);
    sem_wait(&passing);
    int car_on_bridge;
    sem_getvalue(&left_go,&car_on_bridge);
    printf("Car number %ld on left is passing.\n",car_num);
    // printf("Number of cars on bridge are:%d\n",5-car_on_bridge);
    // done++;
    // printf("Car number %ld on left has passed.\n",car_num);
    // if(done==car_left_left){
    //     done=0;
    //     for(int i=0;i<5;i++){
    //         sem_post(&right_go);
    //     }
    // }
    sem_post(&passing);
    // printf("Car number %ld on left has passed.\n",car_num);
    passing_func();
    sem_wait(&binary);

    left_go_count--;
    // printf("right_go_count:%d,left_go_count:%d,car_left:%d,car_right:%d\n",right_go_count,left_go_count,car_left,car_right);
    if(right_go_count==0){
        sem_post(&left_go);
    }else{
        if(car_left-5==left_go_count || left_go_count==0){
            car_left-=5;
            for(int i=0;i<5;i++){
                sem_post(&right_go);
            }
        }
    }
    if(left_go_count==0 && right_go_count==0){
        sem_post(&done);
    }
    sem_post(&binary);
    // sem_post(&left_go);

}

void *right(void* args){
    long car_num=(long)(args);
    sem_wait(&right_go);
    sem_wait(&passing);
    int car_on_bridge;
    sem_getvalue(&right_go,&car_on_bridge);
    printf("Car number %ld on right is passing.\n",car_num);
    // printf("Number of cars on bridge are:%d\n",5-car_on_bridge);
    // done++;
    // printf("Car number %ld on right has passed.\n",car_num);
    sem_post(&passing);
    // printf("Car number %ld on right has passed.\n",car_num);

    passing_func();
    sem_wait(&binary);
    right_go_count--;
    // printf("right_go_count:%d,left_go_count:%d,car_left:%d,car_right:%d\n",right_go_count,left_go_count,car_left,car_right);
    if(left_go_count==0){
        sem_post(&right_go);
    }else{
        if(car_right-5==right_go_count || right_go_count==0){
            car_right-=5;
            for(int i=0;i<5;i++){
                sem_post(&left_go);
            }
        }
    }
    if(left_go_count==0 && right_go_count==0){
        sem_post(&done);
    }
    sem_post(&binary);

    // sem_post(&right_go);

}

int main(int argc, char *argv[]){
    pthread_t left_thread,right_thread;
    printf("Enter the number on left side of the bridge:");
    scanf("%d",&car_left);
    pthread_t left_array[car_left];
    left_go_count=car_left;
    printf("Enter the number on right side of the bridge:");
    scanf("%d",&car_right);
    pthread_t right_array[car_right];
    right_go_count=car_right;
    sem_init(&passing,0,0);
    sem_init(&left_go,0,5);
    sem_init(&right_go,0,0);
    sem_init(&done,0,0);

    sem_init(&binary,0,1);

    sem_post(&passing);
    int rc;
    
    for(int i=0;i<car_left;i++){
        rc=pthread_create(&left_thread,NULL,left,(void*)(long)i+1);
        left_array[i]=left_thread;
    }
    for(int i=0;i<car_right;i++){
        rc=pthread_create(&right_thread,NULL,right,(void*)(long)i+1);
        right_array[i]=right_thread;
    }
    for(int i=0;i<car_left;i++){
        rc=pthread_join(left_array[i],NULL);
    }
    for(int i=0;i<car_right;i++){
        rc=pthread_join(right_array[i],NULL);
    }
    sem_wait(&done);
    return 0;
}