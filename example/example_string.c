/*
    How to use these examples:
    Compile and run the program and look for the code you need help for
    Each Example section is headed with a header `EXAMPLE: <Some heading...>`
    Look for the appropriate header and search for it in the source file
*/

#include <stdio.h>
#include "../clib/clib_string.h"

#define EXAMPLE(name) \
    puts("------------------------------- EXAMPLE: " #name " -------------------------------\n");

#define EXAMPLE_END \
puts("\n\n");

int main()
{
    CLib_String baseString = CLib_String_init("Hello "); // the base string to which we will do all our operations

    EXAMPLE(Concat two CLib_Strings)
    {
        CLib_String anotherString = CLib_String_init("World"); // another string that will be concatenated

        printf("Concatenate CStrings: \"%s\" + \"%s\" = \"", baseString.str, anotherString.str);
        CLib_String_concat(&baseString, &anotherString);
        printf("%s\"\n", baseString.str);

        CLib_String_deinit(&anotherString); // free up the string once used
    }
    EXAMPLE_END

    EXAMPLE(Concat CLib_String and C String)
    {
        char *anotherCStringToConcatenate = " and Bye!"; // a C Style string for concatenation
        printf("Concatenate a CString \"%s\" to the baseString from before \"%s\" = \"", anotherCStringToConcatenate, baseString.str);
        CLib_String_concatCStr(&baseString, anotherCStringToConcatenate);
        printf("%s\"\n", baseString.str);
    }
    EXAMPLE_END

    EXAMPLE(Tokenization)
    {
        printf("Tokenization:\nThe string to be tokenized: \"%s\"\nTokens:\n", baseString.str);
        CLib_String token = {NULL, 0, 0}; // a string view to store the token information

        // REMEMBER: The token is only a String view and not an actual string
        //           It only points to a location in the baseString and a length
        //           As a result it has buf_len = 0 as it reserves no space in reality
        //           Attempt to free a CLib_String view like token will do nothing in reality and will be ignored

        CLib_String_tokenizer(&baseString, &token, " ");
        while (token.str[0]) // while token points to a valid string location
        {
            // REMEMBER: The token is a pointer to original baseString thus direct printf or puts will print the entire string
            // puts(token.str) <== BAD WILL PRINT ENTIRE STRING AFTER TOKEN
            fwrite(token.str, token.len, 1, stdout);

            putchar('\n'); // adding a next line

            CLib_String_tokenizer(&baseString, &token, " "); // call to get next token
        }
    }
    EXAMPLE_END

    EXAMPLE(Substring a String View from CLib_String)
    {
        printf("CLib_String to substring a string view from = \"%s\" with begin at 4 and extract 9 characters\n", baseString.str);
        CLib_String sub = CLib_String_substrView(&baseString, 4, 9);
        printf("Substring = \"");
        fwrite(sub.str, sub.len, 1, stdout); // writing only the required part as sub is not a real string
        puts("\"\n");

        // REMEMBER: Again we do not deinit sub as it is only a String View with no actual memory allocation
        //           Calling CLib_String_deinit(&sub) will have no effect
    }
    EXAMPLE_END
    
    EXAMPLE(Substring a CLib_String from another CLib_String)
    {
        printf("CLib_String to substring a string view from = \"%s\" with begin at 3 and extract 7 characters\n", baseString.str);
        CLib_String sub = CLib_String_substr(&baseString, 3, 7);
        printf("Substring = \"%s\"\n", sub.str);
        CLib_String_deinit(&sub);
    }
    EXAMPLE_END

    CLib_String_deinit(&baseString); // always free the string once done!

    CLib_String base2;

    EXAMPLE(Initialise a CLib_String with N chars)
    {
        char *source = "Hello world and some bad string to ignore";
        printf("Source to initialise from = \"%s\" with N = 11\n", source);
        base2 = CLib_String_initN(source, 11);
        printf("Initialised CLib_String contents = \"%s\"\n", base2.str);
    }
    EXAMPLE_END

    EXAMPLE(Copy a CLib_String)
    {
        printf("String to copy from = \"%s\"\n", base2.str);
        CLib_String cpy = CLib_String_copy(&base2);
        printf("Copied = \"%s\"\n", cpy.str);
        
        char *someStr = "Some random string to concatenate";
        printf("String that we will concatenate to our copy = \"%s\"\n", someStr);
        CLib_String_concatCStr(&cpy, someStr);
        printf("Contents of copy string = \"%s\"\n", cpy.str);
        printf("Contents of original string = \"%s\"\n", base2.str);

        CLib_String_deinit(&cpy);
    }
    EXAMPLE_END

    EXAMPLE(Concat N chars from CLib_String to another CLib_String)
    {
        CLib_String temp = CLib_String_copy(&base2);
        CLib_String toConcat = CLib_String_init(" valid part and invalid part");
        printf("Original string = \"%s\"\nString to concatenate = \"%s\"\nN = 11\n", temp.str, toConcat.str);
        CLib_String_concatN(&temp, &toConcat, 11);
        printf("Concatenated string = \"%s\"\n", temp.str);

        CLib_String_deinit(&temp);
        CLib_String_deinit(&toConcat);
    }
    EXAMPLE_END

    EXAMPLE(Concat N chars from C String to another CLib_String)
    {
        char *toConcat = " another valid part [hiri cumis onvelod pert]"; // Easter egg: Identify the encoding logic of invalid part ;) (its too simple trust me, hint: "vowels")
        printf("Original String = \"%s\"\nString to concatenate = \"%s\"\n", base2.str, toConcat);
        CLib_String_concatCStrN(&base2, toConcat, 19);
        printf("Concatenated string = \"%s\"\n", base2.str);
    }
    EXAMPLE_END

    CLib_String_deinit(&base2);
    return 0;
}