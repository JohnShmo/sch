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
    darnew(&arr, 1);

    for (int i = 1; i <= 10; i++)
    {
        darpush(&arr, i);
    }

    for (size_t i = 0; i < darsiz(&arr); i++)
    {
        printf("%d\n", dardat(&arr)[i]);
    }

    darfree(&arr);

    return 0;
}