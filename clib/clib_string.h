/*
    Adds dynamic string support to CLib library
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

#ifndef INCLUDED_CLIB_STRING
#define INCLUDED_CLIB_STRING

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef CLIB_BUF_CHUNK
    #define CLIB_BUF_CHUNK (256)
#endif // CLIB_BUF_CHUNK

/**
 * @brief Container to store the dynamic string ptr, length and buffer length @n
 *        Note: Do not edit any contents of CLib_String struct, especially the buf_len content, it is required to safely free memory
 */
typedef struct
{
    char *str;   // heap allocated string
    int len;     // length of string
    int buf_len; // actual length of the buffer
} CLib_String;

/**
 * @brief Initialises the CLib_String struct with a heap alocated string with appropriate memory
 *
 * @param src The source string to initialise with, must be null terminated, or else will lead to undefined behaviour
 * @return CLib_String The struct containing data about the string
 */
CLib_String CLib_String_init(const char *src)
{
    unsigned int count;
    int len = strlen(src);
    if (len == 0)
    {
        count = 1;
    }
    else
    {
        div_t divValue = div(len, CLIB_BUF_CHUNK);
        count = divValue.quot;
        if (divValue.rem) // allocate extra for spill
        {
            count++;
        }
    }

    int buf_len = CLIB_BUF_CHUNK * count;
    char *newMem = (char *)malloc(buf_len); // generate and copy the provided string to heap string
    memcpy(newMem, src, len + 1);

    return (CLib_String){
        newMem,
        len,
        buf_len};
}

/**
 * @brief Initialises the CLib_String struct with a length of len on a heap alocated string with appropriate memory
 *
 * @param src The source string to initialise with, must be null terminated, or else will lead to undefined behaviour
 * @param len The length of the string to copy and allocate for
 * @return CLib_String The struct containing data about the string
 */
CLib_String CLib_String_initN(const char *src, int len)
{
    unsigned int count;
    if (len == 0)
    {
        count = 1;
    }
    else
    {
        div_t divValue = div(len, CLIB_BUF_CHUNK);
        count = divValue.quot;
        if (divValue.rem) // allocate extra for spill
        {
            count++;
        }
    }

    int buf_len = CLIB_BUF_CHUNK * count;
    char *newMem = (char *)malloc(buf_len); // generate and copy the provided string to heap string

    int srcLen = strlen(src);
    memcpy(newMem, src, (srcLen > len ? len : srcLen) + 1);

    newMem[len] = '\0'; // make sure the last bit is \0

    return (CLib_String){
        newMem,
        len,
        buf_len};
}

/**
 * @brief Standard call for cstd free for char *str, also sets the internal states to 0
 *
 * @param str The CLib_String ref to free
 */
void CLib_String_deinit(CLib_String *str)
{
    free(str->str);
    str->len = 0;
    str->buf_len = 0;
}

/**
 * @brief Resets the provided CLib_String to empty state
 *
 * @param str The CLiber_String pointer to the String
 */
void CLib_String_reset(CLib_String *str)
{
    CLib_String_deinit(str);
    *str = CLib_String_init("");
}

/**
 * @brief Copies an existing CLib_String to a new CLib_String @n
 *        Note: The returned CLib_String is same as any other CLib_String and must be freed with
 *        CLib_String_deinit(CLib_String *) call
 *
 * @param str The CLib_String * to copy from
 * @return CLib_String The CLib_String copied to
 */
