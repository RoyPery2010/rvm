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

void push_token(Lexer *lexer, Token value) {
    if(lexer->stack_size >= MAX_TOKEN_STACK_SIZE) {
        fprintf(stderr, "ERROR: Stack Overflow\n");
        exit(1);
    }
    lexer->stack_size++;
    lexer->token_stack[lexer->stack_size] = value;
}

Token pop_token(Lexer *lexer) {
    if(lexer->stack_size <= 0) {
        fprintf(stderr, "ERROR: Stack Underflow\n");
        exit(1);
    }
    lexer->stack_size--;
    return lexer->token_stack[lexer->stack_size];
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
    } else if (strcmp(name, "mod") == 0) {
        return TYPE_MOD;
    } else if (strcmp(name, "addf") == 0) {
        return TYPE_ADD_F;
    } else if (strcmp(name, "subf") == 0) {
        return TYPE_SUB_F;
    } else if (strcmp(name, "mulf") == 0) {
        return TYPE_MUL_F;
    } else if (strcmp(name, "divf") == 0) {
        return TYPE_DIV_F;
    } else if (strcmp(name, "modf") == 0) {
        return TYPE_MOD_F;
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
    assert(&token != NULL && "ERROR: Token cannot be NULL\n");
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
        case TYPE_MOD:
            printf("Found MOD\n");
            break;
        case TYPE_ADD_F:
            printf("Found TYPE_ADD_F\n");
            break;
        case TYPE_SUB_F:
            printf("Found TYPE_SUB_F\n");
            break;
        case TYPE_MUL_F:
            printf("Found TYPE_MUL_F\n");
            break;
        case TYPE_DIV_F:
            printf("Found TYPE_DIV_F\n");
            break;
        case TYPE_MOD_F:
            printf("Found TYPE_MOD_F\n");
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
        case TYPE_INT:
            printf("Found TYPE_INT\n");
            break;
        case TYPE_FLOAT:
            printf("Found TYPE_FLOAT\n");
            break;
        case TYPE_CHAR:
            printf("Found TYPE_CHAR\n");
            break;
        case TYPE_LABEL_DEF:
            printf("Found TYPE LABEL DEF\n");
            break;
        case TYPE_LABEL:
            printf("Found TYPE LABEL\n");
            break;
        case TYPE_HALT:
            printf("Found HALT\n");
            break;
        default:
            assert(false);
    }
    //printf("text: %s, line: %d, character: %d\n", token.text, token.line, token.character);
}

TokenType check_label_type(char *current, int *current_index) {
    //printf("check_label_type: current_index %d\n", *current_index);
    //printf("CURRENT CHARACTER: ----%c---\n", current[*current_index]);
    if(current[*current_index] == ':') {
        current_index += 1;
        return TYPE_LABEL_DEF;
    }
    return TYPE_LABEL;
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
    if (type == TYPE_NONE) {
        type = check_label_type(current, current_index);
    }
    assert(type != TYPE_NONE && "Custom identifiers are not implemented yet!");
    Token token = init_token(type, keyword_name, line, character);
    print_token(token);
    return token;
}

Token generate_char(char *current, int *current_index, int line, int character) {
    //printf("current_index %d line %d character %d\n", *current_index, line, character);
    char *character_name = malloc(sizeof(char) * 16);
    *current_index += 1;
    printf("CURRENT INDEX: %c\n", current[*current_index]);
    character_name[0] = current[*current_index];
    *current_index += 1;
    if (current[*current_index] != '\'') {
        fprintf(stderr, "ERROR: Expected close single quote\n");
        exit(1);
    }
    character_name[1] = '\0';
    //printf("keyword_name %s\n", keyword_name);
    TokenType type = TYPE_CHAR;
    Token token = init_token(type, character_name, line, character);
    print_token(token);
    return token;
}

Token generate_num(char *current, int *current_index, int line, int character) {
    //printf("current_index %d line %d character %d\n", *current_index, line, character);
    char *keyword_name = malloc(sizeof(char) * 16);
    int keyword_length = 0;
    while (isdigit(current[*current_index])) {
        keyword_name[keyword_length] = current[*current_index];
        *current_index += 1;
        keyword_length++;
    }
    if (current[*current_index] != '.') {
        keyword_name[keyword_length] = '\0';
        TokenType type = TYPE_INT;
        Token token = init_token(type, keyword_name, line, character);
        return token;
    }
    keyword_name[keyword_length] = current[*current_index];
    *current_index += 1;
    keyword_length++;
    while(isdigit(current[*current_index])){
        keyword_name[keyword_length] = current[*current_index];
        *current_index += 1;
        keyword_length++;
    }
    keyword_name[keyword_length] = '\0';
    TokenType type = TYPE_FLOAT;
    Token token = init_token(type, keyword_name, line, character);
    return token;
}

Lexer lexer(char *file_name) {
    int length;
    int current_index = 0;
    char *current = open_file(file_name, &length);
    int line = 1;
    int character = 1;

    Lexer lex = {.stack_size = 0, .file_name = "test.rasm"};
    while (current_index < length) {
        if (current[current_index] == '\n') {
            line++;
            character = 0;
        }
        if(isalpha(current[current_index])) {
            Token token = generate_keyword(current, &current_index, line, character);
            push_token(&lex, token);
            current_index--;
        } else if (isdigit(current[current_index])) {
            Token token = generate_num(current, &current_index, line, character);
            push_token(&lex, token);
            current_index--;
        } else if (current[current_index] == '\'') {
            Token token = generate_char(current, &current_index, line, character);
            push_token(&lex, token);
        } else if (current[current_index] == ';') {
            while(current[current_index] != '\n') {
                current_index++;
            }
            line++;
        }
        character++;
        current_index++;
    }
    /*for (int i = 0; i <= lex.stack_size; i++) {
        print_token(lex.token_stack[i]);
    }*/
    return lex;
}