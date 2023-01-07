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


// 5. Pomyśl o buforowaniu danych, co jeśli wątek czytający z pliku czyta szybciej niż przetwarzający?
// Spróbuj rozwiązać ten problem za pomocą jakieś struktury danych (RingBuffer / Queue),
// try with github copilot, ale ogólnie to 1. robimy notyfikacje, a poza tymi notyifkacjami
// w sytuacji wktórej reader jest szybszy niż analyzer, trzeba zrobić jeszcze buforowanie
// chyba albo notyfikacje albo buforowanie

//6. Należy także zaimplementować przechwytywanie sygnału SIGTERM i zadbać o odpowiednie zamknięcie
//aplikacji (zamknięcie pliku, zwolnienie pamięci, zakończenie wątków).

// Aplikacja musi posiadać przynajmniej 1 automatyczny test. Może być to test jednostkowy napisany w
// C (budowany za pomocą make test) lub inny test napisany w C lub w języku skryptowym odpalającym
// cały program i testujący jakieś zachowanie aplikacji
// . Do pisania testów jednostkowych, nie potrzebujesz żadnej dodatkowej biblioteki. Proste testy możesz
// napisać z użyciem wbudowanej funkcji assert()

//Aplikacja musi zostać przetestowana pod kątem wycieków pamięci, do tego należy użyć programu valgrind,