#include "hashmap.h"

unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    return value % TABLE_SIZE;
}

HashMap* createHashMap() {
    HashMap *hashmap = (HashMap *)malloc(sizeof(HashMap));
    if (hashmap == NULL) {
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashmap->table[i] = NULL;
    }
    return hashmap;
}

void insert(HashMap *hashmap, const char *key, int value) {
    unsigned int slot = hash(key);

    Entry *newEntry = (Entry *)malloc(sizeof(Entry));
    newEntry->key = strdup(key);
    newEntry->value = value;
    newEntry->next = hashmap->table[slot];
    hashmap->table[slot] = newEntry;
}

int get(HashMap *hashmap, const char *key) {
    unsigned int slot = hash(key);
    Entry *entry = hashmap->table[slot];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return -1; // Not found
}
