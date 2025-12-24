#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "IPC_HELPER.h"

#define FILENAME "data.txt"

void writeToFile(int* array, int size) {
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        perror("Error in Opening File!\n");
        exit(1);
    }

    fprintf(file, "%d\n", size);
    for (int index = 0; index < size; index++)
    {
        fprintf(file, "%d ", array[index]);
    }

    fclose(file);
}

void readFromFile(int* array, int* size) {
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        perror("Error in Opening File!\n");
        exit(1);
    }

    fscanf(file, "%d", size);
    for (int index = 0; index < *size; index++)
    {
        fscanf(file, "%d", &array[index]);
    }

    fclose(file);
}

void childProcess(int* array, int size) {
    readFromFile(array, &size);
    quickSort(array, 0, size - 1);
    writeToFile(array, size);
    exit(0);
}

void parentProcess(int* array, int size) {
    wait(NULL);
    readFromFile(array, &size);
    printf("Array After Sorting: \n");
    displayArray(array, size);
}

int main() {
    int size;
    int* array = readArray(&size);

    printf("Array Before Sorting: \n");
    displayArray(array, size);

    writeToFile(array, size);

    pid_t processId = fork();

    if (processId < 0)
    {
        perror("Fork Failed!\n");
        free(array);
        exit(1);
    }

    else if (processId == 0)
    {
        childProcess(array, size);
    }

    else
    {
        parentProcess(array, size);
    }

    free(array);
    return 0;
}