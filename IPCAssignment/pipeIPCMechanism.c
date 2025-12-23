#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "IPC_HELPER.h"

void sendData(int fileDescriptor, int *array, int size) {
    if (write(fileDescriptor, &size, sizeof(int)) != sizeof(int))
    {
        perror("write failed (size)");
        exit(1);
    }

    if (write(fileDescriptor, array, size * sizeof(int)) != size * sizeof(int))
    {
        perror("write failed (array)");
        exit(1);
    }
}

void receiveData(int fileDescriptor, int *array, int *size) {
    if (read(fileDescriptor, size, sizeof(int)) != sizeof(int))
    {
        perror("read failed (size)");
        exit(1);
    }

    if (read(fileDescriptor, array, (*size) * sizeof(int)) != (*size) * sizeof(int))
    {
        perror("read failed (array)");
        exit(1);
    }
}

void childProcess(int *array, int parent2child[2], int child2parent[2]) {
    int size;

    close(parent2child[1]);
    close(child2parent[0]);

    receiveData(parent2child[0], array, &size);
    quickSort(array, 0, size - 1);
    sendData(child2parent[1], array, size);

    close(parent2child[0]);
    close(child2parent[1]);
    free(array);
    exit(0);
}

void parentProcess(int *array, int size, int parent2child[2], int child2parent[2]) {
    close(parent2child[0]);
    close(child2parent[1]);
    
    sendData(parent2child[1], array, size);
    wait(NULL);
    receiveData(child2parent[0], array, &size);

    printf("Array After Sorting: \n");
    displayArray(array, size);

    close(parent2child[1]);
    close(child2parent[0]);
}

int main() {
    int size;
    int* array = readArray(&size);
    
    printf("Array Before Sorting: \n");
    displayArray(array, size);

    int parent2child[2], child2parent[2];
    if (pipe(parent2child) == -1 || pipe(child2parent) == -1)
    {
        perror("Pipe Failed!");
        exit(1);
    }

    pid_t processId = fork();

    if (processId < 0)
    {
        perror("Fork Failed!\n");
        free(array);
        exit(1);
    }

    else if (processId == 0)
    {
        childProcess(array, parent2child, child2parent);
    }

    else
    {
        parentProcess(array, size, parent2child, child2parent);
    }

    free(array);
    return 0;
}