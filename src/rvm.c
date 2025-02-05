#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "./rvm.h"

const char *err_as_cstr(Err err)
{
    switch (err)
    {
    case ERR_OK:
        return "ERR_OK";
    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_ILLEGAL_INST:
        return "ERR_ILLEGAL_INST";
    case ERR_DIV_BY_ZERO:
        return "ERR_DIV_BY_ZERO";
    case ERR_ILLEGAL_INST_ACCESS:
        return "ERR_ILLEGAL_INST_ACCESS";
    case ERR_ILLEGAL_OPERAND:
        return "ERR_ILLEGAL_OPERAND";
    default:
        assert(0 && "err_as_cstr: Unreachable");
    }
}






const char *inst_type_as_cstr(Inst_Type type)
{
    switch (type)
    {
    case INST_NOP:
        return "INST_NOP";
    case INST_PUSH:
        return "INST_PUSH";
    case INST_DUP:
        return "INST_DUP";
    case INST_PLUS:
        return "INST_PLUS";
    case INST_MINUS:
        return "INST_MINUS";
    case INST_MULT:
        return "INST_MULT";
    case INST_DIV:
        return "INST_DIV";
    case INST_JMP:
        return "INST_JMP";
    case INST_JMP_IF:
        return "INST_JMP_IF";
    case INST_HALT:
        return "INST_HALT";
    case INST_EQ:
        return "INST_EQ";
    case INST_PRINT_DEBUG:
        return "INST_PRINT_DEBUG";
    default:
        assert(0 && "inst_type_as_cstr: unreachable");
    }
}



#define MAKE_INST_PUSH(value) {.type = INST_PUSH, .operand = (value)}
#define MAKE_INST_PLUS {.type = INST_PLUS}
#define MAKE_INST_MINUS {.type = INST_MINUS}
#define MAKE_INST_MULT {.type = INST_MULT}
#define MAKE_INST_DIV {.type = INST_DIV}
#define MAKE_INST_JMP(addr) {.type = INST_JMP, .operand = (addr)}
#define MAKE_INST_HALT {.type = INST_HALT, .operand = (addr)}
#define MAKE_INST_DUP(addr) {.type = INST_DUP, .operand = (addr)}

