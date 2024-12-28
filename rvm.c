#include "rvm.h"
#include "rasmlexer.h"


Inst program[] = {
    DEF_INST_PUSH(1),
    DEF_INST_PUSH(4),
    DEF_INST_PUSH(6),
    DEF_INST_PUSH(8),
    DEF_INST_PUSH(10),
    DEF_INST_PUSH(12),
    DEF_INST_INDUP(2),
    DEF_INST_PRINT(),


};

#define PROGRAM_SIZE (sizeof(program)/sizeof(program[0]))


void push(Machine *machine, int value) {
    if(machine->stack_size >= MAX_STACK_SIZE) {
        fprintf(stderr, "ERROR: Stack Overflow\n");
        exit(1);
    }
    machine->stack[machine->stack_size] = value;
    machine->stack_size++;
}
int pop(Machine *machine) {
    if(machine->stack_size <= 0) {
        fprintf(stderr, "ERROR: rvm - Stack Underflow\n");
        exit(1);
    }
    int value = machine->stack[machine->stack_size - 1];
    machine->stack_size--;
    return value;
}
void index_swap(Machine *machine, int index) {
    if (index > machine->stack_size || index < 0) {
        fprintf(stderr, "ERROR: Index out of range\n");
        exit(1);
    }
    int temp_value = machine->stack[index];
    machine->stack[index] = pop(machine);
    push(machine, temp_value);
}
void index_dup(Machine *machine, int index) {
    if (index > machine->stack_size || index < 0) {
        fprintf(stderr, "ERROR: Index out of range\n");
        exit(1);
    }
    push(machine, machine->stack[index]);
}
void print_stack(Machine *machine) {
    printf("------ STACK\n");
    for (int i = machine->stack_size - 1; i >= 0; i--) {
        printf("%d\n", machine->stack[i]);
    }
    printf("------ END OF STACK\n");
}

void write_program_to_file(Machine *machine, char *file_path) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not write to file %s\n", file_path);
        exit(1);
    }
    int count = fwrite(machine->instructions, sizeof(Inst), PROGRAM_SIZE, file);
    printf("Written %d / %lu instructions to file %s\n", count, PROGRAM_SIZE, file_path);

    fclose(file);
}
Machine *read_program_from_file(Machine *machine, char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not read from file %s\n", file_path);
        exit(1);
    }
    Inst *instructions = malloc(sizeof(Inst) * MAX_STACK_SIZE);

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(instructions, sizeof(instructions[0]), length / 8, file);
    printf("PROGRAM_SIZE: %zu, length: %d, %ld\n", sizeof(&instructions)/sizeof(instructions[0]), length/8, PROGRAM_SIZE);
    machine->program_size = length / 8;
    machine->instructions = instructions;
    machine->stack_size = 0;
    fclose(file);
    return machine;
}
void run_instructions(Machine *machine) {
    int a, b;
    printf("RUNNING\n");
    //printf("run_instructions - program_size = %d\n", machine->program_size); 
    for (int ip = 0; ip < machine->program_size; ip++) {
        //print_stack(machine);
        //printf("run_instruction ip = %d type = %d\n", ip, (int) machine->instructions[ip].type);
        switch(machine->instructions[ip].type) {
	        case INST_NOP:
		        continue;
            case INST_PUSH:
                //printf("push %d\n", machine->instructions[ip].value);
                push(machine, machine->instructions[ip].value);
                break;
            case INST_POP:
                pop(machine);
                break;
            case INST_DUP:
                a = pop(machine);
                push(machine, a);
                push(machine, a);
                break;
            case INST_INDUP:
                //printf("indup %d\n", machine->instructions[ip].value);
                index_dup(machine, machine->instructions[ip].value);
                break;
            case INST_SWAP:
                a = pop(machine);
                b = pop(machine);
                push(machine, a);
                push(machine, b);
                break;
            case INST_INSWAP:
                index_swap(machine, machine->instructions[ip].value);
                break;
            case INST_ADD:
                a = pop(machine);
                b = pop(machine);
                //printf("add %d %d\n", a, b);
                push(machine, a + b);
                break;
            case INST_SUB:
                a = pop(machine);
                b = pop(machine);
                push(machine, b - a);
                break;
            case INST_MUL:
                a = pop(machine);
                b = pop(machine);
                push(machine, a * b);
                break;
            case INST_DIV:
                a = pop(machine);
                b = pop(machine);
                if (b == 0) {
                    fprintf(stderr, "ERROR: Cannot divide by 0\n");
                    exit(1);
                }
                push(machine, b / a);
                break;
            case INST_MOD:
                a = pop(machine);
                b = pop(machine);
                push(machine, a % b);
                break;
            case INST_CMPE:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a == b) {
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPNE:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a != b) {
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPG:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a > b) {
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
            case INST_CMPL:
                a = pop(machine);
                b = pop(machine);
                push(machine, b);
                push(machine, a);
                if (a < b) {
                    push(machine, 1);
                } else {
                    push(machine, 0);
                }
                break;
	        case INST_CMPGE:
                a = pop(machine);                                           
                b = pop(machine);                                           
                push(machine, b);                                           
                push(machine, a);
		        if (a >= b) {
		            push(machine, 1);
		        } else {
		        push(machine, 0);
		        }
		        break;
	        case INST_CMPLE:
		        a = pop(machine);
		        b = pop(machine);
		        push(machine, b);
		        push(machine, a);
		        if (a <= b) {
		            push(machine, 1);
		        } else {
		            push(machine, 0);
		        }
		        break;
            case INST_JMP:
                ip = machine->instructions[ip].value;
                if (ip + 1 >= machine->program_size) {
                        fprintf(stderr, "ERROR: Cannot jump out of bounds\n");
                        exit(1);
                }
                break;
            case INST_ZJMP:
                if (pop(machine) == 0) {
                    printf("IP: %d\n", ip);
                    ip = machine->instructions[ip].value;
                    if (ip + 1 >= machine->program_size) {
                        fprintf(stderr, "ERROR: Cannot jump out of bounds\n");
                        exit(1);
                    }
                    printf("IP: %d\n", ip);
                } else {
                    continue;
                }
                break;
            case INST_NZJMP:
                if (pop(machine) != 0) {
                    printf("IP: %d\n", ip);
                    ip = machine->instructions[ip].value;
                    if (ip + 1 >= machine->program_size) {
                        fprintf(stderr, "ERROR: Cannot jump out of bounds\n");
                        exit(1);
                    }
                    printf("IP: %d\n", ip);
                } else {
                    continue;
                }
                break;
            case INST_PRINT:
                printf("%d\n", pop(machine));
                break;
	        case INST_HALT:
		        ip = machine->program_size;
		        break;
        }
    }
}
/*
int rvm() {
    lexer();
    Machine *loaded_machine = malloc(sizeof(Machine));
    loaded_machine->instructions = program;
    write_program_to_file(loaded_machine, "test.rvm");
    loaded_machine = read_program_from_file(loaded_machine, "test.rvm");
    run_instructions(loaded_machine);
    print_stack(loaded_machine);
    return 0;
}
*/