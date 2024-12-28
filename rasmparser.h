#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "rasmlexer.h"

typedef struct ParseList {
    Token value;
    struct ParseList *next;
} ParseList;


void print_list(ParseList *head);

ParseList parser(Lexer lexer);

#endif