Err rvm_execute_inst(RVM *rvm)
{
    if (rvm->ip < 0 || rvm->ip >= rvm->program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }
    Inst inst = rvm->program[rvm->ip];

    switch (inst.type)
    {
    case INST_NOP:
        rvm->ip += 1;
        break;
    case INST_PUSH:
        if (rvm->stack_size >= RVM_STACK_CAPACITY)
        {
            return ERR_STACK_OVERFLOW;
        }
        rvm->stack[rvm->stack_size++] = inst.operand;
        rvm->ip += 1;
        break;
    case INST_PLUS:
        if (rvm->stack_size < 2)
        {
            return ERR_STACK_UNDERFLOW;
        }
        rvm->stack[rvm->stack_size - 2] += rvm->stack[rvm->stack_size - 1];
        rvm->stack_size -= 1;
        rvm->ip += 1;
        break;
    case INST_MINUS:
        if (rvm->stack_size < 2)
        {
            return ERR_STACK_UNDERFLOW;
        }
        rvm->stack[rvm->stack_size - 2] -= rvm->stack[rvm->stack_size - 1];
        rvm->stack_size -= 1;
        rvm->ip += 1;
        break;
    case INST_MULT:
        if (rvm->stack_size < 2)
        {
            return ERR_STACK_UNDERFLOW;
        }
        rvm->stack[rvm->stack_size - 2] *= rvm->stack[rvm->stack_size - 1];
        rvm->stack_size -= 1;
        rvm->ip += 1;
        break;
    case INST_DIV:
        if (rvm->stack_size < 2)
        {
            return ERR_STACK_UNDERFLOW;
        }
        if (rvm->stack[rvm->stack_size - 1] == 0) {
            return ERR_DIV_BY_ZERO;
        }
        rvm->stack[rvm->stack_size - 2] /= rvm->stack[rvm->stack_size - 1];
        rvm->stack_size -= 1;
        rvm->ip += 1;
        break;
    case INST_JMP:
        rvm->ip = inst.operand;
        break;
    case INST_HALT:
        rvm->halt = 1;
        break;
    case INST_EQ:
        if (rvm->stack_size < 2)
        {
            return ERR_STACK_UNDERFLOW;
        }
        rvm->stack[rvm->stack_size - 2] = rvm->stack[rvm->stack_size - 1] == rvm->stack[rvm->stack_size - 2];
        rvm->stack_size -= 1;
        rvm->ip += 1;
        break;
    case INST_JMP_IF:
        if (rvm->stack_size < 2)
        {
            return ERR_STACK_UNDERFLOW;
        }
        if (rvm->stack[rvm->stack_size - 1]) {
            rvm->stack_size -= 1;
            rvm->ip = inst.operand;
        } else {
            rvm->ip += 1;
        }
        break;
    case INST_PRINT_DEBUG:
        if (rvm->stack_size < 1)
        {
            return ERR_STACK_UNDERFLOW;
        }
        printf("%ld\n", rvm->stack[rvm->stack_size - 1]);
        rvm->stack_size -= 1;
        rvm->ip += 1;
        break;
    case INST_DUP:
        if (rvm->stack_size >= RVM_STACK_CAPACITY)
        {
            return ERR_STACK_OVERFLOW;
        }
        if (rvm->stack_size - inst.operand <= 0) {
            return ERR_STACK_UNDERFLOW;
        }
        if (inst.operand < 0) {
            return ERR_ILLEGAL_OPERAND;
        }
        rvm->stack[rvm->stack_size] = rvm->stack[rvm->stack_size - 1 - inst.operand];
        rvm->stack_size += 1;
        rvm->ip += 1;
        break;
    default:
        return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}

void rvm_dump_stack(FILE *stream, const RVM *rvm)
{
    fprintf(stream, "Stack:\n");
    if (rvm->stack_size > 0)
    {
        for (Word i = 0; i < rvm->stack_size; ++i)
        {
            fprintf(stream, " %ld\n", rvm->stack[i]);
        }
    }
    else
    {
        fprintf(stream, "   [empty]\n");
    }
}




void rvm_load_program_from_memory(RVM *rvm, Inst *program, size_t program_size) {
    assert(program_size < RVM_PROGRAM_CAPACITY);
    memcpy(rvm->program, program, sizeof(program[0]) * program_size);
    rvm->program_size = program_size;
}

void rvm_load_program_from_file(RVM *rvm, const char *file_path) {
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    long m = ftell(f);
    if (m < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    assert(m % sizeof(rvm->program[0]) == 0);
    assert((size_t) m <= RVM_PROGRAM_CAPACITY * sizeof(rvm->program[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    rvm->program_size = fread(rvm->program, sizeof(rvm->program[0]), m / sizeof(rvm->program[0]), f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    fclose(f);
}
void rvm_save_program_to_file(Inst *program, size_t program_size, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    fwrite(program, sizeof(program[0]), program_size, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not write to file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    fclose(f);
}




String_View cstr_as_sv(const char *cstr) {
    return (String_View) {
        .count = strlen(cstr),
        .data = cstr,
    };
}

String_View sv_trim_left(String_View sv) {
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i += 1;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data + i,
    };
}

String_View sv_trim_right(String_View sv) {
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
        i += 1;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data,
    };
}

String_View sv_trim(String_View sv) {
    return sv_trim_right(sv_trim_left(sv));
}

String_View sv_chop_by_delim(String_View *sv, char delim) {
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i += 1;
    }
    String_View result = {
        .count = i,
        .data = sv->data
    };
    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data += i + 1;
    } else {
        sv->count -= i;
        sv->data += i;
    }
    return result;
}

int sv_eq(String_View a, String_View b) {
    if (a.count != b.count) {
        return 0;
    } else {
        return memcmp(a.data, b.data, a.count) == 0;
    }
}

int sv_to_int(String_View sv) {
    int result = 0;

    for (size_t i = 0; i < sv.count && isdigit(sv.data[i]); ++i) {
        result = result * 10 + sv.data[i] - '0';
    }

    return result;
}


Inst rvm_translate_line(String_View line) {
    line = sv_trim_left(line);
    String_View inst_name = sv_chop_by_delim(&line, ' ');
    if (sv_eq(inst_name, cstr_as_sv("push"))) {
        line = sv_trim_left(line);
        int operand = sv_to_int(sv_trim_right(line));
        return (Inst) {.type = INST_PUSH, .operand = operand};
    } else if (sv_eq(inst_name, cstr_as_sv("dup"))) {
        line = sv_trim_left(line);
        int operand = sv_to_int(sv_trim_right(line));
        return (Inst) {.type = INST_DUP, .operand = operand};
    } else if (sv_eq(inst_name, cstr_as_sv("plus"))) {
        return (Inst) {.type = INST_PLUS};
    } else if (sv_eq(inst_name, cstr_as_sv("jmp"))) {
        line = sv_trim_left(line);
        int operand = sv_to_int(sv_trim_right(line));
        return (Inst) {.type = INST_JMP, .operand = operand};
    }
    else {
        fprintf(stderr, "ERROR: unknown instruction `%.*s`", (int) inst_name.count, inst_name.data);
        exit(1);
    }
}


size_t rvm_translate_source(String_View source, Inst *program, size_t program_capacity) {
    size_t program_size = 0;
    while (source.count > 0) {
        assert(program_size < program_capacity);
        String_View line = sv_chop_by_delim(&source, '\n');
        if (line.count > 0) {
            program[program_size++] = rvm_translate_line(line);
        }
    }
    return program_size;
}

String_View slurp_file(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    long m = ftell(f);
    if (m < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }
    char *buffer = malloc(m);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for file %s\n", strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    size_t n = fread(buffer, 1, m, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n", file_path, strerror(errno));
        exit(1);
    }

    fclose(f);

    return (String_View) {
        .count = n,
        .data = buffer,
    };
}
