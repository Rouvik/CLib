#include <stdio.h>

#include "clib/clib_string.h"
#include "clib/clib_fs.h"

int main()
{
    CLib_String contents = CLib_String_init("");
    if(CLib_Fs_readFile("./clib/clib_fs.h", &contents))
    {
        fprintf(stderr, "Error failed to read file\n");
        return 1;
    }

    printf("Output: \"%s\"\n", contents.str);

    CLib_String_deinit(&contents);
    return 0;
}