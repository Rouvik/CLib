#include <stdio.h>
#include <limits.h>

#include "clib/clib_list.h"

CLib_List_setup(int, INT_MIN)
    CLib_List_setup(float, __FLT_MIN__)

#define printList_int(list, nodeVar)             \
    CLib_List_int_t *nodeVar = list.root;        \
    for (int i = 0; i < list.size; i++)          \
    {                                            \
        printf("Elements: %d\n", nodeVar->data); \
        nodeVar = nodeVar->next;                 \
    }

#define printList_float(list, nodeVar)             \
    CLib_List_float_t *nodeVar = list.root;        \
    for (int i = 0; i < list.size; i++)            \
    {                                              \
        printf("Elements: %.1f\n", nodeVar->data); \
        nodeVar = nodeVar->next;                   \
    }

// main code ...

int main()
{
    CLib_List_int_Root list = CLib_List_int_init(5);
    CLib_List_int_pushBack(&list, 4);
    CLib_List_int_pushBack(&list, 2);
    CLib_List_int_pushFront(&list, 1);

    // list: 1 5 4 2

    printList_int(list, n1);

    int data = CLib_List_int_popBack(&list);
    printf("Pop Back: %d\n", data);

    printList_int(list, n2);

    CLib_List_int_deinit(&list);

    puts("FLoat Part:\n");

    CLib_List_float_Root list2 = CLib_List_float_init(5.2f);
    CLib_List_float_pushBack(&list2, 4.3f);
    CLib_List_float_pushBack(&list2, 2.9f);
    CLib_List_float_pushFront(&list2, 1.0f);

    // list2: 1.0 5.2 4.3 2.9

    printList_float(list2, n3);

    float data2 = CLib_List_float_popBack(&list2);
    printf("Pop Back: %.1f\n", data2);

    printList_float(list2, n4);

    CLib_List_float_deinit(&list2);
    return 0;
}