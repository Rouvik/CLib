#include <stdio.h>

#include "clib/clib_vector.h"

CLIB_VECTOR_SETUP(int, 0)

int main()
{
    CLib_Vector_int_t vector = CLib_Vector_int_init();
    CLib_Vector_int_pushBack(&vector, 6);
    CLib_Vector_int_pushBack(&vector, 5);
    CLib_Vector_int_pushBack(&vector, 4);
    CLib_Vector_int_pushBack(&vector, 3);
    CLib_Vector_int_pushBack(&vector, 2);
    CLib_Vector_int_pushBack(&vector, 1);

    CLib_Vector_int_fill(&vector, 69, 10, 5);

    int data = CLib_Vector_int_popFront(&vector);

    printf("Pop Front: %d\n", data);

    for (int i = 0; i < vector.size; i++)
    {
        printf("Element: %d\n", vector.data[i]);
    }

    printf("DEBUG: Size: %d, Reserved: %d\n",vector.size, vector.reserved);

    CLib_Vector_int_deinit(&vector);
    return 0;
}