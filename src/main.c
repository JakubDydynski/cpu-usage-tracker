#include "main.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
/*-------------------------------------SIGTERM----------------------------------*/
volatile sig_atomic_t done = 0;
 
void term(int signum)
{
    done = 1;
}


char trash[1000];
#define NUM_OF_CPU 7
#define NUM_OF_CPU_FIELDS 10
#define NUM_OF_MEASUR 2

typedef struct{
  int cpu_data[NUM_OF_MEASUR][NUM_OF_CPU][NUM_OF_CPU_FIELDS]; // 2 MEASUREMENTS 7 TABLES EACH 10 ELEMENTS

}cpu_usage;
cpu_usage cpu_usage_data;

void* Reader_Thread(void* args)
{

  FILE *fp;

  fp = fopen("/proc/stat", "r");
  if (fp == NULL)
  {
    printf("some error");
  }
  else
  {
    for (uint8_t i=0; i<NUM_OF_CPU; i++)
    {
      fscanf(fp, "%s ", trash);
      for (uint8_t j=0; j<NUM_OF_CPU_FIELDS; j++)
      {
         fscanf(fp, "%d ", &cpu_usage_data.cpu_data[0][i][j]);
      }
    }

    fclose(fp);
  }


  // todo: sleep with nanosecond // or use thread which has thrd_sleep - read upon that ik reading again, but with purpose of finally writing something
  // modulate - make more functions
  // and that's it - first thread done
    while(1)
    {
        printf("I am threadFunction.\n");
    }
}



int main()
{
    /*creating thread id*/
    pthread_t id;
    int ret;
  
    /*creating thread*/
    ret=pthread_create(&id,NULL,&Reader_Thread,NULL);
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

/*

cpu  5175288 0 3606971 72209636 0 212319 0 0 0 0
cpu0 748796 0 831396 8543793 0 148795 0 0 0 0


      user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice

  cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0


previous trzeba zroibć w ten sposób że dla przykałdu liczysz, czekasz 500ms liczysz i liczysz z tego

PrevIdle = previdle + previowait

Idle = idle + iowait


PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal

NonIdle = user + nice + system + irq + softirq + steal


PrevTotal = PrevIdle + PrevNonIdle

Total = Idle + NonIdle


# differentiate: actual value minus the previous one

totald = Total - PrevTotal

idled = Idle - PrevIdle


CPU_Percentage = (totald - idled)/totald


*/

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