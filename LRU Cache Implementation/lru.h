#ifndef LRU_H
#define LRU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define MIN_CACHE_CAPACITY 1
#define MAX_CACHE_CAPACITY 1000
#define INPUT_SIZE 100
#define VALUE_SIZE 100
#define SMALLEST_PRIME_NUMBER 2
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
    int hashSize;
} Queue;

typedef struct HashNode {
    int key;
    QueueNode* queueNode;
    struct HashNode* next;
} HashNode;

//Command Handlers
void displayMenuInstructions();
void processCreateCacheCommand(HashNode*** hashMap, Queue** queue, char* input);
void processPutCommand(HashNode** hashMap, Queue* queue, char* input);
void processGetCommand(HashNode** hashMap, Queue* queue, char* input);
void handleCommand(HashNode*** hashMap, Queue** queue);
void freeCache(HashNode** hashMap, Queue* queue);

//Core Cache Functions
void initialiseHashMap(HashNode*** hashMap, int hashSize);
void initialiseQueue(Queue** queue, int capacity, int hashSize);
void initialiseCache(HashNode*** hashMap, Queue** queue, int capacity);

QueueNode* findNode(HashNode** hashMap, int key, int hashSize);
void moveToFront(Queue* queue, QueueNode* node);
bool updateIfKeyExists(HashNode** hashMap, Queue* queue, int key, char* value);
void deleteLRU(HashNode** hashMap, Queue* queue);
void insertNewKey(HashNode** hashMap, Queue* queue, int key, char* value);
void handlePut(HashNode** hashMap, Queue* queue, int key, char* value);
char* handleGet(HashNode** hashMap, Queue* queue, int key);

void freeHashMap(HashNode** hashMap, int hashSize);
void freeQueue(Queue* queue);

//Helper Functions
int getDigit (char** input);
int getCapacity(char *input);
bool getKey(char* input, int *key, bool checkEnd);
bool getKeyValue(char* input, int *key, char* value);

bool isPrime(int value);
int findNextPrime(int value);

QueueNode* createQueueNode(int key, char* value);
HashNode* createHashNode(int key, QueueNode* queueNode);
void insertAtFront(Queue* queue, QueueNode* node);
void insertIntoHashMap(HashNode** hashMap, HashNode* node, int hashSize);
int hash(int key, int hashSize);
void removeNodeFromQueue(Queue* queue, QueueNode* node);
void deleteFromHashMap(HashNode** hashMap, int key, int hashSize);
void deleteFromQueue(Queue* queue);

#endif