#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "IPC_HELPER.h"

#define MESSAGE_KEY 1234
#define SIZE 50
#define PARENT_TO_CHILD 1
#define CHILD_TO_PARENT 2

typedef struct Message {
    long messageType;
    int size;
    int array[SIZE];
} Message;

void sendMessage(int messageId, long messageType, int *array, int size) {
    Message message;
    message.messageType = messageType;
    message.size = size;

    for (int index = 0; index < size; index++)
    {
        message.array[index] = array[index];
    }

    if (msgsnd(messageId, &message, sizeof(message) - sizeof(long), 0) == -1)
    {
        perror("msgsnd failed");
        exit(1);
    }
}

void receiveMessage(int messageId, long type, int *array, int *size) {
    Message message;
    if (msgrcv(messageId, &message, sizeof(message) - sizeof(long), type, 0) == -1)
    {
        perror("msgrcv failed");
        exit(1);
    }

    *size = message.size;
    for (int index = 0; index < *size; index++)
    {
        array[index] = message.array[index];
    }
}

void childProcess(int messageId, int* array, int* size) {
    receiveMessage(messageId, PARENT_TO_CHILD, array, size);
    quickSort(array, 0, *size - 1);
    sendMessage(messageId, CHILD_TO_PARENT, array, *size);
    free(array);
    exit(0);
}

void parentProcess(int messageId, int* array, int* size) {
    sendMessage(messageId, PARENT_TO_CHILD, array, *size);
    wait(NULL);
    receiveMessage(messageId, CHILD_TO_PARENT, array, size);

    printf("Array After Sorting: \n");
    displayArray(array, *size);

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
    displayArray(array, size);

    pid_t processId = fork();

    if (processId < 0)
    {
        perror("Fork Failed!\n");
        free(array);
        exit(1);
    }

    else if (processId == 0)
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