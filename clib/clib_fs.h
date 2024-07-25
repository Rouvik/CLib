/*
                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
*/

#ifndef INCLUDED_CLIB_FS
#define INCLUDED_CLIB_FS

#include <stdio.h>
#include <stdint.h>

#include "clib_string.h"

#define CLIB_FS_DIRMODE_FILES 1
#define CLIB_FS_DIRMODE_FOLDERS 1 << 1

CLib_String CLib_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim);

#ifdef _WIN32 // systems with win api

#include <windows.h>

CLib_String CLib_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim)
{
    WIN32_FIND_DATAA fdFile;
    HANDLE hFind = NULL;

    if (mask[0] == '\0') // make sure mask is not error type
    {
        mask = "*.*";
    }

    int pathLen = strlen(path);
    int maskLen = strlen(mask);

    char cpyPath[pathLen + maskLen + 1];
    memcpy(cpyPath, path, pathLen);
    memcpy(cpyPath + pathLen, mask, maskLen + 1);

    CLib_String output = CLib_String_init("");

    if ((hFind = FindFirstFileA(cpyPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "[CBuilder FS Error] Failed to open path %s\n", cpyPath);
        CLib_String_deinit(&output);
        return (CLib_String){NULL, 0, 0};
    }

    do
    {
        if ((fdFile.cFileName[0] == '.') && (fdFile.cFileName[1] == '\0' || fdFile.cFileName[1] == '.'))
        {
            continue;
        }

        if ((mode & CLIB_FS_DIRMODE_FOLDERS) && (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            CLib_String_concatCStr(&output, fdFile.cFileName);
        }
        else if (mode & CLIB_FS_DIRMODE_FILES && !(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            CLib_String_concatCStr(&output, fdFile.cFileName);
        }

        CLib_String_concatCStr(&output, (char *)delim); // add the ending delimeter
    } while (FindNextFileA(hFind, &fdFile));

    FindClose(hFind); // cleanup

    return output;
}

#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__) // systems supporting dirent.h

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
CLib_String CLib_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim)
{
    if (mask[0] == '/') // if first character is / then ignore
    {
        mask++;
    }

    struct dirent *dirEntry;
    DIR *dir;
    struct stat fileStatus;

    dir = opendir(path);
    if (!dir)
    {
        fprintf(stderr, "[CBuilder FS Error] %s\n", strerror(errno));
        return (CLib_String){NULL, 0, 0}; // return nothing
    }

    CLib_String output = CLib_String_init("");

    while ((dirEntry = readdir(dir)) != NULL)
    {
        // ignore . and .. entries
        if ((dirEntry->d_name[0] == '.') && (dirEntry->d_name[1] == '\0' || dirEntry->d_name[1] == '.'))
        {
            continue;
        }

        char *dotPos = strchr(mask, '.') + 1;
        if (!dotPos)
        {
            dotPos = (char *)mask + strlen(mask);
        }

        char *fileExt = strrchr(dirEntry->d_name, '.') + 1;
        int fileNameLen = fileExt - dirEntry->d_name - 1;
        if (fileExt == NULL + 1)
        {
            int dirLen = strlen(dirEntry->d_name);
            fileExt = dirEntry->d_name + dirLen;
            fileNameLen = dirLen;
        }
        
        char *fileName;
        if (fileExt == dirEntry->d_name + 1)
        {
            fileName = "";
        }
        else
        {
            fileName = (char *)alloca(fileNameLen + 1);
            memcpy(fileName, dirEntry->d_name, fileNameLen);
            fileName[fileNameLen] = '\0';
        }
        
        int maskLen = strlen(mask);
        if (mask[0] != '*' && strncmp(mask, fileName, fileNameLen - 1))
        {
            continue;
        }

        if (dotPos[0] != '*' && strcmp(dotPos, fileExt))
        {
            continue;
        }

        stat(dirEntry->d_name, &fileStatus);
        if ((mode & CLIB_FS_DIRMODE_FOLDERS) && (fileStatus.st_mode & S_IFDIR))
        {
            CLib_String_concatCStr(&output, dirEntry->d_name);
        }
        else if ((mode & CLIB_FS_DIRMODE_FILES) && !(fileStatus.st_mode & S_IFDIR))
        {
            CLib_String_concatCStr(&output, dirEntry->d_name);
        }

        CLib_String_concatCStr(&output, (char *)delim);
    }

    closedir(dir); // cleanup

    return output;
}

#else // unsupported system
#error "[CLib FS] Unsupported system, if you know about your system, feel free to define _WIN32 for windows __linux__ or __APPLE__ or __MACH__ for linux and mac systems"
#endif

#endif // INCLUDED_CLIB_FS