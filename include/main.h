#ifndef MAIN_H
#define MAIN_H

enum
{
    user,
    nice_stat,
    system_stat,
    idle,
    iowait,
    irq,
    softirq,
    steal,
    guest,
    guest_nice
};

enum
{
    first,
    next
};

enum
{
    sigterm_exit=1,
    watchdog_exit
};

#endif
