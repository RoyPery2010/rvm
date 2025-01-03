#pragma once

#define MAX_TOKEN_STACK_SIZE 1024
#include <stdbool.h>
typedef enum {
    TYPE_NONE = -1,
    TYPE_NOP = 0,
    TYPE_PUSH,
    TYPE_POP,
    TYPE_DUP,
    TYPE_INDUP,
    TYPE_SWAP,
    TYPE_INSWAP,
    TYPE_ADD,
    TYPE_SUB,
    TYPE_MUL,
    TYPE_DIV,
    TYPE_MOD,
    TYPE_ADD_F,
    TYPE_SUB_F,
    TYPE_MUL_F,
    TYPE_DIV_F,
    TYPE_MOD_F,
    TYPE_CMPE,
    TYPE_CMPNE,
    TYPE_CMPG,
    TYPE_CMPL,
    TYPE_CMPGE,
    TYPE_CMPLE,
    TYPE_JMP,
    TYPE_ZJMP,
    TYPE_NZJMP,
    TYPE_PRINT,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_LABEL_DEF,
    TYPE_LABEL,
    TYPE_HALT,
} TokenType;

typedef struct {
    TokenType type;
    char *text;
    int line;
    int character;
} Token;

typedef struct {
    Token token_stack[MAX_TOKEN_STACK_SIZE];
    int stack_size;
    char *file_name;
} Lexer;
char *open_file(char *file_path, int *length);
void push_token(Lexer *lexer, Token value);
Token pop_token(Lexer *lexer);
Token init_token(TokenType type, char *text, int line, int character);
TokenType check_builtin_keywords(char *name);
void print_token(Token token);
TokenType check_label_type(char *current, int *current_index);
Token generate_keyword(char *current, int *current_index, int line, int character);
Token generate_num(char *current, int *current_index, int line, int character);
Lexer lexer(char *file_name);