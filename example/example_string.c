#include <stdio.h>
#include "../clib/clib_string.h"

int main()
{
    CLib_String baseString = CLib_String_init("Hello "); // the base string to which we will do all our operations
    CLib_String anotherString = CLib_String_init("World"); // another string that will be concatenated

    printf("Concatenate CStrings: \"%s\" + \"%s\" = \"", baseString.str, anotherString.str);
    CLib_String_concat(&baseString, &anotherString);
    printf("%s\"\n", baseString.str);

    CLib_String_deinit(&anotherString); // free up the string once used
    
    char *anotherCStringToConcatenate = " and Bye!"; // a C Style string for concatenation
    printf("Concatenate a CString \"%s\" to the baseString from before \"%s\" = \"", anotherCStringToConcatenate, baseString.str);
    CLib_String_concatCStr(&baseString, anotherCStringToConcatenate);
    printf("%s\"\n", baseString.str);

    printf("Tokenization:\nThe string to be tokenized: \"%s\"\nTokens:\n", baseString.str);
    CLib_String token = {NULL, 0, 0}; // a string view to store the token information
    
    // REMEMBER: The token is only a String view and not an actual string
    //           It only points to a location in the baseString and a length
    //           As a result it has buf_len = 0 as it reserves no space in reality
    //           Dont attempt to free a CLib_String view like token as it points to baseString in reality!

    CLib_String_tokenizer(&baseString, &token, " ");
    while (token.str[0]) // while token points to a valid string location
    {
        // REMEMBER: The token is a pointer to original baseString thus direct printf or puts will print the entire string
        // puts(token.str) <== BAD WILL PRINT ENTIRE STRING AFTER TOKEN
        fwrite(token.str, token.len, 1, stdout);

        putchar('\n'); // adding a next line
        
        CLib_String_tokenizer(&baseString, &token, " "); // call to get next token
    }
    
    // More examples coming soon...

    CLib_String_deinit(&baseString); // always free the string once done!
    return 0;
}