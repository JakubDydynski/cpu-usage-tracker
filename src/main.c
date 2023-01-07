#include "main.h"
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

#define NON_IDLE(meas,i) cpu_usage_data.cpu_data[meas][i][user] + cpu_usage_data.cpu_data[meas][i][nice_stat] + \
					cpu_usage_data.cpu_data[meas][i][system_stat] + cpu_usage_data.cpu_data[meas][i][irq] + \
					cpu_usage_data.cpu_data[meas][i][softirq] + cpu_usage_data.cpu_data[meas][i][steal]

#define IDLE(meas,i) cpu_usage_data.cpu_data[meas][i][idle] + cpu_usage_data.cpu_data[meas][i][iowait]
#define NUM_OF_CPU 12 
#define NUM_OF_CPU_FIELDS 10
#define NUM_OF_MEASUR 2

#warning "Before use check whether define NUM_OF_CPU correspond to number of cpus of the machine"
#warning "Create function to count number of cpus"
/*-------------------------------------TYPEDEF----------------------------------*/
typedef struct{
	uint32_t cpu_data[NUM_OF_MEASUR][NUM_OF_CPU][NUM_OF_CPU_FIELDS]; // 2 MEASUREMENTS 7 TABLES EACH 10 ELEMENTS
	uint32_t cpu_usage_in_percent[NUM_OF_CPU];
}cpu_usage_t;

/*-------------------------------------GLOBALS----------------------------------*/
char trashc[10];
int trashi[10];

cpu_usage_t cpu_usage_data;

pthread_mutex_t meas_lock;
pthread_cond_t meas_condition;

uint8_t meas_done=0;
uint8_t calc_done=1;

/*-------------------------------------SIGTERM----------------------------------*/
volatile sig_atomic_t done = 0;
 
void term(int signum)
{
		done = 1;
}

/*-------------------------------------THREADS----------------------------------*/
void* Reader_Thread(void* args)
{   
    struct timespec ts;
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
            FILE *fp;
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
                
                for (uint8_t i=0; i<NUM_OF_CPU; i++)
                {
                    fscanf(fp, "%s ", trashc);
                    for (uint8_t j=0; j<NUM_OF_CPU_FIELDS; j++)
                    {
                            fscanf(fp, "%d ", &cpu_usage_data.cpu_data[k][i][j]);
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
        for (uint8_t i=0; i<NUM_OF_CPU; i++)
        {
            uint32_t idle_first = IDLE(first, i);
            uint32_t non_idle_first = NON_IDLE(first, i);
            uint32_t total_first = idle_first + non_idle_first;

            uint32_t idle_next = IDLE(next, i);
            uint32_t non_idle_next = NON_IDLE(next, i);
            uint32_t total_next = idle_next + non_idle_next;

            uint32_t totald = total_next - total_first;
            uint32_t idled = idle_next - idle_first;

            cpu_usage_data.cpu_usage_in_percent[i] = ((totald - idled)*100)/totald;
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
    ts.tv_sec = 1;
    ts.tv_nsec = 0;

	while(1)
	{ 
        pthread_mutex_lock(&meas_lock);
        printf("Printer IN\n");
        for (uint8_t i=0; i<NUM_OF_CPU; i++)
        {
            printf("Cpu %d: %d%% \n", i, cpu_usage_data.cpu_usage_in_percent[i]);
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
    {}

    void* res=NULL;

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

    printf("done.\n");
    return 0;
}