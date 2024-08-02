/*
    Adds template vector support to CLib library
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

#include <stdlib.h>
#include <string.h>

#define CLIB_VECTOR_RESERVE (5)

#define CLIB_VECTOR_SETUP(TYPE, ERROR_VALUE)                                                                            \
    typedef struct CLib_Vector_##TYPE                                                                                   \
    {                                                                                                                   \
        TYPE *data;                                                                                                     \
        unsigned int size;                                                                                              \
        unsigned int reserved;                                                                                          \
    } CLib_Vector_##TYPE##_t;                                                                                           \
                                                                                                                        \
    CLib_Vector_##TYPE##_t CLib_Vector_##TYPE##_init()                                                                  \
    {                                                                                                                   \
        CLib_Vector_##TYPE##_t vector;                                                                                  \
        vector.data = (TYPE *)malloc(sizeof(TYPE) * CLIB_VECTOR_RESERVE);                                               \
        vector.size = 0;                                                                                                \
        vector.reserved = CLIB_VECTOR_RESERVE;                                                                          \
                                                                                                                        \
        return vector;                                                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    void CLib_Vector_##TYPE##_deinit(CLib_Vector_##TYPE##_t *vector)                                                    \
    {                                                                                                                   \
        free(vector->data);                                                                                             \
        vector->size = 0;                                                                                               \
        vector->reserved = 0;                                                                                           \
    }                                                                                                                   \
                                                                                                                        \
    void CLib_Vector_##TYPE##_pushBack(CLib_Vector_##TYPE##_t *vector, TYPE data)                                       \
    {                                                                                                                   \
        if (vector->size == vector->reserved)                                                                           \
        {                                                                                                               \
            vector->reserved += CLIB_VECTOR_RESERVE;                                                                    \
            vector->data = (TYPE *)realloc(vector->data, vector->reserved * sizeof(TYPE));                              \
        }                                                                                                               \
                                                                                                                        \
        vector->data[(vector->size)++] = data;                                                                          \
    }                                                                                                                   \
                                                                                                                        \
    TYPE CLib_Vector_##TYPE##_popBack(CLib_Vector_##TYPE##_t *vector)                                                   \
    {                                                                                                                   \
        if (vector->size)                                                                                               \
        {                                                                                                               \
            return vector->data[--(vector->size)];                                                                      \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            return ERROR_VALUE;                                                                                               \
        }                                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    void CLib_Vector_##TYPE##_pushFront(CLib_Vector_##TYPE##_t *vector, TYPE data)                                      \
    {                                                                                                                   \
        if (vector->size == vector->reserved)                                                                           \
        {                                                                                                               \
            vector->reserved += CLIB_VECTOR_RESERVE;                                                                    \
            vector->data = (TYPE *)realloc(vector->data, vector->reserved * sizeof(TYPE));                              \
        }                                                                                                               \
                                                                                                                        \
        memcpy(vector->data + 1, vector->data, vector->size * sizeof(TYPE));                                            \
        vector->data[0] = data;                                                                                         \
        (vector->size)++;                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    TYPE CLib_Vector_##TYPE##_popFront(CLib_Vector_##TYPE##_t *vector)                                                  \
    {                                                                                                                   \
        if (vector->size)                                                                                               \
        {                                                                                                               \
            TYPE data = vector->data[0];                                                                                \
            memcpy(vector->data, vector->data + 1, (vector->size - 1) * sizeof(TYPE));                                  \
            (vector->size)--;                                                                                           \
                                                                                                                        \
            return data;                                                                                                \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            return ERROR_VALUE;                                                                                         \
        }                                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    void CLib_Vector_##TYPE##_insert(CLib_Vector_##TYPE##_t *vector, int position, TYPE data)                           \
    {                                                                                                                   \
        if (position > vector->reserved)                                                                                \
        {                                                                                                               \
            div_t divValue = div(position, CLIB_VECTOR_RESERVE);                                                        \
            if (divValue.rem)                                                                                           \
            {                                                                                                           \
                divValue.quot++;                                                                                        \
            }                                                                                                           \
            divValue.quot *= CLIB_VECTOR_RESERVE;                                                                       \
                                                                                                                        \
            vector->data = (TYPE *)realloc(vector->data, divValue.quot * sizeof(TYPE));                                 \
            vector->reserved = divValue.quot;                                                                           \
        }                                                                                                               \
                                                                                                                        \
        vector->data[position] = data;                                                                                  \
                                                                                                                        \
        if (position > vector->size)                                                                                    \
        {                                                                                                               \
            memset(vector->data, 0, (position - vector->size) * sizeof(TYPE));                                          \
            vector->size = position;                                                                                    \
        }                                                                                                               \
                                                                                                                        \
        vector->size++;                                                                                                 \
    }                                                                                                                   \
                                                                                                                        \
    void CLib_Vector_##TYPE##_reserve(CLib_Vector_##TYPE##_t *vector, int expected_elements)                            \
    {                                                                                                                   \
        int new_size = expected_elements * CLIB_VECTOR_RESERVE;                                                         \
        if (new_size > vector->reserved)                                                                                \
        {                                                                                                               \
            vector->data = (TYPE *)realloc(vector->data, new_size * sizeof(TYPE));                                      \
            vector->reserved = new_size;                                                                                \
        }                                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    void CLib_Vector_##TYPE##_fill(CLib_Vector_##TYPE##_t *vector, TYPE value, unsigned int offset, unsigned int count) \
    {                                                                                                                   \
        int lastPos = offset + count;                                                                                   \
        if (lastPos > vector->reserved)                                                                                 \
        {                                                                                                               \
            div_t divValue = div(lastPos, CLIB_VECTOR_RESERVE);                                                         \
            if (divValue.rem)                                                                                           \
            {                                                                                                           \
                divValue.quot++;                                                                                        \
            }                                                                                                           \
            divValue.quot *= CLIB_VECTOR_RESERVE;                                                                       \
                                                                                                                        \
            vector->data = (TYPE *)realloc(vector->data, divValue.quot * sizeof(TYPE));                                 \
            vector->reserved = divValue.quot;                                                                           \
        }                                                                                                               \
                                                                                                                        \
        for (int i = offset; i < lastPos; i++)                                                                          \
        {                                                                                                               \
            vector->data[i] = value;                                                                                    \
        }                                                                                                               \
                                                                                                                        \
        if (offset > vector->size)                                                                                      \
        {                                                                                                               \
            memset(vector->data + vector->size, 0, (offset - vector->size) * sizeof(TYPE));                             \
        }                                                                                                               \
                                                                                                                        \
        if (lastPos > vector->size)                                                                                     \
        {                                                                                                               \
            vector->size = lastPos;                                                                                     \
        }                                                                                                               \
    }

// EOF