/**
 * @file clib_macro.h
 * @author Rouvik Maji (majirouvik@gmail.com)
 * @brief Adds simple yet useful macros to codebase
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024 GNU GPL <https://www.gnu.org/licenses/>
 */

#include <stdio.h>

/**
 * @brief Gets the length of a stack allocated array by performing sizeof(arr) / sizeof(arr[0])
 */
#define CLIB_ARR_LEN(arr) \
    (sizeof(arr) / sizeof(arr[0]))

/**
 * @brief An alternative to #X in macros, used mainly for inner macro expansion and stringification
 */
#define CLIB_STRINGIFY(x) #x

/**
 * @brief Converts a macro constant to string using inner macro expansion
 */
#define CLIB_TOSTRING(x) CLIB_STRINGIFY(x)

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

/**
 * @brief Basic log function using puts, logs to stdout
 * @code
 * CLIB_LOG(DEFAULT, "Some message") => [DEFAULT LOG] Some message
 * @endcode
 */
#define CLIB_LOG(tag, message) \
puts("[" #tag " LOG] " message);

/**
 * @brief Basic log function using fputs, logs to stderr
 * @code
 * CLIB_ERR(DEFAULT, "Some message") => [DEFAULT ERROR] Some message
 * @endcode
 */
#define CLIB_ERR(tag, message) \
fputs("[" #tag " ERROR] " message "\n", stderr);

/**
 * @brief Logs with file name and line number included to stdout using puts
 * @code
 * CLIB_FLOG(DEFAULT, "Some message") => At main.c:7 :: [DEFAULT LOG] Some message
 * @endcode
 */
#define CLIB_FLOG(tag, message) \
puts("At " __FILE__ ":" CLIB_TOSTRING(__LINE__) " :: " "[" #tag " LOG] " message);

/**
 * @brief Logs with file name and line number included to stderr using fputs
 * @code
 * CLIB_FERR(DEFAULT, "Some message") => At main.c:7 :: [DEFAULT ERROR] Some message
 * @endcode
 */
#define CLIB_FERR(tag, message) \
fputs("At " __FILE__ ":" CLIB_TOSTRING(__LINE__) " :: " "[" #tag " ERROR] " message "\n", stderr);