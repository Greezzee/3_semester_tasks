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
            shmctl(*shmid, IPC_RMID, NULL);
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
    size_t* size;
    int shmid, shmid_size;
    char pathname[] = "quine_writer.c";
    char size_pathname[] = "quine_reader.c";

    int new = GetSharedMemory((void*)&size, &shmid_size, size_pathname, sizeof(size_t));
    new |= GetSharedMemory((void*)&array, &shmid, pathname, *size);

    if (new) {
        printf("Shared memory is empty\n");
        shmctl(shmid, IPC_RMID, NULL);
        shmctl(shmid_size, IPC_RMID, NULL);
        exit(0);
    }
    else {
        printf("%s\n", array);
    }
    

    if (shmdt(array) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    if (shmdt(size) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(shmid_size, IPC_RMID, NULL);
    return 0;
}