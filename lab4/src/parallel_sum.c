#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/time.h>
#include "sum.h"  

void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (unsigned int i = 0; i < array_size; i++) {
    array[i] = rand();
  }
}

int main(int argc, char **argv) {
  uint32_t threads_num = -1;
  uint32_t array_size = -1;
  uint32_t seed = -1;

  while (true) {
    static struct option options[] = {
        {"threads_num", required_argument, 0, 0},
        {"seed", required_argument, 0, 0},
        {"array_size", required_argument, 0, 0},
        {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1) break;

    switch (option_index) {
      case 0:
        threads_num = atoi(optarg);
        if (threads_num <= 0) {
          printf("threads_num must be positive\n");
          return 1;
        }
        break;
      case 1:
        seed = atoi(optarg);
        if (seed <= 0) {
          printf("seed must be positive\n");
          return 1;
        }
        break;
      case 2:
        array_size = atoi(optarg);
        if (array_size <= 0) {
          printf("array_size must be positive\n");
          return 1;
        }
        break;
      default:
        printf("Unknown option\n");
        return 1;
    }
  }

  if (threads_num == -1 || seed == -1 || array_size == -1) {
    printf("Usage: ./parallel_sum  --threads_num num --seed num --array_size num\n");
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed); 

  pthread_t threads[threads_num];

  int block_size = array_size / threads_num;
  struct SumArgs args[threads_num];

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = i * block_size;
    if (i == threads_num - 1) {
      args[i].end = array_size;
    }
    else {
     args[i].end = (i + 1) * block_size;
    }

    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
        printf("Error: pthread_create failed!\n");
        return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %.3f ms\n", elapsed_time);

  free(array);
  return 0;
}