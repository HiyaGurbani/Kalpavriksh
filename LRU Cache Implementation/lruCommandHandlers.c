#include "lru.h"

void displayMenuInstructions() {
    printf("Welcome to LRU Cache!\n");
    printf("Available commands:\n");
    printf("1. createCache <capacity>   : Initialize cache with capacity (1-%d)\n", MAX_CACHE_CAPACITY);
    printf("2. put <key> <value>        : Insert or update a key-value pair\n");
    printf("3. get <key>                : Retrieve value for the key\n");
    printf("4. exit                     : Exit the program\n");
}

void processCreateCacheCommand(HashNode*** hashMap, Queue** queue, char* input) {
    int capacity = getCapacity(input + strlen(CMD_CREATE_CACHE));
    if (capacity == 0)
    {
        printf("Cache capacity must be between [%d-%d].\n", MIN_CACHE_CAPACITY, MAX_CACHE_CAPACITY);
        return;
    }

    if (capacity == -1)
    {
        printf("Invalid Command Syntax.\n");
        return;
    }

    if (*hashMap != NULL || *queue != NULL)
    {
        freeCache(*hashMap, *queue);
        *hashMap = NULL;
        *queue = NULL;
    }
    
    initialiseCache(hashMap, queue, capacity);
    printf("Cache is now initialised.\n");
}

void processPutCommand(HashNode** hashMap, Queue* queue, char* input) {
    if (!hashMap || !queue)
    {
        printf("Cache not initialised.\n");
        return;
    }

    int key = 0;
    char value[VALUE_SIZE] = "";
    if (!getKeyValue(input + strlen(CMD_PUT), &key, value))
    {
        printf("Invalid Command Syntax.\n");
        return;
    }
    
    handlePut(hashMap, queue, key, value);
    printf("Key %d inserted/updated successfully.\n", key);
}

void processGetCommand(HashNode** hashMap, Queue* queue, char* input) {
    if (!hashMap || !queue)
    {
        printf("Cache not initialised.\n");
        return;
    }

    int key = 0;
    if (!getKey(input + strlen(CMD_GET), &key, true))
    {
        printf("Invalid Command Syntax.\n");
        return;
    }

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

void freeCache(HashNode** hashMap, Queue* queue) {
    if (!queue || !hashMap)
    {
        return;
    }

    freeHashMap(hashMap, queue->hashSize);
    freeQueue(queue);  
}

void handleCommand(HashNode*** hashMap, Queue** queue) {
    char input[INPUT_SIZE];
    printf("\nEnter the command: ");
    fgets(input, INPUT_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strncmp(input, CMD_CREATE_CACHE, strlen(CMD_CREATE_CACHE)) == 0)
    {
        processCreateCacheCommand(hashMap, queue, input);
    }

    else if (strncmp(input, CMD_PUT, strlen(CMD_PUT)) == 0)
    {
        processPutCommand(*hashMap, *queue, input);
    }

    else if (strncmp(input, CMD_GET, strlen(CMD_GET)) == 0)
    {
        processGetCommand(*hashMap, *queue, input);
    }

    else if (strcmp(input, CMD_EXIT) == 0)
    {
        freeCache(*hashMap, *queue);
        printf("Exiting LRU Cache!\n");
        *hashMap = NULL;
        *queue = NULL;
        exit(0);
    }

    else
    {
        printf("Invalid Command.\n");
    }
}