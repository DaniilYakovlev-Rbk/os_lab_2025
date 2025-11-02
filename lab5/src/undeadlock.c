#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t forks[5];

void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left_fork = id;
    int right_fork = (id + 1) % 5;
    
    pthread_mutex_lock(&forks[left_fork]);
    printf("Философ %d взял левую вилку %d\n", id, left_fork);
        
    pthread_mutex_lock(&forks[right_fork]);
    printf("Философ %d взял правую вилку %d - ЕСТ!\n", id, right_fork);
        
    pthread_mutex_unlock(&forks[right_fork]);
    pthread_mutex_unlock(&forks[left_fork]);
    
    printf("Философ %d поел\n", id);
    return NULL;
}

int main() {
    pthread_t philosophers[5];
    int ids[5] = {0, 1, 2, 3, 4};
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    
    printf("Начало работы...\n");
    
    for (int i = 0; i < 5; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    
    printf("Все философы поели успешно!\n");
    return 0;
}