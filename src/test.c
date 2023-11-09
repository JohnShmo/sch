#include <stdio.h>
#include "sch_array.h"
#include "sch_string.h"

int main(void)
{
    string_t str;
    dstrnew(&str, "Hello, world!");
    printf("Length: %zu\n", dstrlen(&str));
    printf("Capacity: %zu\n", dstrcap(&str));
    printf("%s\n", dstrc(&str));

    dstrcat(&str, " Short.");
    printf("Length: %zu\n", dstrlen(&str));
    printf("Capacity: %zu\n", dstrcap(&str));
    printf("%s\n", dstrc(&str));

    dstrcat(&str, " Long long long long long long!");
    printf("Length: %zu\n", dstrlen(&str));
    printf("Capacity: %zu\n", dstrcap(&str));
    printf("%s\n", dstrc(&str));

    dstrfit(&str);
    printf("Length: %zu\n", dstrlen(&str));
    printf("Capacity: %zu\n", dstrcap(&str));
    printf("%s\n", dstrc(&str));

    dstrfree(&str);

    return 0;
}