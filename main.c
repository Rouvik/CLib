#include <stdio.h>

#include "clib/clib_fs.h"

int main()
{
    CLib_Fs_writeFileCStr("./test.txt", "Created File from Raw! Waw!", 5, true);
    return 0;
}