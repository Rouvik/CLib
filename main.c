#include <stdio.h>
#include <limits.h>

#include "clib/clib_list.h"

CLib_List_setup(int, INT_MIN)

int main()
{
    CLib_List_int_Root list = CLib_List_int_init(1);

    CLib_List_int_pushBack(&list, 2);
    CLib_List_int_pushBack(&list, 3);
    CLib_List_int_pushBack(&list, 4);
    CLib_List_int_pushBack(&list, 5);

    CLib_List_int_insert(&list, 9, 2);

    CLib_List_int_t *node = list.root;
    for (int i = 0; i < list.size; i++)
    {
        printf("%d\n", node->data);
        node = node->next;
    }
    
    CLib_List_int_deinit(&list);
    return 0;
}