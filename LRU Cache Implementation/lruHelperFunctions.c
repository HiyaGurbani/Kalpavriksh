#include "lru.h"

int getDigit (char** input) {
    int digit = 0;
    char *currInput = *input;
    
    while(isdigit((unsigned char)*currInput))
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

    if (*input != '\0')
    {
        return -1;
    }

    if (capacity < MIN_CACHE_CAPACITY || capacity > MAX_CACHE_CAPACITY )
    {
        return 0;
    }

    return capacity;
}

bool getKey(char* input, int *key, bool checkEnd) {
    if (*input == '\0' || !isdigit((unsigned char) *input))
    {
        return false;
    }

    *key = getDigit(&input);

    if (!checkEnd) {
        return true;
    }

    while (*input == ' ')
    {
        input++;
    }
    return (*input == '\0');
}

bool getKeyValue(char* input, int *key, char* value) {
    if (!getKey(input, key, false)) 
    {
        return false;
    }

    while (isdigit((unsigned char)*input)) 
    {
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

    strcpy(value, input);

    return true;
}

bool isPrime(int value) {
    if (value <= 1)
    {
        return false;
    }

    if (value == 2) 
    {
        return true;
    }

    if (value % 2 == 0)
    {
        return false;
    }

    int sqrtVal = (int)sqrt(value);
    for (int index = 3; index <= sqrtVal; index += 2)
    {
        if (value % index == 0)
        {
            return false;
        }
    }

    return true;
}

int findNextPrime(int value) {
    if (value <= SMALLEST_PRIME_NUMBER) 
    {
        return SMALLEST_PRIME_NUMBER;
    }

    if (value % SMALLEST_PRIME_NUMBER == 0) 
    {
        value++;
    }

    while (!isPrime(value))
    {
        value += 2;
    }

    return value;
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

void insertIntoHashMap(HashNode** hashMap, HashNode* node, int hashSize) {
    int index = hash(node->key, hashSize);
    node->next = hashMap[index];
    hashMap[index] = node;
}

int hash(int key, int hashSize) {
    int index = key % hashSize;
    if (index < 0)
    {
        return (index + hashSize);
    } 
    else
    {
        return index;
    }
}


void removeNodeFromQueue(Queue* queue, QueueNode* node) {
    if (!queue || !node)
    {
        return;
    }

    if (node->prev) 
    {
        node->prev->next = node->next;
    }
    else
    {
        queue->front = node->next;
    }

    if (node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        queue->rear = node->prev;
    }

    node->prev = node->next = NULL;
    queue->size--;
}

void deleteFromHashMap(HashNode** hashMap, int key, int hashSize) {
    int index = hash(key, hashSize);
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
            curr = NULL;
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
    toDelete = NULL;
    queue->size--;
}
