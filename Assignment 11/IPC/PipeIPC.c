#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int* readArray(int *size) {
    printf("Enter the size of array: ");
    scanf("%d", size);

    int* array = malloc((*size) * sizeof(int));
    if (!array) 
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    printf("Enter elements of array: ");
    for (int index = 0; index < *size; index++)
    {
        scanf("%d", &array[index]);
    }

    return array;
}

void display(int* array, int size) {
    for (int index = 0; index < size; index++)
    {
        printf("%d ", array[index]);
    }
    printf("\n");
}


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
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
    display(array, size);

    close(parent2child[1]);
    close(child2parent[0]);
}

int main() {
    int size;
    int* array = readArray(&size);
    
    printf("Array Before Sorting: \n");
    display(array, size);

    int parent2child[2], child2parent[2];
    if (pipe(parent2child) == -1 || pipe(child2parent) == -1)
    {
        perror("Pipe Failed!");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed!\n");
        free(array);
        exit(1);
    }

    else if (pid == 0)
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