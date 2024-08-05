/*
    Adds file system support to CLib library
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

#ifndef INCLUDED_CLIB_FS
#define INCLUDED_CLIB_FS

/**
 * @file clib_fs.h
 * @author Rouvik Maji (majirouvik@gmail.com)
 * @brief Adds file system support to CLib library
 * @date 2024-08-05
 * 
 * @copyright Copyright (c) 2024 GNU GPL <https://www.gnu.org/licenses/>
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "clib_string.h"

#define CLIB_FS_DIRMODE_FILES (1)
#define CLIB_FS_DIRMODE_FOLDERS (1 << 1)

// definitions
/**
 * @brief Opens a directory and prints its list of files and folders seperated by delim to a CLib_String and returns it
 * @note  The returned CLib_String must be freed with CLib_String_deinit()
 * 
 * @param path The path to the directory to open
 * @param mask The file mask to filter through while writing, Examples: *.*(all files and folders), *.txt(all text), main.*(all files with name main like main.c, main.exe, main.s ...)
 * @param mode The mode to use when looking for files, possible modes are CLIB_FS_DIRMODE_FILES and CLIB_FS_DIRMODE_FOLDERS, can be OR ed to get both files and folders
 * @param delim The delimeter to append between file and folder names when writing
 * @return CLib_String The CLib_String containing list of files and folders seperated by const char *delim
 */
CLib_String CLib_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim);

/**
 * @brief Reads a file into a CLib_String *str
 * 
 * @param path The path of file to read from
 * @param str The CLib_String to read to, must be initialised with CLib_String_init()
 * @return true If the call FAILS and opening or reading file didnot suceed, the CLib_String is usually in a corrupted state in this situation and must be freed with CLib_String_deinit()
 * @return false If the call suceeds
 */
bool CLib_Fs_readFile(const char *path, CLib_String *str);

/**
 * @brief Synchronously writes a CLib_String to a file specified by const char *path
 * 
 * @param path The path of the file including file name to write to
 * @param contents The CLib_String containing the contents to write from
 * @param append If set to true, the contents are appended to file else content is overwritten on file
 * @return true If the call fails and error is printed to stderr
 * @return false If call suceeds and file is written
 */
bool CLib_Fs_writeFile(const char *path, CLib_String *contents, bool append);


/**
 * @brief Synchronously writes a const char* to a file specified by const char *path
 * 
 * @param path The path of the file including file name to write to
 * @param contents The CLib_String containing the contents to write from
 * @param length The length of the string contents to write, <b>If set to 0 then length is determined automatically</b>
 * @param append If set to true, the contents are appended to file else content is overwritten on file
 * @return true If the call fails and error is printed to stderr
 * @return false If call suceeds and file is written
 */
bool CLib_Fs_writeFileCStr(const char *path, const char *contents, int length, bool append);

#if defined(_WIN32) // systems with win api

#define PRINT_LAST_ERROR                                                                                                                                                                          \
    DWORD err = GetLastError();                                                                                                                                                                   \
    LPSTR buffer = NULL;                                                                                                                                                                          \
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) & buffer, 0, NULL); \
    fprintf(stderr, "Windows Error Msg: %s\n", buffer);                                                                                                                                           \
    LocalFree(buffer);

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
        PRINT_LAST_ERROR
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

bool CLib_Fs_readFile(const char *path, CLib_String *str)
{
    char chunk[CLIB_BUF_CHUNK];
    HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        PRINT_LAST_ERROR
        return true;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        PRINT_LAST_ERROR
        CloseHandle(hFile);
        return true;
    }

    lldiv_t divValue = lldiv(size.QuadPart, CLIB_BUF_CHUNK);

    for (int i = 0; i < divValue.quot; i++)
    {
        if (!ReadFile(hFile, chunk, CLIB_BUF_CHUNK, NULL, NULL))
        {
            PRINT_LAST_ERROR
            CloseHandle(hFile);
            return true;
        }

        CLib_String_concatCStr(str, chunk);
    }

    if (divValue.rem)
    {
        char remainder[divValue.rem];
        if (!ReadFile(hFile, remainder, divValue.rem, NULL, NULL))
        {
            PRINT_LAST_ERROR
            CloseHandle(hFile);
            return true;
        }

        CLib_String_concatCStr(str, remainder);
    }

    CloseHandle(hFile);

    return false; // success
}