CLib_String CLib_String_copy(CLib_String *str)
{
    CLib_String newStr;
    newStr.str = (char *)malloc(str->buf_len); // assign new memory on heap
    newStr.buf_len = str->buf_len;     // copy the buf_len and len
    newStr.len = str->len;

    memcpy(newStr.str, str->str, str->buf_len); // actually copy the string
    return newStr;
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1
 *
 * @param str1 The first CLib_String* to concatenate to
 * @param str2 The second CLib_String* to concatenate from
 * @return CLib_String* The reference to str1
 */
CLib_String *CLib_String_concat(CLib_String *str1, CLib_String *str2)
{
    int max_len = str1->len + str2->len + 2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        div_t divValue = div(max_len, CLIB_BUF_CHUNK);
        if (divValue.rem) // allocate extra chunk for spill
        {
            divValue.quot++;
        }

        str1->buf_len = CLIB_BUF_CHUNK * divValue.quot; // set new buf_len
        str1->str = (char *) realloc(str1->str, str1->buf_len); // allocate more memory
    }

    memcpy((str1->str + str1->len), str2->str, str2->len + 1); // assumes str1 has enough space

    str1->len += str2->len; // assign the new len

    return str1; // return the str1 reference
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1
 *
 * @param str1 The first CLib_String* to concatenate to
 * @param str2 The second char* to concatenate from
 * @return CLib_String* The reference to str1
 */
CLib_String *CLib_String_concatCStr(CLib_String *str1, const char *str2)
{
    int len2 = strlen(str2);
    int max_len = str1->len + len2 + 2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        div_t divValue = div(max_len, CLIB_BUF_CHUNK);
        if (divValue.rem) // allocate extra chunk for spill
        {
            divValue.quot++;
        }

        str1->buf_len = CLIB_BUF_CHUNK * divValue.quot; // set new buf_len
        str1->str = (char *)realloc(str1->str, str1->buf_len); // allocate more memory
    }

    memcpy((str1->str + str1->len), str2, len2 + 1); // assumes str1 has enough space

    str1->len += len2; // assign the new length

    return str1; // return the str1 reference
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1 for count number of bytes
 *        also it automatically attaches a \0 character at the end of the string of str1
 *
 * @param str1 The first CLib_String* to concatenate to
 * @param str2 The second CLib_String* to concatenate from
 * @param count The number of characters to concatenate excluding the ending \0
 * @return CLib_String* The reference to str1
 */
CLib_String *CLib_String_concatN(CLib_String *str1, CLib_String *str2, int count)
{
    int max_len = str1->len + count + 2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        div_t divValue = div(max_len, CLIB_BUF_CHUNK);
        if (divValue.rem) // allocate extra chunk for spill
        {
            divValue.quot++;
        }

        str1->buf_len = CLIB_BUF_CHUNK * divValue.quot; // set new buf_len
        str1->str = (char *)realloc(str1->str, str1->buf_len); // allocate more memory
    }

    if (count < str2->len)
    {
        memcpy((str1->str + str1->len), str2->str, count); // assumes str1 has enough space
        *(str1->str + str1->len + count) = '\0'; // make sure it is null terminated
        str1->len += count; // assign the new len
    }
    else
    {
        memcpy((str1->str + str1->len), str2->str, str2->len); // assumes str1 has enough space
        *(str1->str + str1->len + str2->len) = '\0'; // make sure it is null terminated
        str1->len += str2->len; // assign the new len
    }

    return str1; // return the str1 reference
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1 for count number of bytes
 *        also it automatically attaches a \0 character at the end of the string of str1
 *
 * @param str1 The first CLib_String* to concatenate to
 * @param str2 The second char* to concatenate from
 * @param count The number of characters excluding the ending \0
 * @return CLib_String* The reference to str1
 */
CLib_String *CLib_String_concatCStrN(CLib_String *str1, char *str2, int count)
{
    int len2 = strlen(str2);
    int max_len = str1->len + count + 2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        div_t divValue = div(max_len, CLIB_BUF_CHUNK);
        if (divValue.rem) // allocate extra chunk for spill
        {
            divValue.quot++;
        }

        str1->buf_len = CLIB_BUF_CHUNK * divValue.quot; // set new buf_len
        str1->str = (char *)realloc(str1->str, str1->buf_len); // allocate more memory
    }

    if (count < len2)
    {
        memcpy((str1->str + str1->len), str2, count); // assumes str1 has enough space
        *(str1->str + str1->len + count) = '\0'; // make sure it is null terminated
        str1->len += count; // assign the new len
    }
    else
    {
        memcpy((str1->str + str1->len), str2, len2); // assumes str1 has enough space
        *(str1->str + str1->len + len2) = '\0'; // make sure it is null terminated
        str1->len += len2; // assign the new len
    }

    return str1; // return the str1 reference
}

