#include <stdbool.h>
#include "IPC_HELPER.h"

void getValidInteger(int* element) {
    while (scanf("%d", element) != 1)
    {
        printf("Invalid Input! Enter again: ");        
    }
}

void getValidSize(int* size) {
    while (true)
    {
        getValidInteger(size);

        if (*size < MIN_SIZE)
        {
            printf("Size must be greater than zero! Enter again: ");
            continue;
        }

        break;
    }
}


int* readArray(int *size) {
    printf("Enter the size of array: ");
    getValidSize(size);

    int* array = malloc((*size) * sizeof(int));
    if (!array) 
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    printf("Enter elements of array: ");
    for (int index = 0; index < *size; index++)
    {
        getValidInteger(&array[index]);
    }

    return array;
}

void displayArray(const int* array, int size) {
    for (int index = 0; index < size; index++)
    {
        printf("%d ", array[index]);
    }
    printf("\n");
}

void swap(int *value1, int *value2) {
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
            swap(&array[currentIndex], &array[index]);
        }
    }

    swap(&array[index + 1], &array[high]);
    return index + 1;
}

void quickSort(int *array, int low, int high) {
    if (low < high)
    {
        int partitionIndex = partition(array, low, high);
        quickSort(array, low, partitionIndex - 1);
        quickSort(array, partitionIndex + 1, high);
    }
}