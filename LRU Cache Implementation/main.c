#include "lru.h"

int main() {
    LRUCache* cache = NULL;

    displayMenuInstructions();

    while(true)
    {
        handleCommand(&cache);
    }
}