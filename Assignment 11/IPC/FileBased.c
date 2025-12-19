#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define FILENAME "data.txt"

void display(int* array, int size) {
    for (int index = 0; index < size; index++)
    {
        printf("%d ", array[index]);
    }
    printf("\n");
}

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

void swap(int* value1, int* value2) {
    int temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}

int partition(int* array, int low, int high) {
    int pivot = array[high];
    int index = low - 1;

    for (int currentIndex = low; currentIndex < high; currentIndex++)
    {
        if (array[currentIndex] < pivot)
        {
            index++;
            swap(&array[index], &array[currentIndex]);
        }
    }

    swap(&array[index + 1], &array[high]);
    return index + 1;
}

void quickSort(int* array, int low, int high) {
    if (low < high)
    {
        int partitionIndex = partition(array, low, high);

        quickSort(array, low, partitionIndex - 1);
        quickSort(array, partitionIndex + 1, high);
    }
}

int main() {
    int size;
    printf("Enter the size of array: ");
    scanf("%d", &size);

    int* array = malloc(size * sizeof(int));
    if (!array) 
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    printf("Enter elements of array: ");
    for (int index = 0; index < size; index++)
    {
        scanf("%d", &array[index]);
    }

    printf("Array Before Sorting: \n");
    display(array, size);

    writeToFile(array, size);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed!\n");
        exit(1);
    }

    else if (pid == 0)
    {
        readFromFile(array, &size);
        quickSort(array, 0, size - 1);
        writeToFile(array, size);
        free(array);
        exit(0);
    }

    else
    {
        wait(NULL);
        readFromFile(array, &size);
        printf("Array After Sorting: \n");
        display(array, size);
        free(array);
    }

    return 0;
}