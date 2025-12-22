#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define FILE_PATH "resource/accountDB.txt"
#define RESPONSE_SIZE 256

pthread_mutex_t fileMutex;

void* handleClient(void* arg) {
    int clientSock = *(int*)arg;
    free(arg);

    int choice;
    double amount;
    double balance;
    char response[RESPONSE_SIZE];

    if (recv(clientSock, &choice, sizeof(choice), 0) <= 0) 
    {
        close(clientSock);
        return NULL;
    }   


    pthread_mutex_lock(&fileMutex);

    FILE* file = fopen(FILE_PATH, "r+");
    if (!file)
    {
        strcpy(response, "Error opening account file!");
        send(clientSock, response, sizeof(response), 0);
        pthread_mutex_unlock(&fileMutex);
        close(clientSock);
        return NULL;
    }

    fscanf(file, "%lf", &balance);

    switch (choice)
    {
        case 1:
        recv(clientSock, &amount, sizeof(amount), 0);
        if (amount > balance)
        {
            sprintf(response, "Withdrawal failed. Insufficient balance: %.2lf", balance);
        }
        else
        {
            balance -= amount;
            rewind(file);
            fprintf(file, "%.2lf", balance);
            sprintf(response, "Withdrawal successful. New balance: %.2lf", balance);
        }
        break;

        case 2:
        recv(clientSock, &amount, sizeof(amount), 0);
        balance += amount;
        rewind(file);
        fprintf(file, "%.2lf", balance);
        sprintf(response, "Deposit successful. New balance: %.2lf", balance);
        break;

        case 3:
        sprintf(response, "Current balance: %.2lf", balance);
        break;

        default:
        strcpy(response, "Invalid choice");
    }

    fclose(file);
    pthread_mutex_unlock(&fileMutex);

    send(clientSock, response, sizeof(response), 0);
    close(clientSock);
    return NULL;
}

int main() {
    int serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    pthread_mutex_init(&fileMutex, NULL);

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("Socket creation failed!");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("Bind failed");
        exit(1);
    }
    listen(serverSock, 5);

    printf("ATM Server running on port %d...\n", PORT);

    while (true)
    {
        clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrLen);
        int* newSock = malloc(sizeof(int));
        if (!newSock)
        {
            perror("Memory Allocation Failed!");
            exit(1);
        }
        *newSock = clientSock;

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, newSock);
        pthread_detach(tid);
    }

    pthread_mutex_destroy(&fileMutex);
    close(serverSock);

    return 0;
}