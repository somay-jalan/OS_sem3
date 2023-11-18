#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>
#include <semaphore.h> 

sem_t load_unload,car_go,boarding_unboarding,car_stop,car_loaded,car_unloaded; 
int car_load_int,car_unload_int;
int num_passengers, capacity;

void load(){
    printf("Loading the car\n");
    sleep(2);
}

void unload(){
    printf("Unloading the car\n");
    sleep(2);
}

void board(){
    printf("Boarding the car\n");
    sleep(1);
}
void unboard(){
    printf("Unbording the car\n");
    sleep(1);
}
void *passenger(void *args){
    long passenger_num=(long)args;
    sem_wait(&boarding_unboarding);
    printf("Passenger number :%ld is boarding\n",passenger_num);
    board();
    car_load_int++;
    printf("Passenger number :%ld has boarded\n",passenger_num);
    sem_post(&boarding_unboarding);
    if(car_load_int==capacity){
        sem_post(&car_loaded);
    }
    sem_wait(&car_stop);
    sem_wait(&load_unload);
    sem_wait(&boarding_unboarding);
    printf("Passenger number :%ld is unboarding\n",passenger_num);
    unboard();
    car_unload_int++;
    printf("Passenger number :%ld has unboarded\n",passenger_num);
    sem_post(&boarding_unboarding);
    sem_post(&car_stop);
    sem_post(&load_unload);
    if(car_unload_int==capacity){
        sem_post(&car_unloaded);
    }
}

void *car(void *args){
    printf("Car thread starting.\n");
    printf("Car loading\n");
    load();
    printf("Car loaded\n");
    sem_post(&load_unload);
    sem_wait(&car_go);
    printf("Car going\n");
    sleep(2);
    printf("Car completed its journey\n");
    // sem_post(&car_go);
    sem_post(&car_stop);
    printf("Car unloading\n");
    unload();
    printf("Car unloaded\n");
    sem_post(&load_unload);
    sem_wait(&car_unloaded);
    sem_wait(&car_stop);
}




int main(int argc, char *argv[]){
    pthread_t car_thread,passenger_thread;
    sem_init(&load_unload,0,0);
    sem_init(&car_go,0,0);
    sem_init(&car_stop,0,0);
    sem_init(&boarding_unboarding,0,1);
    sem_init(&car_unloaded,0,0);
    sem_init(&car_loaded,0,0);

    printf("Enter the number of passengers:");
    scanf("%d",&num_passengers);
    printf("Enter the capacity of car:");
    scanf("%d",&capacity);
    if(capacity>num_passengers){
        printf("Capacity more than numbers of passengers.\n");
        return 0;
    }
    int cur_count=0;
    while(cur_count<num_passengers){
        car_load_int=0;
        car_unload_int=0;
        if(num_passengers-cur_count<capacity){
            printf("Not enough passengers left to fill the car.\n");
            printf("All the journeys has been done\n");
            return 0;
        }
        int rc;
        rc=pthread_create(&car_thread,NULL,car,NULL);assert(rc==0);
        sem_wait(&load_unload);
        printf("Boarding passengers\n");
        for(int i=0;i<capacity;i++){
            rc=pthread_create(&passenger_thread,NULL,passenger,(void*)(long)(cur_count+1));assert(rc==0);
            cur_count++;
        }
        sem_wait(&car_loaded);
        printf("Boarding complete\n");
        sem_post(&car_go);
        rc=pthread_join(car_thread,NULL);assert(rc==0);
    }
    printf("All the journeys has been done\n");
    return 0;
}