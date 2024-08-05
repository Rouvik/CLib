/**
 * @file clib_macro.h
 * @author Rouvik Maji (majirouvik@gmail.com)
 * @brief Adds simple yet useful macros to codebase
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024 GNU GPL <https://www.gnu.org/licenses/>
 */

/**
 * @brief Gets the length of a stack allocated array by performing sizeof(arr) / sizeof(arr[0])
 */
#define CLIB_ARR_LEN(arr) \
    (sizeof(arr) / sizeof(arr[0]))

/**
 * @brief Gets the offset of a member of a struct
 */
#define CLIB_OFFSET_OF(type, member) \
    ((size_t) & (((type *)0)->member))

/**
 * @brief Gets the parent pointer to a struct from a pointer to its member
 */
#define CLIB_CONTAINER_OF(ptr, type, member)                     \
    ({                                                           \
        const typeof(((type *)0)->member) *__mptr = (ptr);       \
        (type *)((char *)__mptr - CLIB_OFFSET_OF(type, member)); \
    })

