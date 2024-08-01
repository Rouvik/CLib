#include <stdio.h>

#include "clib/clib_vector_base_type.h"

int main()
{
    CLib_Vector_t vector = CLib_Vector_init();
    CLib_Vector_pushBack(&vector, 5);
    CLib_Vector_pushBack(&vector, 4);
    CLib_Vector_pushBack(&vector, 3);
    CLib_Vector_pushBack(&vector, 2);
    CLib_Vector_pushBack(&vector, 1);

    for (int i = 0; i < vector.size; i++)
    {
        printf("Element: %d\n", vector.data[i]);
    }

    printf("DEBUG: Size: %d, Reserved: %d\n",vector.size, vector.reserved);

    CLib_Vector_deinit(&vector);
    return 0;
}