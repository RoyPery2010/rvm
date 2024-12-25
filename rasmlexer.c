#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "rasmlexer.h"

char *open_file(char *file_path, int *length) {
    FILE *file = fopen(file_path, "r");
    if(!file) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        exit(1);
    }
    char *current = {0};
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);
    current = malloc(sizeof(char) * *length);
    fread(current, 1, *length, file);
    if (!current) {
        fprintf(stderr, "ERROR: Could not read from file\n");
    }
    fclose(file);
    return current;
}

Token init_token(TokenType type, char *text, int line, int character) {
    Token token = {.type = type, .text = text, .line = line, .character = character};
    return token;
}

TokenType check_builtin_keywords(char *name) {
    //printf("check_builtin_keywords %s\n", name);
    if (strcmp(name, "nop") == 0) {
        return TYPE_NOP;
    } else if (strcmp(name, "push") == 0) {
        return TYPE_PUSH;
    } else if (strcmp(name, "pop") == 0) {
        return TYPE_POP;
    } else if (strcmp(name, "dup") == 0) {
        return TYPE_DUP;
    } else if (strcmp(name, "indup") == 0) {
        return TYPE_INDUP;
    } else if (strcmp(name, "swap") == 0) {
        return TYPE_SWAP;
    } else if (strcmp(name, "inswap") == 0) {
        return TYPE_INSWAP;
    } else if (strcmp(name, "add") == 0) {
        return TYPE_ADD;
    } else if (strcmp(name, "sub") == 0) {
        return TYPE_SUB;
    } else if (strcmp(name, "mul") == 0) {
        return TYPE_MUL;
    } else if (strcmp(name, "div") == 0) {
        return TYPE_DIV;
    } else if (strcmp(name, "cmpe") == 0) {
        return TYPE_CMPE;
    } else if (strcmp(name, "cmpne") == 0) {
        return TYPE_CMPNE;
    } else if (strcmp(name, "cmpg") == 0) {
        return TYPE_CMPG;
    } else if (strcmp(name, "cmpl") == 0) {
        return TYPE_CMPL;
    } else if (strcmp(name, "cmpge") == 0) {
        return TYPE_CMPGE;
    } else if (strcmp(name, "cmple") == 0) {
        return TYPE_CMPLE;
    } else if (strcmp(name, "jmp") == 0) {
        return TYPE_JMP;
    } else if (strcmp(name, "zjmp") == 0) {
        return TYPE_ZJMP;
    } else if (strcmp(name, "nzjmp") == 0) {
        return TYPE_NZJMP;
    } else if (strcmp(name, "print") == 0) {
        return TYPE_PRINT;
    } else if (strcmp(name, "halt") == 0) {
        return TYPE_HALT;
    }
    return TYPE_NONE;
}
void print_token(Token token) {
    switch (token.type) {
        case TYPE_NONE:
            printf("Found NONE\n");
            break;
        case TYPE_NOP:
            printf("Found NOP\n");
            break;
        case TYPE_PUSH:
            printf("Found PUSH\n");
            break;
        case TYPE_POP:
            printf("Found POP\n");
            break;
        case TYPE_DUP:
            printf("Found DUP\n");
            break;
        case TYPE_INDUP:
            printf("Found INDUP\n");
            break;
        case TYPE_SWAP:
            printf("Found SWAP\n");
            break;
        case TYPE_INSWAP:
            printf("Found INSWAP\n");
            break;
        case TYPE_ADD:
            printf("Found ADD\n");
            break;
        case TYPE_SUB:
            printf("Found SUB\n");
            break;
        case TYPE_MUL:
            printf("Found MUL\n");
            break;
        case TYPE_DIV:
            printf("Found DIV\n");
            break;
        case TYPE_CMPE:
            printf("Found CMPE\n");
            break;
        case TYPE_CMPNE:
            printf("Found CMPNE\n");
            break;
        case TYPE_CMPG:
            printf("Found CMPG\n");
            break;
        case TYPE_CMPL:
            printf("Found CMPL\n");
            break;
        case TYPE_CMPGE:
            printf("Found CMPGE\n");
            break;
        case TYPE_CMPLE:
            printf("Found CMPLE\n");
            break;
        case TYPE_JMP:
            printf("Found JMP\n");
            break;
        case TYPE_ZJMP:
            printf("Found ZJMP\n");
            break;
        case TYPE_NZJMP:
            printf("Found NZJMP\n");
            break; 
        case TYPE_PRINT:
            printf("Found PRINT\n");
            break;
        case TYPE_HALT:
            printf("Found HALT\n");
            break;
    }
    //printf("text: %s, line: %d, character: %d\n", token.text, token.line, token.character);
}
Token generate_keyword(char *current, int *current_index, int line, int character) {
    //printf("current_index %d line %d character %d\n", *current_index, line, character);
    char *keyword_name = malloc(sizeof(char) * 16);
    int keyword_length = 0;
    while (isalpha(current[*current_index])) {
        keyword_name[keyword_length] = current[*current_index];
        *current_index += 1;
        keyword_length++;
    }
    keyword_name[keyword_length] = '\0';
    //printf("keyword_name %s\n", keyword_name);
    TokenType type = check_builtin_keywords(keyword_name);
    assert(type != TYPE_NONE && "Custom identifiers are not implemented yet!");
    Token token = init_token(type, keyword_name, line, character);
    return token;
}

int lexer() {
    int length;
    char *current = open_file("test.rasm", &length);
    int current_index = 0;
    int line = 1;
    int character = 1;

    while (current_index < length) {
        if(isalpha(current[current_index])) {
            Token token = generate_keyword(current, &current_index, line, character);
            print_token(token);
        } else if (isdigit(current[current_index])) {
            printf("NUMBERIC\n");
        } else if (current[current_index] == '\n') {
            line++;
            current_index++;
        }
        //
        //character++;
    }
    return 0;
}