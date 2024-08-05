/**
 * @file clib_list_base_type.h
 * @author Rouvik Maji (majirouvik@gmail.com)
 * @brief The dev base type code for templated clib/clib_list.h, any updates must be made here first and then converted to macro form for templating
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024 GNU GPL <https://www.gnu.org/licenses/>
 */

#include <stdlib.h>

#ifndef CLib_List_TYPE
#define CLib_List_TYPE int
#endif // CLib_List_TYPE

#ifndef CLib_List_ERROR_VALUE
#define CLib_List_ERROR_VALUE 0
#endif // CLib_List_ERROR_VALUE

/**
 * @brief The list structure used to store the data of type TYPE as described in CLIB_LIST_SETUP(TYPE, ERROR_VALUE) macro and a pointer to next element
 * @warning Dont edit the next pointer at all if you don't want to shoot yourself at foot
 */
typedef struct CLib_List
{
    CLib_List_TYPE data;
    struct CLib_List *next;
} CLib_List_t;

/**
 * @brief The root struct containing the list data like size and pointer to its first node
 */
typedef struct
{
    int size;

    CLib_List_t *root;
} CLib_List_Root;

/**
 * @brief Initialises a list with a root element containing the data and returns the CLib_List_Root
 * 
 * @param data The data to store in the root node of the list
 * @return CLib_List_Root The list root container returned containing valid list
 */
CLib_List_Root CLib_List_init(CLib_List_TYPE data)
{
    CLib_List_Root list;
    list.size = 1;
    list.root = (CLib_List_t *)malloc(sizeof(CLib_List_t));
    list.root->data = data;
    list.root->next = NULL;

    return list;
}

/**
 * @brief Initialises an empty CLib_List with only CLib_List_Root of size 0 and root = NULL
 * 
 * @return CLib_List_Root The list with empty contents
 */
CLib_List_Root CLib_List_initEmpty()
{
    return (CLib_List_Root){.size = 0, .root = NULL};
}

/**
 * @brief Frees the CLib_List and sets CLib_List_Root passed to a 0 state ie, {.size = 0, .root = NULL}
 * 
 * @param root The pointer to the CLib_List_Root entity
 */
void CLib_List_deinit(CLib_List_Root *root)
{
    CLib_List_t *node = root->root;
    while (node->next != NULL)
    {
        CLib_List_t *present = node;
        node = node->next;

        free(present);
    }

    free(node);

    root->root = NULL;
    root->size = 0;
}

/**
 * @brief Pushes an element to the end of CLib_List
 * 
 * @param root The pointer to CLib_List_Root to push to
 * @param data The data to push at end of CLib_List
 */
void CLib_List_pushBack(CLib_List_Root *root, CLib_List_TYPE data)
{
    if (root->size == 0)
    {
        root->root = (CLib_List_t *)malloc(sizeof(CLib_List_t));
        root->root->data = data;
        root->root->next = NULL;
    }
    else
    {

        CLib_List_t *rootNode = root->root;
        while (rootNode->next != NULL)
        {
            rootNode = rootNode->next;
        }

        rootNode->next = (CLib_List_t *)malloc(sizeof(CLib_List_t));
        rootNode->next->data = data;
        rootNode->next->next = NULL;
    }

    root->size++;
}

/**
 * @brief Pops an element from the end of CLib_List and returns it
 * 
 * @param root The pointer of the CLib_List_Root entity to pop and return from
 * @return CLib_List_TYPE The data popped off from the list
 */
CLib_List_TYPE CLib_List_popBack(CLib_List_Root *root)
{
    if (root->size < 1)
    {
        return CLib_List_ERROR_VALUE;
    }
    
    if (root->size == 1)
    {
        CLib_List_TYPE ret = root->root->data;
        free(root->root);
        root->root = NULL;

        return ret;
    }
    
    CLib_List_t *node = root->root;
    for (int i = 2; i < root->size; i++)
    {
        node = node->next;
    }

    CLib_List_TYPE ret = node->next->data;
    free(node->next);
    node->next = NULL;

    root->size--;

    return ret;
}

/**
 * @brief Pushes an element to the beginning of CLib_List_Root
 * 
 * @param root The pointer to the CLib_List_Root to push to
 * @param data The data to push to the beginning of the list
 */
void CLib_List_pushFront(CLib_List_Root *root, CLib_List_TYPE data)
{
    CLib_List_t *oldNodes = root->root;
    root->root = (CLib_List_t *)malloc(sizeof(CLib_List_t));

    root->root->data = data;
    root->root->next = oldNodes;

    root->size++;
}

/**
 * @brief Pops off an element from the beginning of the CLib_List_Root
 * 
 * @param root The pointer to the CLib_List_Root to pop off from
 * @return CLib_List_TYPE The data popped off from the list
 */
CLib_List_TYPE CLib_List_popFront(CLib_List_Root *root)
{
    CLib_List_t *rootNode = root->root;
    root->root = root->root->next;

    CLib_List_TYPE ret = rootNode->data;
    free(rootNode);

    root->size--;
    
    return ret;
}

/**
 * @brief Inserts an element to a given index in CLib_List @n
 *        Behaviour Description:
 *          - If index is less than size: The element is pushed after the node specified by index
 *          - If index is greater than or equal to size: The element is pushed to the end of the list
 * 
 * @param root The pointer to the CLib_List_Root to insert into
 * @param data The data to be inserted in the list
 * @param index The index to insert after in the CLib_List
 */
void CLib_List_insert(CLib_List_Root* root, CLib_List_TYPE data, unsigned int index)
{
    CLib_List_t *node = root->root;
    for (int i = 0; i < index; i++)
    {
        if (node->next == NULL)
        {
            break;
        }

        node = node->next;
    }

    CLib_List_t *nextPtr = node->next;

    node->next = (CLib_List_t *)malloc(sizeof(CLib_List_t));
    node->next->next = nextPtr;
    node->next->data = data;

    root->size++;
}

/**
 * @brief Returns the element contained by the list at a particular index
 * @note  This function returns ERROR_VALUE incase of bad indexing which mush be checked by the end user
 * 
 * @param root The pointer to the CLib_List_Root to get from
 * @param index The index to fetch the data from
 * @return CLib_List_TYPE The data at that index or ERROR_VALUE for bad index
 */
CLib_List_TYPE CLib_List_at(CLib_List_Root *root, int index)
{
    if(index > root->size - 1)
    {
        return CLib_List_ERROR_VALUE;
    }

    CLib_List_t *node = root->root;
    for (int i = 0; i < index; i++)
    {
        node = node->next;
    }

    return node->data;
}