/**
 * @brief Tokenizes an instance of CLib_String with the delimeters from char *delim string
 *
 * @param original The original CLib_String * to read from, must be passed each call, and must not be changed during calls
 * @param prevToken The previous token read from the tokenizer, must be set to {NULL, 0, 0} initially to begin with the process,
 *                  also this function is reentrant due to CLib_String *prevToken @n
 *                  Node: During first call, prevToken len and buf_len may not be initialised
 * @param delim The const char *delim string where each character is a delimeter
 */
void CLib_String_tokenizer(CLib_String *original, CLib_String *prevToken, const char *delim)
{
    if (prevToken->str == NULL) // first time tokenize
    {
        prevToken->str = original->str;
        prevToken->len = 0;
        prevToken->buf_len = 0; // assign the buffer length as 0 this proves that this string is a container string
    }
    else
    {
        prevToken->str += prevToken->len + 1; // update to new possible token location
    }

    char *ptr = prevToken->str;
    while (strchr(delim, *ptr) != NULL && *ptr != '\0') // make sure ptr points to a valid string loc
    {
        ptr++;
    }

    prevToken->str = ptr; // assign the new pointer location to tokenize from
    int len = 0;
    while (strchr(delim, *ptr) == NULL && ptr - original->str < original->len)
    {
        len++;
        ptr++;
    }

    prevToken->len = len;
}

/**
 * @brief Returns a CLib_String view of the substring part of the provided CLIB_String *str
 *        this view shares the same memory as *str and *may or may not be null terminated*
 *        must be used in tandem with the .len member of CLib_String, to mark it as a view,
 *        the .buf_len parameter of CLib_String is set to 0 @n
 *        <b>Note: The returned CLib_String (view) `must not be freed` with CLib_String_deinit() calls, as it shares memory with *str</b>
 * 
 * @param str The CLib_String to extract the substring from
 * @param start The start location of the substring
 * @param length The length of the substring
 * @return CLib_String The CLib_String(view, ie. .buf_len = 0, read above) containing the pointer to appropriate string and its length
 */
CLib_String CLib_String_substrView(CLib_String *str, unsigned int start, unsigned int length)
{
    if (start > str->len)
    {
        return (CLib_String){NULL, 0, 0};
    }
    
    return (CLib_String){.str = str->str + start, .len = length, .buf_len = 0};
}

/**
 * @brief Returns an actual allocated substring of the requested string with a valid .buf_len instead of its View counterpart
 *        CLib_String_substrView(), this must be freed with a call to CLib_String_deinit() call
 * 
 * @param str The CLib_String to extract the substring from
 * @param start The start location of the substring
 * @param length The length of the substring
 * @return CLib_String The CLib_String containing the substring and its length, must be freed with a call to CLib_String_deinit(&substr)
 */
CLib_String CLib_String_substr(CLib_String *str, unsigned int start, unsigned int length)
{
    if (start + length >= str->len)
    {
        return (CLib_String){NULL, 0, 0};
    }
    
    div_t divValue = div(length, CLIB_BUF_CHUNK);
    if (divValue.rem)
    {
        divValue.quot++;
    }
    
    CLib_String substr;
    substr.buf_len = divValue.quot * CLIB_BUF_CHUNK;
    substr.str = (char *)malloc(substr.buf_len);
    memcpy(substr.str, str->str + start, length);
    *(substr.str + length) = '\0';
    substr.len = length;

    return substr;
}

#endif // INCLUDED_CLIB_STRING