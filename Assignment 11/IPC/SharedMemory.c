#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY 1234
#define SIZE 100

typedef struct SharedData {
    int size;
    int array[SIZE];
} SharedData;

void readArray(SharedData *data) {
    printf("Enter the size of array: ");
    scanf("%d", &data->size);

    printf("Enter elements of array: ");
    for (int index = 0; index < data->size; index++)
    {
        scanf("%d", &data->array[index]);
    }
}

void display(SharedData *data) {
    for (int index = 0; index < data->size; index++)
    {
        printf("%d ", data->array[index]);
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

void childProcess(SharedData *data) {
    quickSort(data->array, 0, data->size - 1);
    exit(0);
}

void parentProcess(SharedData *data) {
    printf("Array Before Sorting: \n");
    display(data);

    wait(NULL);

    printf("Array After Sorting: \n");
    display(data);
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

    readArray(data);

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