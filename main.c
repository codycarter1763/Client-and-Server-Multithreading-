/**
 * Name: Carter, Cody
 * Email: ccar115@lsu.edu
 * Project: PA-1 (Multithreading)
 * Instructor: Feng Chen
 * Class: cs4103-sp24
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_JOBS 10000

pthread_mutex_t lock;
pthread_attr_t attr;
pthread_t tid;

sem_t vacant_space, filled_space;

typedef struct {
    int id;
    int client_id;
} job;

job buffer[MAX_JOBS];
int in = 0;
int out = 0;

int num_job_created = 0;
int num_job_processed = 0;
int size_job_pool;
int num_job_total;

void* client_thread(void* arg);
void* server_thread(void* arg);
void serviceLog(const char *thread_type, int thread_id, int job_pool_index, int job_id);

int *client_job_total; 
int *server_job_total;


void* client_thread(void* arg) {
    int client_id = *(int*)arg;
    
    while (1) {
        sem_wait(&vacant_space);
        pthread_mutex_lock(&lock);
        
        if (num_job_created >= num_job_total) {
            pthread_mutex_unlock(&lock);
            sem_post(&filled_space); 
            break;
        }
        if (((in + 1) % size_job_pool) != out) {
            buffer[in].id = num_job_created;
            buffer[in].client_id = client_id;
            in = (in + 1) % size_job_pool;
            num_job_created++;
            client_job_total[client_id - 1]++;
            
            serviceLog("Client", client_id, buffer[in - 1].id, num_job_created-1);
            //printf("Client %d created job %d\n", client_id, buffer[in - 1].id);
        }
        pthread_mutex_unlock(&lock);
        sem_post(&filled_space);
    }

    pthread_exit(NULL);
}

void* server_thread(void* arg) {
    int job_processing_time = *(int*)arg;
    int jobs_processed = 0;
    do {
        sem_wait(&filled_space);
        pthread_mutex_lock(&lock);
        if (num_job_processed < num_job_total) {
            int job_id = buffer[out].id;
            if (in != out) 
            {
                int client_id = buffer[out].client_id;
                out = (out + 1) % size_job_pool;
                //printf("Server processed job %d from client %d\n", job_id, client_id);
                num_job_processed++;
                server_job_total[client_id - 1]++;
                serviceLog("Server", client_id, out, job_id);
            }
        }
        pthread_mutex_unlock(&lock);
        sem_post(&vacant_space);
    } while (num_job_processed < num_job_total);

    pthread_exit(NULL);
}

void serviceLogHeader(const char *header) {
    FILE *service_log = fopen("service.log", "w");

    fprintf(service_log, "%s\n", header);

    fclose(service_log);
}

void serviceLog(const char *thread_type, int thread_id, int job_pool_index, int job_id)
{
    struct timespec timestamp;
    clock_gettime(CLOCK_MONOTONIC, &timestamp);
    
    long ns_time = timestamp.tv_sec * 1000000000 + timestamp.tv_nsec;
    
    FILE *service_log = fopen("service.log", "a");
    
    fprintf(service_log, "%ld    %s      %d           %d          %d\n", 
    ns_time, thread_type, thread_id, job_pool_index, job_id);
    
    fclose(service_log);
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Error: Invalid number of arguments\n");
        return EXIT_FAILURE;
    }

    int num_client_threads = atoi(argv[1]);
    int num_server_threads = atoi(argv[2]);
    size_job_pool = atoi(argv[3]);
    num_job_total = atoi(argv[4]);
    int job_processing_time = atoi(argv[5]);
    
    sem_init(&vacant_space, 0, size_job_pool);
    sem_init(&filled_space, 0, 0);
    pthread_attr_init(&attr);
    pthread_mutex_init(&lock, NULL);

    pthread_t client_threads[num_client_threads];
    pthread_t server_threads[num_server_threads];
    
    client_job_total = malloc(sizeof(int) * num_client_threads); 
    server_job_total = malloc(sizeof(int) * num_server_threads);
    
    serviceLogHeader("Timestamp (ns)    Thread  Thread ID  Pool Index   Job ID");
    for (int i = 0; i < num_client_threads; i++) {
        int *client_id = malloc(sizeof(int)); 
        *client_id = i + 1;
        client_job_total[i] = 0;
        pthread_create(&client_threads[i], &attr, client_thread, client_id); // Pass client ID as argument
    }

    for (int i = 0; i < num_server_threads; i++) 
    {
        server_job_total[i] = 0;
        pthread_create(&server_threads[i], &attr, server_thread, &job_processing_time);
    }

    for (int i = 0; i < num_client_threads; i++) 
    {
        pthread_join(client_threads[i], NULL);
    }

    for (int i = 0; i < num_server_threads; i++) 
    {
        pthread_join(server_threads[i], NULL);
    }

    printf("Number of jobs created by each client thread:\n");
    for(int i = 0; i < num_client_threads; i++)
    {
        printf("Client thread #%d created %d jobs total\n", i + 1, client_job_total[i]);
    }
    printf("Number of jobs processed by each server thread:\n");
    for(int i = 0; i < num_server_threads; i++)
    {
        printf("Server thread #%d processed %d jobs total\n", i + 1, server_job_total[i]);
    }
    printf("All client threads have completed.\n");
    
    return EXIT_SUCCESS;
}


