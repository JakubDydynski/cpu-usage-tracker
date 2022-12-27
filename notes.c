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

// 5. Pomyśl o buforowaniu danych, co jeśli wątek czytający z pliku czyta szybciej niż przetwarzający?
// Spróbuj rozwiązać ten problem za pomocą jakieś struktury danych (RingBuffer / Queue),
// try with github copilot, ale ogólnie to 1. robimy notyfikacje, a poza tymi notyifkacjami
// w sytuacji wktórej reader jest szybszy niż analyzer, trzeba zrobić jeszcze buforowanie

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


//  struct timespec ts;
//     ts.tv_sec = 0;
//     ts.tv_nsec = 500000000;

//     printf("Reader\n");
//     for (uint8_t k=0; k<NUM_OF_MEASUR; k++)
//     {
//         FILE *fp;
//         fp = fopen("/proc/stat", "r");
//         if (fp == NULL)
//         {
//             printf("ERROR: couldn't read from /proc/stat");
//             // abort or send error to logger or smth
//         }
//         else
//         {
//             // first ten measurements are cpu which aggregates all cpus
//             fscanf(fp, "%s ", trashc);
//             fscanf(fp, " %*d %*d %*d %*d %*d %*d %*d %*d %*d %d" , trashi); 
            
//             for (uint8_t i=0; i<NUM_OF_CPU; i++)
//             {
//                 fscanf(fp, "%s ", trashc);
//                 for (uint8_t j=0; j<NUM_OF_CPU_FIELDS; j++)
//                 {
//                         fscanf(fp, "%d ", &cpu_usage_data.cpu_data[k][i][j]);
//                         printf("%d ", cpu_usage_data.cpu_data[k][i][j]);
//                 }
//                 printf ("\n");
//             }

//             fclose(fp);
//         }
//         nanosleep(&ts, NULL);
//         //sleep(1);
//         printf("\n\n");
//     }

//             for (uint8_t i=0; i<NUM_OF_CPU; i++)
//         {
//             //process the data and claculate cpu usage in %
//             uint32_t idle_first = IDLE(first, i);
//             uint32_t non_idle_first = NON_IDLE(first, i);
//             uint32_t total_first = idle_first + non_idle_first;


//             uint32_t idle_next = IDLE(next, i);
//             uint32_t non_idle_next = NON_IDLE(next, i);
//             uint32_t total_next = idle_next + non_idle_next;

//             uint32_t totald = total_next - total_first;
//             uint32_t idled = idle_next - idle_first;
//             printf("%d: totald: %d, idled: %d \n ", i, totald, idled);
//             cpu_usage_data.cpu_usage_in_percent[i] = ((totald - idled)*100)/totald;
//         }

//                 printf("Printer\n");
//         for (uint8_t i=0; i<NUM_OF_CPU; i++)
//         {
//                 //print cpu usage in %
//                 printf("Cpu %d: %d%% \n", i, cpu_usage_data.cpu_usage_in_percent[i]);
//         } 

//6. Należy także zaimplementować przechwytywanie sygnału SIGTERM i zadbać o odpowiednie zamknięcie
//aplikacji (zamknięcie pliku, zwolnienie pamięci, zakończenie wątków).
