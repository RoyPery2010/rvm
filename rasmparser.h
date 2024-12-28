#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "rasmlexer.h"
#include "hashmap.h"

typedef struct ParseList {
    Token value;
    struct ParseList *next;
} ParseList;

void append(ParseList *head, Token value);
void print_list(ParseList *head);
void generate_list(ParseList *root, Lexer *lexer, HashMap *hashmap);
ParseList parser(Lexer lexer);