#include <stdio.h>
#include "sch_array.h"

typedef struct
{
    size_t size;
    size_t capacity;
    int *data;
} int_array_t;

int main(void)
{

    int_array_t arr;
    darnew(&arr, 10);

    for (int i = 0; i < 10; i++)
    {
        darpush(&arr, i);
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", arr.data[i]);
    }

    darfree(&arr);

    return 0;
}