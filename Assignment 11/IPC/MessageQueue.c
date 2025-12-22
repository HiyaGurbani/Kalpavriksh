#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MESSAGE_KEY 1234
#define SIZE 50

typedef struct Message {
    long messageType;
    int size;
    int array[SIZE];
} Message;

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

void sendMessage(int messageId, long messageType, int *array, int size) {
    Message message;
    message.messageType = messageType;
    message.size = size;

    for (int index = 0; index < size; index++)
    {
        message.array[index] = array[index];
    }

    msgsnd(messageId, &message, sizeof(message) - sizeof(long), 0);
}

void receiveMessage(int messageId, long type, int *array, int *size) {
    Message message;
    msgrcv(messageId, &message, sizeof(message) - sizeof(long), type, 0);

    *size = message.size;
    for (int index = 0; index < *size; index++)
    {
        array[index] = message.array[index];
    }
}

void childProcess(int messageId, int* array, int* size) {
    receiveMessage(messageId, 1, array, size);
    quickSort(array, 0, *size - 1);
    sendMessage(messageId, 2, array, *size);
    free(array);
    exit(0);
}

void parentProcess(int messageId, int* array, int* size) {
    sendMessage(messageId, 1, array, *size);
    wait(NULL);
    receiveMessage(messageId, 2, array, size);

    printf("Array After Sorting: \n");
    display(array, *size);

    msgctl(messageId, IPC_RMID, NULL);
}

int main() {
    int size;
    int* array = readArray(&size);

    int messageId = msgget(MESSAGE_KEY, IPC_CREAT | 0666);
    if (messageId < 0)
    {
        perror("msgget failed!");
        exit(1);
    }
    
    printf("Array Before Sorting: \n");
    display(array, size);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork Failed!\n");
        free(array);
        exit(1);
    }

    else if (pid == 0)
    {
        childProcess(messageId, array, &size);
    }

    else
    {
        parentProcess(messageId, array, &size);
    }

    free(array);
    return 0;
}