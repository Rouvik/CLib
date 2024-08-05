/**
 * @file clib_vector_base_type.h
 * @author Rouvik Maji (majirouvik@gmail.com)
 * @brief The dev base type code for templated clib/clib_vector.h, any updates must be made here first and then converted to macro form for templating
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024 GNU GPL <https://www.gnu.org/licenses/>
 */

#include <stdlib.h>
#include <string.h>

/**
 * @brief The basic number of elements reserved by the vector @n
 *        The vector usually reserves elements in multiples of CLIB_VECTOR_RESERVE
 */
#define CLIB_VECTOR_RESERVE (5)

#define TYPE int
#define ERROR 0

/**
 * @brief The container for CLib_Vector storing the pointer to the underlying TYPE *data list, size and reserved elements
 */
typedef struct CLib_Vector
{
    TYPE *data;
    unsigned int size;
    unsigned int reserved;
} CLib_Vector_t;

/**
 * @brief Initialises a CLib_Vector_t with 0 elements and reserved elements of CLIB_VECTOR_RESERVE
 * 
 * @return CLib_Vector_t The vector initialised
 */
CLib_Vector_t CLib_Vector_init()
{
    CLib_Vector_t vector;
    vector.data = (TYPE *)malloc(sizeof(TYPE) * CLIB_VECTOR_RESERVE);
    vector.size = 0;
    vector.reserved = CLIB_VECTOR_RESERVE;

    return vector;
}

/**
 * @brief Frees the CLib_Vector_t from memory and sets it to a zero state ie, {.data = NULL, .size = 0, .reserved = 0}
 * 
 * @param vector The pointer to CLib_Vector_t to free
 */
void CLib_Vector_deinit(CLib_Vector_t *vector)
{
    free(vector->data);
    vector->size = 0;
    vector->reserved = 0;
}

/**
 * @brief Pushes an element data to the end of CLib_Vector
 * 
 * @param vector The pointer to CLib_Vector to push to
 * @param data The data to be pushed at the end
 */
void CLib_Vector_pushBack(CLib_Vector_t *vector, TYPE data)
{
    if (vector->size == vector->reserved)
    {
        vector->reserved += CLIB_VECTOR_RESERVE;
        vector->data = (TYPE *)realloc(vector->data, vector->reserved * sizeof(TYPE));
    }

    vector->data[(vector->size)++] = data;
}

/**
 * @brief Pops off an element from the end of CLib_Vector and returns it
 * 
 * @param vector The pointer to CLib_Vector to pop off from
 * @return TYPE The data popped off from the end
 */
TYPE CLib_Vector_popBack(CLib_Vector_t *vector)
{
    if (vector->size)
    {
        return vector->data[--(vector->size)];
    }
    else
    {
        return ERROR;
    }
}

/**
 * @brief Pushes an element to the beginning of CLib_Vector
 * 
 * @param vector The pointer to the CLib_Vector to push at the beginning
 * @param data The data to be pushed at the beginning
 */
void CLib_Vector_pushFront(CLib_Vector_t *vector, TYPE data)
{
    if (vector->size == vector->reserved)
    {
        vector->reserved += CLIB_VECTOR_RESERVE;
        vector->data = (TYPE *)realloc(vector->data, vector->reserved * sizeof(TYPE));
    }

    memcpy(vector->data + 1, vector->data, vector->size * sizeof(TYPE));
    vector->data[0] = data;
    (vector->size)++;
}

/**
 * @brief Pops off an element from the beginning of the list and returns it
 * 
 * @param vector The pointer to the CLib_Vector to pop off from
 * @return TYPE The data popped off from the CLib_Vector
 */
TYPE CLib_Vector_popFront(CLib_Vector_t *vector)
{
    if (vector->size)
    {
        TYPE data = vector->data[0];
        memcpy(vector->data, vector->data + 1, (vector->size - 1) * sizeof(TYPE));
        (vector->size)--;

        return data;
    }
    else
    {
        return ERROR;
    }
}

/**
 * @brief Inserts and sets (if the element is already present) to the CLib_Vector @n
 *        Behaviour Description:
 *          - If position is less than size: Sets the element at the position with the data
 *          - If position is greater than or equal to size: Adds 0 values to the uninitialised elements and sets the element at position
 *        @n
 *        Note: All of the above operations will resize the vector if required
 * 
 * @param vector The pointer to the CLib_Vector to insert into
 * @param position The position to insert into
 * @param data The data to insert at position
 */
void CLib_Vector_insert(CLib_Vector_t *vector, int position, TYPE data)
{
    if (position > vector->reserved)
    {
        div_t divValue = div(position, CLIB_VECTOR_RESERVE);
        if (divValue.rem)
        {
            divValue.quot++;
        }
        divValue.quot *= CLIB_VECTOR_RESERVE;

        vector->data = (TYPE *)realloc(vector->data, divValue.quot * sizeof(TYPE));
        vector->reserved = divValue.quot;
    }

    vector->data[position] = data;

    if (position > vector->size)
    {
        memset(vector->data, 0, (position - vector->size) * sizeof(TYPE));
        vector->size = position;
    }

    vector->size++;
}

/**
 * @brief Reserves the vector with an arbitary amount of expected_elements, this function can actually reserve for an arbitary size and not a multiple of CLIB_VECTOR_RESERVE
 * 
 * @param vector The pointer to CLib_Vector to reserve for
 * @param expected_elements The number of elements expected by the vector
 */
void CLib_Vector_reserve(CLib_Vector_t *vector, int expected_elements)
{
    int new_size = expected_elements * CLIB_VECTOR_RESERVE;
    if (new_size > vector->reserved)
    {
        vector->data = (TYPE *)realloc(vector->data, new_size * sizeof(TYPE));
        vector->reserved = new_size;
    }
}

/**
 * @brief Fills a part of the vector with a particular value and possibly resizes the vector to accomodate
 * 
 * @param vector The pointer to the CLib_Vector to fill
 * @param value The value to be filled in the vector
 * @param offset The offset at which filling begins
 * @param count The number of elements to fill
 */
void CLib_Vector_fill(CLib_Vector_t *vector, TYPE value, unsigned int offset, unsigned int count)
{
    int lastPos = offset + count;
    if (lastPos > vector->reserved)
    {
        div_t divValue = div(lastPos, CLIB_VECTOR_RESERVE);
        if (divValue.rem)
        {
            divValue.quot++;
        }
        divValue.quot *= CLIB_VECTOR_RESERVE;

        vector->data = (TYPE *)realloc(vector->data, divValue.quot * sizeof(TYPE));
        vector->reserved = divValue.quot;
    }

    for (int i = offset; i < lastPos; i++)
    {
        vector->data[i] = value;
    }

    if (offset > vector->size)
    {
        memset(vector->data + vector->size, 0, (offset - vector->size) * sizeof(TYPE));
    }

    if (lastPos > vector->size)
    {
        vector->size = lastPos;
    }
}