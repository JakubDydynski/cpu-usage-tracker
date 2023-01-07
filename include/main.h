#ifndef MAIN_H
#define MAIN_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


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

#endif