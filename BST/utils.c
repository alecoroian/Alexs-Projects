/*******************************************************************************
 * Name        : utils.c
 * Author      : Alex Coroian
 * Pledge      : I pledge my Honour that I have abided by the Stevens Honour System
 ******************************************************************************/
#include "utils.h"
#include <stdio.h>

// Comparison of ints
int cmpr_int(void *x, void *y)
{
    int x1 = *((int *)x);
    int y1 = *((int *)y);

    if (x1 > y1)
        return 1;
    else if (x1 < y1)
        return -1;
    else
        return 0;
}

// Comparison of floats
int cmpr_float(void *x, void *y)
{
    float x2 = *((float *)x);
    float y2 = *((float *)y);

    if (x2 > y2)
        return 1;
    else if (x2 < y2)
        return -1;
    else
        return 0;
}

// Print integers
void print_int(void *x)
{
    printf("%d ", *((int *)x));
}

// Print floats
void print_float(void *x)
{
    printf("%.6f ", *((float *)x));
}
