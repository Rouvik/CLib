/*
    Adds template list support to CLib library
    Copyright (C) 2024  Rouvik Maji

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact: majirouvik@gmail.com
*/

/**
 * @file clib_list.h
 * @author Rouvik Maji (majirouvik@gmail.com)
 * @brief Adds template list support to CLib library
 * 
 * @note Documentation at: dev/clib_list_base_type.h
 * 
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024 GNU GPL <https://www.gnu.org/licenses/>
 */
#include <stdlib.h>

#define CLIB_LIST_SETUP(TYPE, ERROR_VALUE)                                                       \
    typedef struct CLib_List_##TYPE                                                              \
    {                                                                                            \
        TYPE data;                                                                               \
        struct CLib_List_##TYPE *next;                                                           \
    } CLib_List_##TYPE##_t;                                                                      \
                                                                                                 \
    typedef struct                                                                               \
    {                                                                                            \
        int size;                                                                                \
                                                                                                 \
        CLib_List_##TYPE##_t *root;                                                              \
    } CLib_List_##TYPE##_Root;                                                                   \
                                                                                                 \
    CLib_List_##TYPE##_Root CLib_List_##TYPE##_init(TYPE data)                                   \
    {                                                                                            \
        CLib_List_##TYPE##_Root list;                                                            \
        list.size = 1;                                                                           \
        list.root = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));                \
        list.root->data = data;                                                                  \
        list.root->next = NULL;                                                                  \
                                                                                                 \
        return list;                                                                             \
    }                                                                                            \
                                                                                                 \
    CLib_List_##TYPE##_Root CLib_List_##TYPE##_initEmpty()                                       \
    {                                                                                            \
        return (CLib_List_##TYPE##_Root){.size = 0, .root = NULL};                               \
    }                                                                                            \
                                                                                                 \
    void CLib_List_##TYPE##_deinit(CLib_List_##TYPE##_Root *root)                                \
    {                                                                                            \
        CLib_List_##TYPE##_t *node = root->root;                                                 \
        while (node->next != NULL)                                                               \
        {                                                                                        \
            CLib_List_##TYPE##_t *present = node;                                                \
            node = node->next;                                                                   \
                                                                                                 \
            free(present);                                                                       \
        }                                                                                        \
                                                                                                 \
        free(node);                                                                              \
                                                                                                 \
        root->root = NULL;                                                                       \
        root->size = 0;                                                                          \
    }                                                                                            \
                                                                                                 \
    void CLib_List_##TYPE##_pushBack(CLib_List_##TYPE##_Root *root, TYPE data)                   \
    {                                                                                            \
        if (root->size == 0)                                                                     \
        {                                                                                        \
            root->root = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));           \
            root->root->data = data;                                                             \
            root->root->next = NULL;                                                             \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
                                                                                                 \
            CLib_List_##TYPE##_t *rootNode = root->root;                                         \
            while (rootNode->next != NULL)                                                       \
            {                                                                                    \
                rootNode = rootNode->next;                                                       \
            }                                                                                    \
                                                                                                 \
            rootNode->next = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));       \
            rootNode->next->data = data;                                                         \
            rootNode->next->next = NULL;                                                         \
        }                                                                                        \
                                                                                                 \
        root->size++;                                                                            \
    }                                                                                            \
                                                                                                 \
    TYPE CLib_List_##TYPE##_popBack(CLib_List_##TYPE##_Root *root)                               \
    {                                                                                            \
        if (root->size < 1)                                                                      \
        {                                                                                        \
            return ERROR_VALUE;                                                                  \
        }                                                                                        \
        if (root->size == 1)                                                                     \
        {                                                                                        \
            TYPE ret = root->root->data;                                                         \
            free(root->root);                                                                    \
            root->root = NULL;                                                                   \
                                                                                                 \
            return ret;                                                                          \
        }                                                                                        \
        CLib_List_##TYPE##_t *node = root->root;                                                 \
        for (int i = 2; i < root->size; i++)                                                     \
        {                                                                                        \
            node = node->next;                                                                   \
        }                                                                                        \
                                                                                                 \
        TYPE ret = node->next->data;                                                             \
        free(node->next);                                                                        \
        node->next = NULL;                                                                       \
                                                                                                 \
        root->size--;                                                                            \
                                                                                                 \
        return ret;                                                                              \
    }                                                                                            \
                                                                                                 \
    void CLib_List_##TYPE##_pushFront(CLib_List_##TYPE##_Root *root, TYPE data)                  \
    {                                                                                            \
        CLib_List_##TYPE##_t *oldNodes = root->root;                                             \
        root->root = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));               \
                                                                                                 \
        root->root->data = data;                                                                 \
        root->root->next = oldNodes;                                                             \
                                                                                                 \
        root->size++;                                                                            \
    }                                                                                            \
                                                                                                 \
    TYPE CLib_List_##TYPE##_popFront(CLib_List_##TYPE##_Root *root)                              \
    {                                                                                            \
        CLib_List_##TYPE##_t *rootNode = root->root;                                             \
        root->root = root->root->next;                                                           \
                                                                                                 \
        TYPE ret = rootNode->data;                                                               \
        free(rootNode);                                                                          \
                                                                                                 \
        root->size--;                                                                            \
        return ret;                                                                              \
    }                                                                                            \
                                                                                                 \
    void CLib_List_##TYPE##_insert(CLib_List_##TYPE##_Root *root, TYPE data, unsigned int index) \
    {                                                                                            \
        CLib_List_##TYPE##_t *node = root->root;                                                 \
        for (int i = 0; i < index; i++)                                                          \
        {                                                                                        \
            if (node->next == NULL)                                                              \
            {                                                                                    \
                break;                                                                           \
            }                                                                                    \
                                                                                                 \
            node = node->next;                                                                   \
        }                                                                                        \
                                                                                                 \
        CLib_List_##TYPE##_t *nextPtr = node->next;                                              \
                                                                                                 \
        node->next = (CLib_List_##TYPE##_t *)malloc(sizeof(CLib_List_##TYPE##_t));               \
        node->next->next = nextPtr;                                                              \
        node->next->data = data;                                                                 \
                                                                                                 \
        root->size++;                                                                            \
    }                                                                                            \
                                                                                                 \
    TYPE CLib_List_##TYPE##_at(CLib_List_##TYPE##_Root *root, int index)                         \
    {                                                                                            \
        if (index > root->size - 1)                                                              \
        {                                                                                        \
            return ERROR_VALUE;                                                                  \
        }                                                                                        \
                                                                                                 \
        CLib_List_##TYPE##_t *node = root->root;                                                 \
        for (int i = 0; i < index; i++)                                                          \
        {                                                                                        \
            node = node->next;                                                                   \
        }                                                                                        \
                                                                                                 \
        return node->data;                                                                       \
    }

// EOF