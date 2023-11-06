#include <stdio.h>
#include "sch_array.h"
#include "sch_string.h"

int main(void)
{
    dstr_t str = dstrnew("Hello, world!");
    dstrcat(str, " I'm a string!");
    dstrcat(str, " This is a test for dynamic strings!");
    dstrcat(str, " I'm a string!");

    printf("%s\n", dcstr(str));
    printf("Length: %zu\n", dstrlen(str));
    printf("Capacity: %zu\n", dstrcap(str));

    dstrfree(str);

    return 0;
}