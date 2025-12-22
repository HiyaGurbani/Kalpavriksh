#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "IPC_HELPER.h"

#define SHM_KEY 1234
#define SIZE 100

typedef struct SharedData {
    int size;
    int array[SIZE];
} SharedData;

void readDataArray(SharedData *data) {
    printf("Enter the size of array: ");
    getValidSize(&data->size);

    printf("Enter elements of array: ");
    for (int index = 0; index < data->size; index++)
    {
        getValidInteger(&data->array[index]);
    }
}

void childProcess(SharedData *data) {
    quickSort(data->array, 0, data->size - 1);
    exit(0);
}

void parentProcess(SharedData *data) {
    printf("Array Before Sorting: \n");
    displayArray(data->array, data->size);

    wait(NULL);

    printf("Array After Sorting: \n");
    displayArray(data->array, data->size);
}

int main() {
    int shmId = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmId < 0)
    {
        perror("shmget Failed!");
        exit(1);
    }

    SharedData *data = shmat(shmId, NULL, 0);
    if (data == (void*)-1)
    {
        perror("shmat Failed!");
        exit(1);
    }

    readDataArray(data);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed!\n");
        exit(1);
    }

    else if (pid == 0)
    {
        childProcess(data);
    }

    else
    {
        parentProcess(data);
    }

    shmdt(data);
    shmctl(shmId, IPC_RMID, NULL);

    return 0;
}