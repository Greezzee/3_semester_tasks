#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

struct dirent* readdir(DIR* dirp);

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Incorrect arg\n");
        exit(-1);
    }
    DIR* d = opendir(argv[1]);
    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == 4)
            printf("Type: directory - [%s]\n", dir->d_name);
        else if (dir->d_type == 8) {
            struct stat info;
            char full_name[256] = {};
            strcat(full_name, argv[1]);
            strcat(full_name, "/");
            strcat(full_name, dir->d_name);
            stat(full_name, &info);
            printf("Type: file      - [%s] - %llu bytes\n", dir->d_name, info.st_size);
        }
    }
    closedir(d);
    return 0;
}