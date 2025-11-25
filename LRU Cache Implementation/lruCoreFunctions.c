#include "lru.h"

void initialiseHashMap(HashNode*** hashMap, int hashSize) {
    HashNode** map = malloc(hashSize * sizeof(HashNode*));
    if (!map)
    {
        printf("Memory Allocation Failed!\n");
        return;
    }

    for (int index = 0; index < hashSize; index++)
    {
        map[index] = NULL;
    }
    *hashMap = map;
}

void initialiseQueue(Queue** queue, int capacity, int hashSize) {
    Queue* newQueue = malloc(sizeof(Queue));
    if (!newQueue)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    newQueue->front = newQueue->rear = NULL;
    newQueue->size = 0;
    newQueue->capacity = capacity;
    newQueue->hashSize = hashSize;

    *queue = newQueue;
}

void initialiseCache(HashNode*** hashMap, Queue** queue, int capacity) {
    int hashSize = findNextPrime(capacity);
    initialiseHashMap(hashMap, hashSize);
    initialiseQueue(queue, capacity, hashSize);
}

QueueNode* findNode(HashNode** hashMap, int key, int hashSize) {
    int index = hash(key, hashSize);
    HashNode* temp = hashMap[index];
    while (temp) {
        if (temp->key == key) {
            return temp->queueNode;  
        }
        temp = temp->next;
    }
    return NULL;
}

void moveToFront(Queue* queue, QueueNode* node) {
    if (!node || queue->front == node) 
    {
        return; 
    }

    removeNodeFromQueue(queue, node);
    insertAtFront(queue, node);
}

bool updateIfKeyExists(HashNode** hashMap, Queue* queue, int key, char* value) {
    QueueNode* node = findNode(hashMap, key, queue->hashSize);

    if (node) {
        strcpy(node->value, value);
        moveToFront(queue, node);
        return true;
    }

    return false;
}

void deleteLRU(HashNode** hashMap, Queue* queue) {
    if (!queue || !queue->rear)
    {
        return;
    }

    int key = queue->rear->key;

    deleteFromHashMap(hashMap, key, queue->hashSize);

    deleteFromQueue(queue);
}

void insertNewKey(HashNode** hashMap, Queue* queue, int key, char* value) {
    QueueNode* newQueueNode = createQueueNode(key, value);
    HashNode* newHashNode = createHashNode(key, newQueueNode);

    insertAtFront(queue, newQueueNode);
    insertIntoHashMap(hashMap, newHashNode, queue->hashSize);
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
    QueueNode* node = findNode(hashMap, key, queue->hashSize);

    if (node) {
        moveToFront(queue, node);
        return node->value;
    }

    return NULL;
}

void freeQueue(Queue* queue) {
    while (queue->front) {
        QueueNode* toDelete = queue->front;
        queue->front = toDelete->next;
        free(toDelete);
        toDelete = NULL;
    }

    free(queue);
    queue = NULL;
}

void freeHashMap(HashNode** hashMap, int hashSize) {
    for (int index = 0; index < hashSize; index++)
    {
        HashNode* temp = hashMap[index];
        while (temp)
        {
            HashNode* toDelete = temp;
            temp = temp->next;
            free(toDelete);
            toDelete = NULL;
        }
    }
    free(hashMap);
    hashMap = NULL;
}