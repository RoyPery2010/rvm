#ifndef RVM_H_
#define RVM_H_
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))
#define RVM_PROGRAM_CAPACITY 1024
#define RVM_STACK_CAPACITY 1024
#define RVM_EXECUTION_LIMIT 69



typedef enum
{
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_ILLEGAL_INST,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INST_ACCESS,
    ERR_ILLEGAL_OPERAND,
} Err;

const char *err_as_cstr(Err err);



typedef int64_t Word;

typedef enum
{
    INST_NOP = 0,
    INST_PUSH,
    INST_DUP,
    INST_PLUS,
    INST_MINUS,
    INST_MULT,
    INST_DIV,
    INST_JMP,
    INST_JMP_IF,
    INST_EQ,
    INST_HALT,
    INST_PRINT_DEBUG,
} Inst_Type;

typedef struct
{
    Inst_Type type;
    Word operand;
} Inst;

typedef struct
{
    Word stack[RVM_STACK_CAPACITY];
    Word stack_size;
    Inst program[RVM_PROGRAM_CAPACITY];
    Word program_size;
    Word ip;
    int halt;
} RVM;


const char *inst_type_as_cstr(Inst_Type type);

Err rvm_execute_inst(RVM *rvm);

void rvm_dump_stack(FILE *stream, const RVM *rvm);



// RVM rvm = {0};

void rvm_load_program_from_memory(RVM *rvm, Inst *program, size_t program_size);

void rvm_load_program_from_file(RVM *rvm, const char *file_path);
void rvm_save_program_to_file(const RVM *rvm, const char *file_path);


typedef struct {
    size_t count;
    const char *data;
} String_View;

String_View cstr_as_sv(const char *cstr);

String_View sv_trim_left(String_View sv);

String_View sv_trim_right(String_View sv);

String_View sv_trim(String_View sv);

String_View sv_chop_by_delim(String_View *sv, char delim);

int sv_eq(String_View a, String_View b);

int sv_to_int(String_View sv);


Inst rvm_translate_line(String_View line);


size_t rvm_translate_source(String_View source, Inst *program, size_t program_capacity);

String_View slurp_file(const char *file_path);

#endif // RVM_H_
