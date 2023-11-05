#include <stdio.h>
#include "sch_array.h"

int main(void)
{
    // Static array
    int arr[5] = { 0 };
    size_t arr_cap = sizeof(arr) / sizeof(arr[0]);
    size_t arr_len = 0;

    arpush(arr, &arr_len, 1);
    arpush(arr, &arr_len, 2);
    arpush(arr, &arr_len, 3);
    arpush(arr, &arr_len, 4);
    arpush(arr, &arr_len, 5);
    arrem(arr, &arr_len, 2);

    printf("arr len: %zu\n", arr_len);
    printf("arr cap: %zu\n", arr_cap);

    for (size_t i = 0; i < arr_len; i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("\n");

    // Dynamic array
    int *darr = DARNEW;

    darpush(darr, 1);
    darpush(darr, 2);
    darpush(darr, 3);
    darpush(darr, 4);
    darpush(darr, 5);
    darrem(darr, 2);

    printf("darr len: %zu\n", darlen(darr));
    printf("darr cap: %zu\n", darcap(darr));

    for (size_t i = 0; i < darlen(darr); i++)
    {
        printf("%d\n", darr[i]);
    }
    printf("\n");

    darfree(darr);

    return 0;
}