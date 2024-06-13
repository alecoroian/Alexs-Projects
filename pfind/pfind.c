/*******************************************************************************
 * Name        : pfind.c
 * Author      : Alex Coroian
 * Pledge      : I pledge my Honour that I have abided by the Stevens Honour System
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

int check_str(char *);
int list_files(const char *, const char *);

char pstring[10];

int main(int argc, char *argv[])
{
    if (check_str(argv[2]) != 0)
    {
        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", argv[2]);
        return EXIT_FAILURE;
    }

    char realPath[PATH_MAX];
    if (realpath(argv[1], realPath) == NULL)
    {
        fprintf(stderr, "Error: Cannot resolve real path for '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    list_files(realPath, realPath);
    return 0;
}

int check_str(char *str)
{
    if (strlen(str) != 9)
    {
        return -1;
    }

    for (int i = 0; i < 9; ++i)
    {
        if (i % 3 == 0 && str[i] != 'r' && str[i] != '-')
        {
            return -1;
        }
        if (i % 3 == 1 && str[i] != 'w' && str[i] != '-')
        {
            return -1;
        }
        if (i % 3 == 2 && str[i] != 'x' && str[i] != '-')
        {
            return -1;
        }
    }

    strncpy(pstring, str, 9);

    pstring[9] = '\0';

    return 0;
}

int list_files(const char *basePath, const char *fullPath)
{
    DIR *dp;
    struct dirent *dirp;
    struct stat fileStat;
    char newPath[PATH_MAX];

    if ((dp = opendir(basePath)) == NULL)
    {
        fprintf(stderr, "Cannot open directory '%s'.\n", basePath);
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(newPath, sizeof(newPath), "%s/%s", basePath, dirp->d_name);

        if (stat(newPath, &fileStat) < 0)
        {
            continue;
        }

        if (S_ISDIR(fileStat.st_mode))
        {
            list_files(newPath, fullPath);
        }

        else if (S_ISREG(fileStat.st_mode))
        {
            char permStr[10];
            snprintf(permStr, sizeof(permStr), "%c%c%c%c%c%c%c%c%c",
                     (fileStat.st_mode & S_IRUSR) ? 'r' : '-',
                     (fileStat.st_mode & S_IWUSR) ? 'w' : '-',
                     (fileStat.st_mode & S_IXUSR) ? 'x' : '-',
                     (fileStat.st_mode & S_IRGRP) ? 'r' : '-',
                     (fileStat.st_mode & S_IWGRP) ? 'w' : '-',
                     (fileStat.st_mode & S_IXGRP) ? 'x' : '-',
                     (fileStat.st_mode & S_IROTH) ? 'r' : '-',
                     (fileStat.st_mode & S_IWOTH) ? 'w' : '-',
                     (fileStat.st_mode & S_IXOTH) ? 'x' : '-');

            if (strcmp(permStr, pstring) == 0)
            {
                printf("%s\n", newPath);
            }
        }
    }

    closedir(dp);
    return 0;
}