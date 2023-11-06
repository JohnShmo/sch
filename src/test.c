#include <stdio.h>
#include "sch_array.h"

int main(void)
{
    // Static array
    int arr[5] = { 0 };
    size_t arr_cap = sizeof(arr) / sizeof(arr[0]);
    size_t arr_len = 0;

    arpush(arr, arr_len++, arr_cap, (int) { 1 });
    arpush(arr, arr_len++, arr_cap, (int) { 2 });
    arpush(arr, arr_len++, arr_cap, (int) { 3 });
    arpush(arr, arr_len++, arr_cap, (int) { 4 });
    arpush(arr, arr_len++, arr_cap, (int) { 5 });

    printf("arr len: %zu\n", arr_len);
    printf("arr cap: %zu\n", arr_cap);

    for (size_t i = 0; i < arr_len; i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("\n");

    // Dynamic array
    int *darr = DARNEW;

    darpush(darr, (int) { 1 });
    darpush(darr, (int) { 2 });
    darpush(darr, (int) { 3 });
    darpush(darr, (int) { 4 });
    darpush(darr, (int) { 5 });

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