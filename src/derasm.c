#include "./rvm.h"
RVM rvm = {0};
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./derasm <input.rvm>\n");
        fprintf(stderr, "ERROR: no input is provided\n");
        exit(1);
    }

    const char *input_file_path = argv[1];

    rvm_load_program_from_file(&rvm, input_file_path);

    for (Word i = 0; i < rvm.program_size; ++i) {
        switch (rvm.program[i].type) {
            case INST_NOP:
                printf("nop\n");
                break;
            case INST_PUSH:
                printf("push %ld\n", rvm.program[i].operand);
                break;
            case INST_DUP:
                printf("dup %ld\n", rvm.program[i].operand);
                break;
            case INST_PLUS:
                printf("plus\n");
                break;
            case INST_MINUS:
                printf("minus\n");
                break;
            case INST_MULT:
                printf("mult\n");
                break;
            case INST_DIV:
                printf("div\n");
                break;
            case INST_JMP:
                printf("jmp %ld\n", rvm.program->operand);
                break;
            case INST_JMP_IF:
                printf("jmp_if %ld\n", rvm.program->operand);
                break;
            case INST_EQ:
                printf("eq\n");
                break;
            case INST_HALT:
                printf("halt\n");
                break;
            case INST_PRINT_DEBUG:
                printf("print_debug\n");
                break;
        }
    }
    return 0;
}
