#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN_CACHE_CAPACITY 1
#define MAX_CACHE_CAPACITY 1000
#define HASH_SIZE 100
#define INPUT_SIZE 100
#define VALUE_SIZE 100
#define CMD_CREATE_CACHE "createCache "
#define CMD_PUT "put "

typedef struct QueueNode {
    int key;
    char value[VALUE_SIZE];
    struct QueueNode* prev;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
    int capacity;
} Queue;

typedef struct HashNode {
    int key;
    QueueNode* queueNode;
    struct HashNode* next;
} HashNode;

int getCapacity(char *input) {
    int capacity = 0;
    
    while(*input >= '0' && *input <= '9')
    {
        int newDigit = *input - '0';
        capacity = capacity * 10 + newDigit;
        input++;
    }

    if (capacity < MIN_CACHE_CAPACITY || capacity > MAX_CACHE_CAPACITY)
    {
        return 0;
    }

    return capacity;
}

bool getKeyValue(char* input, int *key, char* value) {
    bool keyExists = false, valueExists = false;

    while(*input >= '0' && *input <= '9')
    {
        keyExists = true;
        int newDigit = *input - '0';
        *key = *key * 10 + newDigit;
        input++;
    }

    while (*input == ' ')
    {
        input++;
    }

    if (*input == '\0')
    {
        return false;
    }

    char *string = value;
    while(*input != ' ' && *input != '\0')
    {
        valueExists = true;
        *string = *input;
        string++;
        input++;
    }

    *string = '\0';

    // printf("Key: %d Value: %s\n", *key, value);
    if (!keyExists || !valueExists)
    {
        return false;
    }

    return true;
}

void initialiseCache(int capacity, HashNode*** hashMap, Queue** queue) {
    HashNode** map = malloc(HASH_SIZE * sizeof(HashNode*));
    for (int index = 0; index < HASH_SIZE; index++)
    {
        map[index] = NULL;
    }
    *hashMap = map;

    Queue* q = malloc(sizeof(Queue));
    if (!q)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    q->front = q->rear = NULL;
    q->size = 0;
    q->capacity = capacity;

    *queue = q;
}

int hash(int key) {
    return key % HASH_SIZE;
}

void detachNode(Queue* queue, QueueNode* node) {
    if (!node)
    {
        return;
    }

    if (node->prev == NULL && node->next == NULL)
    {
        queue->front = queue->rear = NULL;
    }
    else if (node->prev == NULL)
    {
        queue->front = node->next;
        queue->front->prev = NULL;
    }
    else if (node->next == NULL)
    {
        queue->rear = node->prev;
        queue->rear->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
    queue->size--;
}

void insertAtFront(Queue* queue, QueueNode* node) {
    node->prev = NULL;
    node->next = queue->front;

    if (queue->front)
    {
        queue->front->prev = node;
    }
    queue->front = node;

    if (!queue->rear)
    {
        queue->rear = node;
    }

    queue->size++;
}

bool updateIfKeyExists(HashNode** hashMap, Queue* queue, int key, char* value) {
    int index = hash(key);
    HashNode* temp = hashMap[index];
    while(temp) 
    {
        if (temp->key == key)
        {
            strcpy(temp->queueNode->value, value);
            detachNode(queue, temp->queueNode);
            insertAtFront(queue, temp->queueNode);
            return true;
        }
        temp = temp->next;
    }
        
    return false;
}

void handlePut(HashNode** hashMap, Queue* queue, int key, char* value) {
    if (updateIfKeyExists(hashMap, queue, key, value))
    {
        return;
    }
}

int main() {
    HashNode** hashMap;
    Queue* queue;

    while(true)
    {
        char input[INPUT_SIZE];
        printf("\nEnter the command: ");
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        int capacity;

        if (strncmp(input, CMD_CREATE_CACHE, strlen(CMD_CREATE_CACHE)) == 0)
        {
            capacity = getCapacity(input + strlen(CMD_CREATE_CACHE));
            if (capacity == 0)
            {
                printf("Invalid Command Syntax.\n");
            }

            initialiseCache(capacity, &hashMap, &queue);
            printf("Cache is now initialised.\n");
        }

        else if (strncmp(input, CMD_PUT, strlen(CMD_PUT)) == 0)
        {
            int key = 0;
            char value[VALUE_SIZE] = "";
            if (!getKeyValue(input + strlen(CMD_PUT), &key, value))
            {
                printf("Invalid Command Syntax.\n");
            }
            else
            {
                handlePut(hashMap, queue, key, value);
            }
        }

        else
        {
            printf("Invalid Command.\n");
        }
    }
}