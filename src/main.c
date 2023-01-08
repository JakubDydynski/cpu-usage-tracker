#include "main.h"
#include "util.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*-------------------------------------DEFINES----------------------------------*/
 #define handle_error_en(en, msg) do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define NON_IDLE(meas, i, cpu_num) cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * user)] + cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * nice_stat)] + \
					cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * system_stat)] + cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * irq)] + \
					cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * softirq)] + cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * steal)]

#define IDLE(meas, i, cpu_num) cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * idle)] + cpu_data_ptr[meas + cpu_num * (i + NUM_OF_CPU_FIELDS * iowait)]
#define NUM_OF_CPU_FIELDS 10
#define NUM_OF_MEASUR 2

// typedef struct{
// 	uint32_t cpu_data[NUM_OF_MEASUR][NUM_OF_CPU][NUM_OF_CPU_FIELDS];
// 	uint32_t cpu_usage_in_percent[NUM_OF_CPU];
// }cpu_usage_t;

/*-------------------------------------GLOBALS----------------------------------*/
static char trashc[10];
static int trashi[10];

static pthread_mutex_t meas_lock;
static pthread_cond_t meas_condition;

static uint8_t meas_done=0;
static uint8_t calc_done=1;
static FILE *fp;

static int Num_of_cpu=0;
static int* cpu_data_ptr;
static int* cpu_usage_in_percent;

/*-------------------------------------SIGTERM----------------------------------*/
static volatile sig_atomic_t done = 0;
 
static void term(int signum)
{
    if(signum == SIGTERM)
	    done = 1;
}

/*-------------------------------------THREADS----------------------------------*/
void* Reader_Thread(void* args);
void* Analyzer_Thread(void* args);
void* Printer_Thread(void* args);

void* Reader_Thread(void* args)
{   
    struct timespec ts;
    (void)args;
    ts.tv_sec = 0;
    ts.tv_nsec = 300000000;

	while(1)
	{
        pthread_mutex_lock(&meas_lock);
        while(!calc_done)
        {
            pthread_cond_wait(&meas_condition, &meas_lock);
        }
        calc_done=0;

        printf("Reader IN\n");
        for (uint8_t k=0; k<NUM_OF_MEASUR; k++)
        {
            fp = fopen("/proc/stat", "r");
            if (fp == NULL)
            {
                printf("ERROR: couldn't read from /proc/stat");
                // abort and send error to logger or smth
                pthread_exit(NULL);
            }
            else
            {
                // first ten measurements are cpu which aggregates all cpus
                fscanf(fp, "%s ", trashc);
                fscanf(fp, " %*d %*d %*d %*d %*d %*d %*d %*d %*d %d" , trashi); 
                
                for (uint8_t i=0; i<Num_of_cpu; i++)
                {
                    fscanf(fp, "%s ", trashc);
                    for (uint8_t j=0; j<NUM_OF_CPU_FIELDS; j++)
                    {
                            fscanf(fp, "%d ", &cpu_data_ptr[k + Num_of_cpu*(i + NUM_OF_CPU_FIELDS * j)]);
                    }
                }
                fclose(fp);
            }
            nanosleep(&ts, NULL);
        }
        meas_done=1;
        printf("Reader OUT\n");
        pthread_cond_signal(&meas_condition);
        pthread_mutex_unlock(&meas_lock);
	}
}

void* Analyzer_Thread(void* args)
{
    struct timespec ts;
    (void)args;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000;
	while(1)
	{
        pthread_mutex_lock(&meas_lock);
        while(!meas_done)
        {
            pthread_cond_wait(&meas_condition, &meas_lock);
        }
        meas_done=0;
        printf("Analyzer IN\n");
        for (uint8_t i=0; i<Num_of_cpu; i++)
        {
            int idle_first = IDLE(first, i, Num_of_cpu);
            int non_idle_first = NON_IDLE(first, i, Num_of_cpu);
            int total_first = idle_first + non_idle_first;

            int idle_next = IDLE(next, i, Num_of_cpu);
            int non_idle_next = NON_IDLE(next, i, Num_of_cpu);
            int total_next = idle_next + non_idle_next;

            int totald = total_next - total_first;
            int idled = idle_next - idle_first;

            cpu_usage_in_percent[i] = ((totald - idled)*100)/totald;
        }
        calc_done=1;
        printf("Analyzer OUT\n");
        pthread_cond_signal(&meas_condition);
        pthread_mutex_unlock(&meas_lock);
        nanosleep(&ts, NULL);
	}
}

void* Printer_Thread(void* args)
{
    struct timespec ts;
    (void)args;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;

	while(1)
	{ 
        pthread_mutex_lock(&meas_lock);
        printf("Printer IN\n");
        for (uint8_t i=0; i<Num_of_cpu; i++)
        {
            printf("Cpu %d: %d%% \n", i, cpu_usage_in_percent[i]);
        } 
        printf("Printer OUT\n");
        pthread_mutex_unlock(&meas_lock);
        nanosleep(&ts, NULL);
	}
}

int main()
{
    pthread_t id_reader;
    pthread_t id_analyzer;
    pthread_t id_printer;
    int ret;
    struct sigaction action;

    Num_of_cpu = count_cpu_number();
    if (Num_of_cpu < 0)
    {
        perror("Couldn't compute num of cpu");
        return EXIT_FAILURE;
    }

    cpu_data_ptr = (int*)malloc(NUM_OF_CPU_FIELDS * NUM_OF_MEASUR * (unsigned)Num_of_cpu * sizeof(int));
    if (!cpu_data_ptr) 
    {
        perror("malloc");
        return EXIT_FAILURE;
    }
    cpu_usage_in_percent = (int*)malloc((unsigned)Num_of_cpu * sizeof(int));
    if (!cpu_usage_in_percent) 
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    pthread_mutex_init(&meas_lock, NULL);
    pthread_cond_init(&meas_condition, NULL);

    ret = pthread_create(&id_reader, NULL, &Reader_Thread, NULL);
    if(ret==0)
    {
            printf("Thread created successfully.\n");
    }
    else
    {
            printf("Thread not created.\n");
            return 0; /*return from main*/
    }

    ret = pthread_create(&id_analyzer, NULL, &Analyzer_Thread, NULL);
    if(ret==0)
    {
            printf("Thread created successfully.\n");
    }
    else
    {
            printf("Thread not created.\n");
            return 0; /*return from main*/
    }

    ret = pthread_create(&id_printer, NULL, &Printer_Thread, NULL);
    if(ret==0)
    {
            printf("Thread created successfully.\n");
    }
    else
    {
            printf("Thread not created.\n");
            return 0; /*return from main*/
    }

    while (!done)
    {sleep(1);}

    ret = pthread_cancel(id_printer);
    if(ret != 0)
        handle_error_en(ret, "pthread_cancel");
    ret = pthread_join(id_printer, NULL);
    if(ret != 0)
        handle_error_en(ret, "pthread_join");

    ret = pthread_cancel(id_analyzer);
    if(ret != 0)
        handle_error_en(ret, "pthread_cancel");
    ret = pthread_join(id_analyzer, NULL);
    if(ret != 0)
        handle_error_en(ret, "pthread_join");

    ret = pthread_cancel(id_reader);
    if(ret != 0)
        handle_error_en(ret, "pthread_cancel");
    ret = pthread_join(id_reader, NULL);
    if(ret != 0)
        handle_error_en(ret, "pthread_join");

    pthread_mutex_destroy(&meas_lock);

    if (fp==NULL)
        fclose(fp);
    free(cpu_data_ptr);
    free(cpu_usage_in_percent);

    printf("done.\n");
    return 0;
}
