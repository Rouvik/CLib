#include <stdio.h>

#include "clib/clib_macro.h"

int main()
{
    typedef struct Test
    {
        int x;
        char y;
        int z;
    } Test_t;
    
    Test_t s = {1, 'a', 5};

    printf("%p %p\n", &s, CLIB_CONTAINER_OF(&(s.y), Test_t, y));

    return 0;
}