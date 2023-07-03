#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int count_cpu_number(void)
{
    FILE* fp;
    int ret=0;
    char* trash = (char*) malloc(1500 * sizeof(char));
    if (!trash) 
    {
        perror("malloc");
        return -1;
    }
    fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        printf("ERROR: couldn't read from /proc/stat");
        return -1;
    }
    else
    {
        while (fscanf(fp, "%[^\n] ", trash) != EOF) 
        {
            if(strncmp("cpu", trash, 3) == 0)
            ret++;
        }
        ret--; // first cpu is total stats of all cpus
    }
    //free(trash);
    //fclose(fp);
    return ret;
}
