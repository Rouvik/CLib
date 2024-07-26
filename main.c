#include <stdio.h>

#include "clib/clib_string.h"

int main()
{
    CLib_String str1 = CLib_String_init("Hello ");

    CLib_String_concatCStrN(&str1, "World and Bye", 5);

    printf("Output: \"%s\"\n", str1.str);

    CLib_String_deinit(&str1);

    return 0;
}