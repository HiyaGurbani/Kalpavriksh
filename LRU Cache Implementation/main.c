#include "lru.h"

int main() {
    HashNode** hashMap = NULL;
    Queue* queue = NULL;

    displayMenuInstructions();

    while(true)
    {
        handleCommand(&hashMap, &queue);
    }
}