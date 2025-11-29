#include "lru.h"

void displayMenuInstructions() {
    printf("Welcome to LRU Cache!\n");
    printf("Available commands:\n");
    printf("1. createCache <capacity>   : Initialize cache with capacity (1-%d)\n", MAX_CACHE_CAPACITY);
    printf("2. put <key> <value>        : Insert or update a key-value pair\n");
    printf("3. get <key>                : Retrieve value for the key\n");
    printf("4. exit                     : Exit the program\n");
}

void processCreateCacheCommand(LRUCache** cache, char* input) {
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

    if (*cache)
    {
        freeCache(*cache);
        *cache = NULL;
    }
    
    *cache = initialiseCache(capacity);
    printf("Cache is now initialised.\n");
}

void processPutCommand(LRUCache* cache, char* input) {
    if (!cache) 
    {
        printf("Cache not initialized.\n");
        return;
    }

    int key = 0;
    char value[VALUE_SIZE] = "";
    if (!getKeyValue(input + strlen(CMD_PUT), &key, value))
    {
        printf("Invalid Command Syntax.\n");
        return;
    }
    
    handlePut(cache, key, value);
    printf("Key %d inserted/updated successfully.\n", key);
}

void processGetCommand(LRUCache* cache, char* input) {
    if (!cache) 
    {
        printf("Cache not initialized.\n");
        return;
    }

    int key = 0;
    char* currString = input + strlen(CMD_GET);
    if (!getKey(&currString, &key, true))
    {
        printf("Invalid Command Syntax.\n");
        return;
    }

    char* value = handleGet(cache, key);
    if (value)
    {
        printf("%s\n", value);
    }
    else
    {
        printf("NULL\n");
    }
}

void freeCache(LRUCache* cache) {
    freeHashMap(cache->hashMap, cache->hashSize);
    freeQueue(cache->queue);
    free(cache);
    cache = NULL;  
}

void handleCommand(LRUCache** cache) {
    char input[INPUT_SIZE];
    printf("\nEnter the command: ");
    fgets(input, INPUT_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strncmp(input, CMD_CREATE_CACHE, strlen(CMD_CREATE_CACHE)) == 0)
    {
        processCreateCacheCommand(cache, input);
    }

    else if (strncmp(input, CMD_PUT, strlen(CMD_PUT)) == 0)
    {
        processPutCommand(*cache, input);
    }

    else if (strncmp(input, CMD_GET, strlen(CMD_GET)) == 0)
    {
        processGetCommand(*cache, input);
    }

    else if (strcmp(input, CMD_EXIT) == 0)
    {
        if (*cache) {
            freeCache(*cache);
            *cache = NULL;
        }
        printf("Exiting LRU Cache!\n");
        exit(0);
    }

    else
    {
        printf("Invalid Command.\n");
    }
}