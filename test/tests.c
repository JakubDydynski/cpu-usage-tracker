#include "util.h"
#include "assert.h"
#include <stdio.h>

#define CPU_NUMBER 12
#warning Not really a good test, check cpu number on your machine

static void TEST_count_cpu_number()
{
    int expected_result = CPU_NUMBER;

    int res = count_cpu_number();

    assert(res == expected_result);
    
}

int main()
{

    TEST_count_cpu_number();
    printf("TESTS PASSED\n");
    return 0;
}