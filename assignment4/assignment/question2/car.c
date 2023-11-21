#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>
#include <semaphore.h> 

sem_t new_ride,car_load,boarding_unboarding,car_stop,onboard,unboarded,capacity_sem;
int num_passengers, capacity,onboard_int,unboarded_int;

void load(){
    printf("Loading the car\n");
    sleep(2);
}

void unload(){
    printf("Unloading the car\n");
    sleep(2);
}

void board(){
    printf("Boarding the passenger\n");
    sleep(1);
}
void unboard(){
    printf("Unbording the passenger\n");
    sleep(1);
}
void *passenger(void *args){
    while(1){
        long passenger_num=(long)args;
        // int capacty_now;
        // sem_getvalue(&capacity_sem,&capacty_now);
        // printf("capacty_now:%d\n",capacty_now);
        sem_wait(&capacity_sem);
        sem_wait(&car_load);
        sem_wait(&boarding_unboarding);
        printf("Passenger number:%ld is boarding.\n",passenger_num);
        board();
        onboard_int++;
        printf("Passenger number:%ld has boarded.\n",passenger_num);
        sem_post(&boarding_unboarding);
        sem_post(&car_load);
        if(onboard_int==capacity){
            sem_post(&onboard);
        }
        sem_wait(&car_stop);
        printf("Passenger number:%ld is unboarding.\n",passenger_num);
        unboard();
        unboarded_int++;
        printf("Passenger number:%ld has unboarded.\n",passenger_num);
        sem_post(&car_stop);
        if(unboarded_int==capacity){
            sem_post(&unboarded);
        }
    }
}

void *car(void *args){
    while(1){
        onboard_int=0;
        unboarded_int=0;
        sem_wait(&new_ride);
        printf("New car ride.\n");
        load();
        sem_post(&car_load);
        sem_post(&boarding_unboarding);
        sem_wait(&onboard);
        printf("All the passengers on baord.\n");
        printf("Car starting the ride.\n");
        sleep(2);
        printf("Car stopping the ride.\n");
        unload();
        sem_post(&car_stop);
        sem_wait(&unboarded);
        printf("All the passengers have unboarded.\n");
        sem_wait(&car_stop);
        sem_wait(&car_load);
        sem_wait(&boarding_unboarding);
        for(int i=0;i<capacity;i++){
            sem_post(&capacity_sem);
        }
        sem_post(&new_ride);
    }
}




int main(int argc, char *argv[]){
    printf("Enter the number of passengers:");
    scanf("%d",&num_passengers);
    printf("Enter the capacity of car:");
    scanf("%d",&capacity);
    pthread_t car_thread,passenger_thread;
    sem_init(&new_ride,0,0);
    sem_init(&car_load,0,0);
    sem_init(&boarding_unboarding,0,0);
    sem_init(&car_stop,0,0);
    sem_init(&onboard,0,0);
    sem_init(&unboarded,0,0);
    sem_init(&capacity_sem,0,0);
    int rc;
    if(capacity>num_passengers){
        printf("Capacity more than numbers of passengers.\n");
        return 0;
    }
    int car_th;

    printf("START\n");
    onboard_int=0;
    unboarded_int=0;
    pthread_t passenger_thread_array[num_passengers];
    for(int i=0;i<capacity;i++){
        sem_post(&capacity_sem);
    }
    for(int i=0;i<num_passengers;i++){
        rc=pthread_create(&passenger_thread,NULL,passenger,(void *)(long)i);assert(rc==0);
        passenger_thread_array[i]=passenger_thread;
    }
    car_th=pthread_create(&car_thread,NULL,car,(void *)1);
    sem_post(&new_ride);
    rc=pthread_join(car_thread,NULL);assert(rc==0);
    return 0;
}