bool CLib_Fs_writeFile(const char *path, CLib_String *contents, bool append)
{
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, (append ? OPEN_ALWAYS : CREATE_ALWAYS), FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        PRINT_LAST_ERROR
        return true;
    }

    if (append)
    {
        SetFilePointer(hFile, 0l, NULL, FILE_END);
    }

    DWORD bytesWritten;
    WriteFile(hFile, contents->str, contents->len, &bytesWritten, NULL);
    if (bytesWritten != contents->len)
    {
        PRINT_LAST_ERROR
        CloseHandle(hFile);
        return true;
    }

    CloseHandle(hFile);
    return false;
}

bool CLib_Fs_writeFileCStr(const char *path, const char *contents, int length, bool append)
{
    if (!length)
    {
        length = strlen(contents);
    }

    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, (append ? OPEN_ALWAYS : CREATE_ALWAYS), FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        PRINT_LAST_ERROR
        return true;
    }

    if (append)
    {
        SetFilePointer(hFile, 0l, NULL, FILE_END);
    }

    DWORD bytesWritten;
    WriteFile(hFile, contents, length, &bytesWritten, NULL);
    if (bytesWritten != length)
    {
        PRINT_LAST_ERROR
        CloseHandle(hFile);
        return true;
    }

    CloseHandle(hFile);
    return false;
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
        fprintf(stderr, "[CLib FS Error] Failed to open directory: %s\n", strerror(errno));
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

bool CLib_Fs_readFile(const char *path, CLib_String *str)
{
    char chunk[CLIB_BUF_CHUNK];
    FILE *file = fopen(path, "rb");

    if (!file)
    {
        fprintf(stderr, "[CLib FS Error] Failed to open file: %s\n", strerror(errno));
        return true; // error
    }

    fseek(file, 0L, SEEK_END);
    long long length = ftell(file);
    rewind(file);

    div_t divValue = div(length, CLIB_BUF_CHUNK);

    for (int i = 0; i < divValue.quot; i++)
    {
        fread(chunk, CLIB_BUF_CHUNK, 1, file);
        CLib_String_concatCStr(str, chunk);
    }

    if (divValue.rem)
    {
        char residual[divValue.rem];
        fread(residual, divValue.rem, 1, file);
        CLib_String_concatCStr(str, residual);
    }

    fclose(file);

    return false; // success
}

bool CLib_Fs_writeFile(const char *path, CLib_String *contents, bool append)
{
    FILE *file = NULL;
    if (append)
    {
        file = fopen(path, "a");
    }
    else
    {
        file = fopen(path, "w");
    }

    if (!file)
    {
        fprintf(stderr, "[CLib FS Error] Failed to open file: %s\n", strerror(errno));
        return true;
    }

    int count = fwrite(contents->str, contents->len, 1, file);
    if (!count)
    {
        fclose(file);
        fprintf(stderr, "[CLib FS Error] Failed to write entire contents of string: %s\n", strerror(errno));
        return false;
    }

    fclose(file);
    return true;
}

bool CLib_Fs_writeFileCStr(const char *path, const char *contents, int length, bool append)
{
    if (!length)
    {
        length = strlen(contents);
    }

    FILE *file = NULL;
    if (append)
    {
        file = fopen(path, "ab");
    }
    else
    {
        file = fopen(path, "wb");
    }

    if (!file)
    {
        fprintf(stderr, "[CLib FS Error] Failed to open file: %s\n", strerror(errno));
        return true;
    }

    int count = fwrite(contents, length, 1, file);
    if (!count)
    {
        fclose(file);
        fprintf(stderr, "[CLib FS Error] Failed to write entire contents of string: %s\n", strerror(errno));
        return false;
    }

    fclose(file);
    return true;
}

#else // unsupported system
#error "[CLib FS] Unsupported system, if you know about your system, feel free to define _WIN32 for windows __linux__ or __APPLE__ or __MACH__ for linux and mac systems"
#endif

#endif // INCLUDED_CLIB_FS