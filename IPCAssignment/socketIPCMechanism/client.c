#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define RESPONSE_SIZE 256
#define MIN_CHOICE 1
#define MAX_CHOICE 4

void getValidChoice(int* choice) {
    while (true)
    {
        if (scanf("%d", choice) != 1)
        {
            printf("Invalid Input! Enter again: ");
            continue;
        }

        if (*choice < MIN_CHOICE || *choice > MAX_CHOICE)
        {
            printf("Invalid Choice! Enter again: ");
            continue;
        }

        break;
    }
}

int main() {
    int socket;
    struct sockaddr_in serverAddress;
    int choice;
    double amount;
    char response[RESPONSE_SIZE];

    socket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
    {
        perror("Socket creation failed!");
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Connection failed");
        exit(1);
    }

    printf("\n------------ATM MENU--------------\n");
    printf("1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
    printf("Enter choice: ");
    getValidChoice(&choice);

    if (choice == 4)
    {
        close(socket);
        return 0;
    }

    send(socket, &choice, sizeof(choice), 0);

    if (choice == 1 || choice == 2)
    {
        printf("Enter amount: ");
        scanf("%lf", &amount);
        send(socket, &amount, sizeof(amount), 0);
    }

    recv(socket, response, sizeof(response), 0);
    printf("\nServer Response: %s\n", response);

    close(socket);
    return 0;
}