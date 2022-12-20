#include "main.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
/*-------------------------------------TYPEDEF----------------------------------*/
typedef struct{
	uint32_t cpu_data[NUM_OF_MEASUR][NUM_OF_CPU][NUM_OF_CPU_FIELDS]; // 2 MEASUREMENTS 7 TABLES EACH 10 ELEMENTS

}cpu_usage;
/*-------------------------------------DEFINES----------------------------------*/

#define NON_IDLE(meas,i) cpu_usage_data.cpu_data[meas][i][user] + cpu_usage_data.cpu_data[meas][i][nice] + \
					cpu_usage_data.cpu_data[meas][i][system] + cpu_usage_data.cpu_data[meas][i][irq] + \
					cpu_usage_data.cpu_data[meas][i][softirq] + cpu_usage_data.cpu_data[meas][i][steal]

#define IDLE(meas,i) cpu_usage_data.cpu_data[meas][i][idle] + cpu_usage_data.cpu_data[meas][i][iowait]
#define NUM_OF_CPU 12
#define NUM_OF_CPU_FIELDS 10
#define NUM_OF_MEASUR 2

#warning "Before use check whether define NUM_OF_CPU correspond to number of cpus of the machine
/*-------------------------------------GLOBALS----------------------------------*/
char trashc[10];
int trashi[10];

cpu_usage cpu_usage_data;
uint8_t cpu_usage_in_percent[NUM_OF_CPU];
/*-------------------------------------SIGTERM----------------------------------*/
volatile sig_atomic_t done = 0;
 
void term(int signum)
{
		done = 1;
}

void* Reader_Thread(void* args)
{

	
	for (uint8_t k=0; k<NUM_OF_MEASUR; k++)
	{
		FILE *fp;
		fp = fopen("/proc/stat", "r");
		if (fp == NULL)
		{
			printf("ERROR: couldn't read from /proc/stat");
			// abort or send error to logger or smth
		}
		else
		{
			fscanf(fp, "%s ", trashc);
         	fscanf(fp, " %*d %*d %*d %*d %*d %*d %*d %*d %*d %d" , trashi);
			
			for (uint8_t i=0; i<NUM_OF_CPU; i++)
			{
				fscanf(fp, "%s ", trash); // first ten measurements are cpu which aggregates all cpus
				for (uint8_t j=0; j<NUM_OF_CPU_FIELDS; j++)
				{
						fscanf(fp, "%d ", &cpu_usage_data.cpu_data[k][i][j]);
				}
			}

			fclose(fp);
		}
		sleep(0.2);
	}
	while(1)
	{
			// printf("I am threadFunction.\n");
	}
}

void* Analyzer_Thread(void* args)
{

	for (uint8_t i=0; i<NUM_OF_CPU; i++)
	{
		//process the data and claculate cpu usage in %
		uint32_t idle_first = IDLE(first, i);
		uint32_t non_idle_first = NON_IDLE(first, i);
		uint32_t total_first = idle_first + non_idle_first;


		uint32_t idle_next = IDLE(next, i);
		uint32_t non_idle_next = NON_IDLE(next, i);
		uint32_t total_next = idle_next + non_idle_next;

		uint32_t totald = total_next - total_first;
		uint32_t idled = idle_next - idle_first;

		cpu_usage_in_percent[i] = (totald - idled)/totald;
	} 
	while(1)
	{
			// printf("I am threadFunction.\n");
	}
}

void* Printer_Thread(void* args)
{
	printf("Printing cpus usage: ")
	for (uint8_t i=0; i<NUM_OF_CPU; i++)
	{
		//print cpu usage in %
		prinf("Cpu %d: %d%% \n", i, cpu_usage_in_percent[i])
	} 
	while(1)
	{
			// printf("I am threadFunction.\n");
	}
}

int main()
{
		/*creating thread id*/
		pthread_t id_reader;
		pthread_t id_analyzer;
		pthread_t id_printer;
		int ret;
	
		ret = pthread_create(&id_reader, NULL, &Reader_Thread, NULL);
		if(ret==0){
				printf("Thread created successfully.\n");
		}
		else{
				printf("Thread not created.\n");
				return 0; /*return from main*/
		}

		ret = pthread_create(&id_analyzer, NULL, &Analyzer_Thread, NULL);
		if(ret==0){
				printf("Thread created successfully.\n");
		}
		else{
				printf("Thread not created.\n");
				return 0; /*return from main*/
		}

		ret = pthread_create(&id_printer, NULL, &Analyzer_Thread, NULL);
		if(ret==0){
				printf("Thread created successfully.\n");
		}
		else{
				printf("Thread not created.\n");
				return 0; /*return from main*/
		}




		struct sigaction action;
		memset(&action, 0, sizeof(struct sigaction));
		action.sa_handler = term;
		sigaction(SIGTERM, &action, NULL);
 
		int loop = 0;

		while (!done)
		{
				int t = sleep(3);
				/* sleep returns the number of seconds left if
				 * interrupted */
				while (t > 0)
				{
						printf("Loop run was interrupted with %d "
									 "sec to go, finishing...\n", t);
						t = sleep(t);
				}
				printf("Finished loop run %d.\n", loop++);
		}
 
		printf("done.\n");
	return 0;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ fork process @@@@@@@@@@@@@@@@@@@@@@@@@

// void *Reader_Thread(void *args)
// {
//   pid_t pid;
//   int status;
//   pid_t ret;
//  // char *const args[3] = {"any_exe", input, NULL};
//   char **env;
//   extern char **environ;




//   pid = fork();
//   if (pid == -1)
//   {
//     printf("fork failed!\n");
//     exit(pid);
//   }
//   else if (pid !=0)
//   {
//     while ((ret = waitpid(pid, &status,0)) == -1)
//     {
//       if (errno!=EINTR)
//       {
//       printf("error");
//       break;
//       }
//     }
//     if ((ret == 0) ||
//         !(WIFEXITED(status) && !WEXITSTATUS(status))) {
//       /* Report unexpected child status */
//     }
//   }
//   else {
//     /* ... Initialize env as a sanitized copy of environ ... */
//     if (execve("/usr/bin/any_cmd", args, env) == -1) {
//       /* Handle error */
//       _Exit(127);
//     }
//   }
//   while(1)
//   {
//     printf("thread\n");
//   }
// }