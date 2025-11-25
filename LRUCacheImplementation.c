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
#define CMD_GET "get "
#define CMD_EXIT "exit"

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

int getDigit (char** input) {
    int digit = 0;
    char *currInput = *input;
    
    while(*currInput >= '0' && *currInput <= '9')
    {
        int newDigit = *currInput - '0';
        digit = digit * 10 + newDigit;
        currInput++;
    }

    *input = currInput;
    return digit;
}

int getCapacity(char *input) {
    int capacity = getDigit(&input);

    if (capacity < MIN_CACHE_CAPACITY || capacity > MAX_CACHE_CAPACITY)
    {
        return 0;
    }

    return capacity;
}

bool getKeyValue(char* input, int *key, char* value) {
    *key = getDigit(&input);

    if (*key == 0)
    {
        return false;
    }

    while (*input == ' ')
    {
        input++;
    }

    if (*input == '\0')
    {
        return false;
    }

    strcpy(value, input);

    return true;
}

int getKey(char* input) {
    return getDigit(&input);
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
    int index = key % HASH_SIZE;
    if (index < 0)
    {
        return (index + HASH_SIZE);
    } 
    else
    {
        return index;
    }
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

QueueNode* findAndMoveToFront(HashNode** hashMap, Queue* queue, int key) {
    int index = hash(key);
    HashNode* temp = hashMap[index];
    while(temp) 
    {
        if (temp->key == key)
        {
            detachNode(queue, temp->queueNode);
            insertAtFront(queue, temp->queueNode);
            return temp->queueNode;
        }
        temp = temp->next;
    }
        
    return NULL;
}

bool updateIfKeyExists(HashNode** hashMap, Queue* queue, int key, char* value) {
    QueueNode* targetNode = findAndMoveToFront(hashMap, queue, key);
    if (targetNode) 
    {
        strcpy(targetNode->value, value);
        return true;
    }
        
    return false;
}

void deleteFromHashMap(HashNode** hashMap, int key) {
    int index = hash(key);
    HashNode* curr = hashMap[index];
    HashNode* prev = NULL;

    while (curr)
    {
        if (curr->key == key)
        {
            if (prev == NULL)
            {
                hashMap[index] = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}

void deleteFromQueue(Queue* queue) {
    if (!queue || !queue->rear)
    {
        return;
    }

    QueueNode* toDelete = queue->rear;
    if (queue->front == queue->rear)
    {
        queue->front = queue->rear = NULL;
    }
    else
    {
        queue->rear = toDelete->prev;
        if (queue->rear)
        {
            queue->rear->next = NULL;
        }
    }

    free(toDelete);
    queue->size--;
}

void deleteLRU(HashNode** hashMap, Queue* queue) {
    if (!queue || !queue->rear)
    {
        return;
    }

    int key = queue->rear->key;

    deleteFromHashMap(hashMap, key);

    deleteFromQueue(queue);
}

QueueNode* createQueueNode(int key, char* value) {
    QueueNode* newNode = malloc(sizeof(QueueNode));
    if (!newNode)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->prev = newNode->next = NULL;

    return newNode;
}

HashNode* createHashNode(int key, QueueNode* queueNode) {
    HashNode* newHashNode = malloc(sizeof(HashNode));
    if (!newHashNode)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    newHashNode->key = key;
    newHashNode->queueNode = queueNode;
    newHashNode->next = NULL;

    return newHashNode;
}

void insertIntoHashMap(HashNode** hashMap, HashNode* node) {
    int index = hash(node->key);
    node->next = hashMap[index];
    hashMap[index] = node;
}

void insertNewKey(HashNode** hashMap, Queue* queue, int key, char* value) {
    QueueNode* newQueueNode = createQueueNode(key, value);
    HashNode* newHashNode = createHashNode(key, newQueueNode);

    insertAtFront(queue, newQueueNode);
    insertIntoHashMap(hashMap, newHashNode);
}

void handlePut(HashNode** hashMap, Queue* queue, int key, char* value) {
    if (updateIfKeyExists(hashMap, queue, key, value))
    {
        return;
    }

    if (queue->size == queue->capacity)
    {
        deleteLRU(hashMap, queue);
    }

    insertNewKey(hashMap, queue, key, value);
}

char* handleGet(HashNode** hashMap, Queue* queue, int key) {
    QueueNode* targetNode = findAndMoveToFront(hashMap, queue, key);
    if (targetNode)
    {
        return targetNode->value;
    }
    
    return NULL;
}

void freeQueue(Queue* queue) {
    while (queue->front) {
        QueueNode* node = queue->front;
        queue->front = node->next;
        free(node);
    }

    free(queue);
}

void freeHashMap(HashNode** hashMap) {
    for (int index = 0; index < HASH_SIZE; index++)
    {
        HashNode* temp = hashMap[index];
        while (temp)
        {
            HashNode* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(hashMap);
}

int main() {
    HashNode** hashMap = NULL;
    Queue* queue = NULL;

    while(true)
    {
        char input[INPUT_SIZE];
        printf("\nEnter the command: ");
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strncmp(input, CMD_CREATE_CACHE, strlen(CMD_CREATE_CACHE)) == 0)
        {
            int capacity = getCapacity(input + strlen(CMD_CREATE_CACHE));
            if (capacity == 0)
            {
                printf("Invalid Command Syntax.\n");
            }

            initialiseCache(capacity, &hashMap, &queue);
            printf("Cache is now initialised.\n");
        }

        else if (strncmp(input, CMD_PUT, strlen(CMD_PUT)) == 0)
        {
            if (!hashMap || !queue)
            {
                printf("Cache not initialised.\n");
                continue;
            }

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

        else if (strncmp(input, CMD_GET, strlen(CMD_GET)) == 0)
        {
            if (!hashMap || !queue)
            {
                printf("Cache not initialised.\n");
                continue;
            }

            int key = getKey(input + strlen(CMD_GET));
            char* value = handleGet(hashMap, queue, key);
            if (value)
            {
                printf("%s\n", value);
            }
            else
            {
                printf("NULL\n");
            }
        }

        else if (strcmp(input, CMD_EXIT) == 0)
        {
            freeQueue(queue);
            freeHashMap(hashMap);
            return 0;
        }

        else
        {
            printf("Invalid Command.\n");
        }
    }
}