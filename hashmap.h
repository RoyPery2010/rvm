#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *table[TABLE_SIZE];
} HashMap;

unsigned int hash(const char *key);

HashMap* createHashMap();
void insert(HashMap *hashmap, const char *key, int value);

int get(HashMap *hashmap, const char *key);