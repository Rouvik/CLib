#include <stdlib.h>

#define CLib_List_setup(TYPE, ERROR_VALUE)                                                 \
    typedef struct CLib_List_##TYPE                                                        \
    {                                                                                      \
        TYPE data;                                                                         \
        struct CLib_List_##TYPE *next;                                                     \
    } CLib_List_##TYPE##_t;                                                                \
                                                                                           \
    typedef struct                                                                         \
    {                                                                                      \
        int size;                                                                          \
                                                                                           \
        CLib_List_##TYPE##_t *root;                                                        \
    } CLib_List_##TYPE##_Root;                                                             \
                                                                                           \
    CLib_List_##TYPE##_Root CLib_List_##TYPE##_init(TYPE data)                             \
    {                                                                                      \
        CLib_List_##TYPE##_Root list;                                                      \
        list.size = 1;                                                                     \
        list.root = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));          \
        list.root->data = data;                                                            \
        list.root->next = NULL;                                                            \
                                                                                           \
        return list;                                                                       \
    }                                                                                      \
                                                                                           \
    CLib_List_##TYPE##_Root CLib_List_##TYPE##_initEmpty()                                 \
    {                                                                                      \
        return (CLib_List_##TYPE##_Root){.size = 0, .root = NULL};                         \
    }                                                                                      \
                                                                                           \
    void CLib_List_##TYPE##_deinit(CLib_List_##TYPE##_Root *root)                          \
    {                                                                                      \
        CLib_List_##TYPE##_t *node = root->root;                                           \
        while (node->next != NULL)                                                         \
        {                                                                                  \
            CLib_List_##TYPE##_t *present = node;                                          \
            node = node->next;                                                             \
                                                                                           \
            free(present);                                                                 \
        }                                                                                  \
                                                                                           \
        free(node);                                                                        \
                                                                                           \
        root->root = NULL;                                                                 \
        root->size = 0;                                                                    \
    }                                                                                      \
                                                                                           \
    void CLib_List_##TYPE##_pushBack(CLib_List_##TYPE##_Root *root, TYPE data)             \
    {                                                                                      \
        if (root->size == 0)                                                               \
        {                                                                                  \
            root->root = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));     \
            root->root->data = data;                                                       \
            root->root->next = NULL;                                                       \
        }                                                                                  \
        else                                                                               \
        {                                                                                  \
                                                                                           \
            CLib_List_##TYPE##_t *rootNode = root->root;                                   \
            while (rootNode->next != NULL)                                                 \
            {                                                                              \
                rootNode = rootNode->next;                                                 \
            }                                                                              \
                                                                                           \
            rootNode->next = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t)); \
            rootNode->next->data = data;                                                   \
            rootNode->next->next = NULL;                                                   \
        }                                                                                  \
                                                                                           \
        root->size++;                                                                      \
    }                                                                                      \
                                                                                           \
    TYPE CLib_List_##TYPE##_popBack(CLib_List_##TYPE##_Root *root)                         \
    {                                                                                      \
        if (root->size < 1)                                                                \
        {                                                                                  \
            return ERROR_VALUE;                                                            \
        }                                                                                  \
        \   
    if (root->size == 1)                                                                   \
        {                                                                                  \
            TYPE ret = root->root->data;                                                   \
            free(root->root);                                                              \
            root->root = NULL;                                                             \
                                                                                           \
            return ret;                                                                    \
        }                                                                                  \
        \   
    CLib_List_##TYPE##_t *node = root->root;                                               \
        for (int i = 2; i < root->size; i++)                                               \
        {                                                                                  \
            node = node->next;                                                             \
        }                                                                                  \
                                                                                           \
        TYPE ret = node->next->data;                                                       \
        free(node->next);                                                                  \
        node->next = NULL;                                                                 \
                                                                                           \
        root->size--;                                                                      \
                                                                                           \
        return ret;                                                                        \
    }                                                                                      \
                                                                                           \
    void CLib_List_##TYPE##_pushFront(CLib_List_##TYPE##_Root *root, TYPE data)            \
    {                                                                                      \
        CLib_List_##TYPE##_t *oldNodes = root->root;                                       \
        root->root = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));         \
                                                                                           \
        root->root->data = data;                                                           \
        root->root->next = oldNodes;                                                       \
                                                                                           \
        root->size++;                                                                      \
    }                                                                                      \
                                                                                           \
    TYPE CLib_List_##TYPE##_popFront(CLib_List_##TYPE##_Root *root)                        \
    {                                                                                      \
        CLib_List_##TYPE##_t *rootNode = root->root;                                       \
        root->root = root->root->next;                                                     \
                                                                                           \
        TYPE ret = rootNode->data;                                                         \
        free(rootNode);                                                                    \
                                                                                           \
        root->size--;                                                                      \
        \    
    return ret;                                                                            \
    }                                                                                      \
                                                                                           \
    TYPE CLib_List_##TYPE##_at(CLib_List_##TYPE##_Root *root, int index)                   \
    {                                                                                      \
        if (index > root->size - 1)                                                        \
        {                                                                                  \
            return ERROR_VALUE;                                                            \
        }                                                                                  \
                                                                                           \
        CLib_List_##TYPE##_t *node = root->root;                                           \
        for (int i = 0; i < index; i++)                                                    \
        {                                                                                  \
            node = node->next;                                                             \
        }                                                                                  \
                                                                                           \
        return node->data;                                                                 \
    }