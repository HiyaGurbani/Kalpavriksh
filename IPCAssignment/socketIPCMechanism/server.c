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
#define BACKLOG 5

pthread_mutex_t fileMutex;

bool handleWithdrawal(int clientSocket, FILE* file, double* balance)
{
    double amount;
    if (recv(clientSocket, &amount, sizeof(amount), 0) <= 0)
    {
        return false;
    }

    if (amount > *balance)
    {
        return false;
    }
    else
    {
        *balance -= amount;
        rewind(file);
        fprintf(file, "%.2lf", *balance);
        return true;
    }
}

bool handleDeposit(int clientSocket, FILE* file, double* balance)
{
    double amount;
    if (recv(clientSocket, &amount, sizeof(amount), 0) <= 0)
    {
        return false;
    }

    *balance += amount;
    rewind(file);
    fprintf(file, "%.2lf", *balance);
    return true;
}

void* handleClient(void* arg) {
    int clientSocket = *(int*)arg;
    free(arg);

    int choice;
    double balance;
    char response[RESPONSE_SIZE];

    if (recv(clientSocket, &choice, sizeof(choice), 0) <= 0) 
    {
        close(clientSocket);
        return NULL;
    }   

    pthread_mutex_lock(&fileMutex);

    FILE* file = fopen(FILE_PATH, "r+");
    if (!file)
    {
        strcpy(response, "Error opening account file!");
        send(clientSocket, response, sizeof(response), 0);
        pthread_mutex_unlock(&fileMutex);
        close(clientSocket);
        return NULL;
    }

    fscanf(file, "%lf", &balance);

    switch (choice)
    {
        case 1:
        if (handleWithdrawal(clientSocket, file, &balance))
        {
            sprintf(response, "Withdrawal successful. New balance: %.2lf", balance);
        }
        else
        {
            sprintf(response, "Withdrawal failed. Insufficient balance: %.2lf", balance);
        }
        break;

        case 2:
        if (handleDeposit(clientSocket, file, &balance))
        {
            sprintf(response, "Deposit successful. New balance: %.2lf", balance);
        }
        else
        {
            strcpy(response, "Deposit failed");
        }
        break;

        case 3:
        sprintf(response, "Current balance: %.2lf", balance);
        break;

        default:
        strcpy(response, "Invalid choice");
    }

    fclose(file);
    pthread_mutex_unlock(&fileMutex);

    send(clientSocket, response, sizeof(response), 0);
    close(clientSocket);
    return NULL;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t addressLength = sizeof(clientAddress);

    pthread_mutex_init(&fileMutex, NULL);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("Socket creation failed!");
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
    {
        perror("Bind failed");
        exit(1);
    }
    if (listen(serverSocket, BACKLOG) < 0)
    {
        perror("Listen failed");
        exit(1);
    }


    printf("ATM Server running on port %d...\n", PORT);

    while (true)
    {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &addressLength);
        int* newSocket = malloc(sizeof(int));
        if (!newSocket)
        {
            perror("Memory Allocation Failed!");
            exit(1);
        }
        *newSocket = clientSocket;

        pthread_t threadId;
        pthread_create(&threadId, NULL, handleClient, newSocket);
        pthread_detach(threadId);
    }

    pthread_mutex_destroy(&fileMutex);
    close(serverSocket);

    return 0;
}