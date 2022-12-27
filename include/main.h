#include <pthread.h>
enum
{
    user,
    nice_stat,
    system,
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
