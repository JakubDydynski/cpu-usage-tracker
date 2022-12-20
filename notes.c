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