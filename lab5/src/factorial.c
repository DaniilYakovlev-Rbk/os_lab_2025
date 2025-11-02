#include <stdio.h>      
#include <stdlib.h>     
#include <pthread.h>    
#include <string.h>     

long long global_result = 1;  
pthread_mutex_t mutex;      

struct ThreadData {
    int start;   
    int end;     
    int mod;    
};

void* calculate_part(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;

    long long local_result = 1;

    for (int i = data->start; i <= data->end; i++) {
        local_result = (local_result * i) % data->mod;
    }

    pthread_mutex_lock(&mutex);
    global_result = (global_result * local_result) % data->mod;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    int k = 0;      
    int pnum = 0;    
    int mod = 0;     

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            k = atoi(argv[i + 1]);
        } else if (strncmp(argv[i], "--pnum=", 7) == 0) {
            pnum = atoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--mod=", 6) == 0) {
            mod = atoi(argv[i] + 6);
        }
    }

    if (k <= 0 || pnum <= 0 || mod <= 0) {
        printf("Использование: %s -k <число> --pnum=<потоки> --mod=<модуль>\n", argv[0]);
        return 1;
    }

    pthread_t threads[pnum];
    struct ThreadData threadData[pnum];
    pthread_mutex_init(&mutex, NULL);

    int amountperthread = k / pnum;    
    int remains = k % pnum;  

    int start = 1;

    for (int i = 0; i < pnum; i++) {
        int end = start + amountperthread - 1;
        if (remains > 0) {
            end++;
            remains--;
        }

        if (end > k) end = k;

        threadData[i].start = start;
        threadData[i].end = end;
        threadData[i].mod = mod;

        pthread_create(&threads[i], NULL, calculate_part, &threadData[i]);

        start = end + 1;
    }

    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("%d! по модулю %d равен %lld\n", k, mod, global_result);

    return 0;
}