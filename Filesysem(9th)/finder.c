#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

#define MAX_NAME 10000
#define MAX_COUNT 10000

struct dirent* readdir(DIR* dirp);

int smart_str_cmp(char* a, char* b) {
    size_t i = 0;
    while (a[i] && b[i]) {
        if (a[i] == '@' || b[i] == '@');
        else if (a[i] == '%' && (smart_str_cmp(a + i, b + i + 1) == 0 || smart_str_cmp(a + i + 1, b + i + 1) == 0)) {
            return 0;
        }
        else if (b[i] == '%' && (smart_str_cmp(a + i + 1, b + i) == 0 || smart_str_cmp(a + i + 1, b + i + 1) == 0)) {
            return 0;
        }
        else if (a[i] != b[i])
            return a[i] - b[i];
        i++;
    }
    
    return a[i] - b[i];
}

int Find(unsigned deep, char* directory, char* finding_file, char** out, int* counter)
{
    if (deep <= 0)
        return 0;
    DIR* d = opendir(directory);
    //printf("[%s, %llu]\n", directory, d);
    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == 4) { //TODO: запретить вход в './' ; Искать наименьшего общего предка при проходе в '../' 
            char full_name[MAX_NAME] = {};
            strcat(full_name, directory);
            strcat(full_name, "/");
            strcat(full_name, dir->d_name);
            Find(deep - 1, full_name, finding_file, out, counter);
        }
        else if (dir->d_type == 8) {
            if (smart_str_cmp(dir->d_name, finding_file) == 0) {
                strcat(out[*counter], directory);
                strcat(out[*counter], "/");
                strcat(out[*counter], dir->d_name);
                *counter += 1;
            }
        }
    }
    closedir(d);
    return *counter;
}

int main(int argc, char** argv) {

    if (argc != 4) {
        printf("Welcome to find program\n");
        printf("Please run in like:\n\n./find [where_find] [what_find] [finding_radius]\n\nYou may use '@' "
               "to determine any symbol (like '?') or '%%' to determine any symbol combination (like '*')\n");
        return 0;
    }

    char** out = (char**)calloc(MAX_COUNT, sizeof(char*));
    for (int i = 0; i < MAX_COUNT; i++)
        out[i] = (char*)calloc(MAX_NAME, sizeof(char));

    char** res_out = (char**)calloc(MAX_COUNT, sizeof(char*));
    for (int i = 0; i < MAX_COUNT; i++)
        res_out[i] = (char*)calloc(MAX_NAME, sizeof(char));

    char** res = (char**)calloc(MAX_COUNT, sizeof(char*));
    for (int i = 0; i < MAX_COUNT; i++)
        res[i] = (char*)calloc(MAX_NAME, sizeof(char));
    int res_count = 0;

    int counter = 0;
    if (Find(atoi(argv[3]), argv[1], argv[2], out, &counter)) {
        for (int i = 0; i < counter; i++) {
            realpath(out[i], res_out[i]);
            int flag = 0;
            for (int j = 0; j < res_count; j++) {
                if (strcmp(res_out[i], res[j]) == 0) {
                    flag = 1;
                    break;
                }
            }
            if (!flag) {
                strcat(res[res_count], res_out[i]);
                res_count++;
            }
        }
        printf("%d Files found!\n", res_count);
        for (int i = 0; i < res_count; i++)
            printf("%d - [%s]\n", i + 1, res[i]);
    }
    else {
        printf("File not found\n");
    }
    for (int i = 0; i < MAX_COUNT; i++)
        free(out[i]);
    free(out);
    return 0;
}