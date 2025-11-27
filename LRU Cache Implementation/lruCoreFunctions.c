#include "lru.h"

HashNode** initialiseHashMap(int hashSize) {
    HashNode** map = malloc(hashSize * sizeof(HashNode*));
    if (!map)
    {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }

    for (int index = 0; index < hashSize; index++)
    {
        map[index] = NULL;
    }
    
    return map;
}

Queue* initialiseQueue() {
    Queue* newQueue = malloc(sizeof(Queue));
    if (!newQueue)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    newQueue->front = newQueue->rear = NULL;
    newQueue->size = 0;

    return newQueue;
}

LRUCache* initialiseCache(int capacity) {
    LRUCache* cache = malloc(sizeof(LRUCache));
    if (!cache)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }
    
    cache->capacity = capacity;
    cache->hashSize = findNextPrime(capacity);

    cache->hashMap = initialiseHashMap(cache->hashSize);
    cache->queue = initialiseQueue();

    return cache;
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

bool updateIfKeyExists(LRUCache* cache, int key, char* value) {
    QueueNode* node = findNode(cache->hashMap, key, cache->hashSize);

    if (node) {
        strcpy(node->value, value);
        moveToFront(cache->queue, node);
        return true;
    }

    return false;
}

void deleteLRU(LRUCache* cache) {
    if (!cache || !cache->queue || !cache->queue->rear)
    {
        return;
    }

    int key = cache->queue->rear->key;

    deleteFromHashMap(cache->hashMap, key, cache->hashSize);

    deleteFromQueue(cache->queue);
}

void insertNewKey(LRUCache* cache, int key, char* value) {
    QueueNode* newQueueNode = createQueueNode(key, value);
    
    insertAtFront(cache->queue, newQueueNode);
    insertIntoHashMap(cache->hashMap, newQueueNode, cache->hashSize);
}

void handlePut(LRUCache* cache, int key, char* value) {
    if (updateIfKeyExists(cache, key, value))
    {
        return;
    }

    if (cache->queue->size == cache->capacity)
    {
        deleteLRU(cache);
    }

    insertNewKey(cache, key, value);
}

char* handleGet(LRUCache* cache, int key) {
    QueueNode* node = findNode(cache->hashMap, key, cache->hashSize);

    if (node) {
        moveToFront(cache->queue, node);
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