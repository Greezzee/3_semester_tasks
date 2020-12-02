#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int GetSharedMemory(void** array, int* shmid, char* pathname, size_t size) {
    int new = 1;
    key_t key;
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }
    if ((*shmid = shmget(key, size, 0666 | IPC_EXCL | IPC_CREAT)) < 0) {
        if (errno != EEXIST) {
            printf("Can't create shared memory\n");
            exit(-1);
        }
        else {
            if ((*shmid = shmget(key, size, 0)) < 0) {
                printf("Can't find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }

    if ((*array = shmat(*shmid, NULL, 0)) == (void*)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    return new;
}

int main() {
    char *array;
    size_t *size;
    int shmid, size_shmid;
    int new = 1;
    char pathname[] = "quine_writer.c";
    char pathname_size[] = "quine_reader.c";

    int fd = open(pathname, O_RDONLY);
    size_t filesize = lseek(fd, 0, SEEK_END) + 1;
    lseek(fd, 0, SEEK_SET);

    GetSharedMemory((void*)&size, &size_shmid, pathname_size, sizeof(size_t));
    GetSharedMemory((void*)&array, &shmid, pathname, filesize);

    memset(array, 0, filesize);
    int a = read(fd, array, filesize);

    *size = filesize;

    if (shmdt(array) < 0) {
        printf("Can't detach shared memory");
        exit(-1);
    }

    if (shmdt(size) < 0) {
        printf("Can't detach shared memory");
        exit(-1);
    }

    close(fd);

    return 0;
}