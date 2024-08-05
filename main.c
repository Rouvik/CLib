#include <stdio.h>

#include "clib/clib_macro.h"

int main()
{
    CLIB_FERR(DEFAULT, "Some message")

    return 0;
}