#include "rasm.h"

#define MAX_PROGRAM_SIZE 1024

void set_value_float(Inst* inst, float v)
{
    inst->data_type = FLOAT_TYPE;
    inst->value.as_float = v;
}

void set_value(Inst* inst, ParseList* head, char* name, int program_size)
{
    switch(head->value.type) {
        case TYPE_INT:
            int i = atoi(head->value.text);
            inst->data_type = INT_TYPE;
            inst->value.as_int = i;           
            printf("Add instruction %d = %s %d\n", program_size, name, i);
            break;
        case TYPE_FLOAT:
            float f = atof(head->value.text);
            inst->data_type = FLOAT_TYPE;
            inst->value.as_float = f;           
            printf("Add instruction %d = %s %f\n", program_size, name, f);
            break;
        case TYPE_CHAR:
            char c = head->value.text[0];
            inst->data_type = CHAR_TYPE;
            inst->value.as_char = c;           
            printf("Add instruction %d = %s %c\n", program_size, name, c);
            break;
        default:
            break;
    }
}
int generate_instructions(ParseList *head, Inst *program) {
    Inst instruction;
    int program_size = 0;
    while(head != NULL) {
        switch (head->value.type) {
            case TYPE_NONE:
                assert(false && "ERROR: Token should not be NONE\n");
                break;
            case TYPE_NOP:
                instruction.type = INST_NOP;
                program[program_size] = instruction;
                //printf("Add instruction %d = NOP\n", program_size);
                break;
            case TYPE_PUSH:
                head = head->next;
                instruction.type = INST_PUSH;
                set_value(&instruction, head, "PUSH", program_size);
                program[program_size] = instruction;      
                break;
            case TYPE_POP:
                instruction.type = INST_POP;
                program[program_size] = instruction;
                break;
            case TYPE_DUP:
                instruction.type = INST_DUP;
                program[program_size] = instruction;
                break;
            case TYPE_INDUP:
                head = head->next;
                instruction.type = INST_INDUP;
                set_value(&instruction, head, "INDUP", program_size);
                break;
            case TYPE_SWAP:
                instruction.type = INST_SWAP;
                program[program_size] = instruction;
                break;
            case TYPE_INSWAP:
                head = head->next;
                instruction.type = INST_INSWAP;
                set_value(&instruction, head, "INSWAP", program_size);
                break;
            case TYPE_ADD:
                instruction.type = INST_ADD;
                program[program_size] = instruction;
                //printf("Add instruction %d = ADD\n", program_size);
                break;
            case TYPE_SUB:
                instruction.type = INST_SUB;
                program[program_size] = instruction;
                break;
            case TYPE_MUL:
                instruction.type = INST_MUL;
                program[program_size] = instruction;
                break;
            case TYPE_DIV:
                instruction.type = INST_DIV;
                program[program_size] = instruction;
                break;
            case TYPE_MOD:
                instruction.type = INST_MOD;
                program[program_size] = instruction;
                break;
            case TYPE_ADD_F:
                instruction.type = INST_ADD_F;
                program[program_size] = instruction;
                printf("Add instruction %d = ADDF\n", program_size);
                break;
            case TYPE_SUB_F:
                instruction.type = INST_SUB_F;
                program[program_size] = instruction;
                break;
            case TYPE_MUL_F:
                instruction.type = INST_MUL_F;
                program[program_size] = instruction;
                break;
            case TYPE_DIV_F:
                instruction.type = INST_DIV_F;
                program[program_size] = instruction;
                break;
            case TYPE_MOD_F:
                instruction.type = INST_MOD_F;
                program[program_size] = instruction;
                break;
            case TYPE_CMPE:
                instruction.type = INST_CMPE;
                program[program_size] = instruction;
                break;
            case TYPE_CMPNE:
                instruction.type = INST_CMPNE;
                program[program_size] = instruction;
                break;
            case TYPE_CMPG:
                instruction.type = INST_CMPG;
                program[program_size] = instruction;
                break;
            case TYPE_CMPL:
                instruction.type = INST_CMPL;
                program[program_size] = instruction;
                break;
            case TYPE_CMPGE:
                instruction.type = INST_CMPGE;
                program[program_size] = instruction;
                break;
            case TYPE_CMPLE:
                instruction.type = INST_CMPLE;
                program[program_size] = instruction;
                break;
            case TYPE_JMP:
                head = head->next;
                instruction.type = INST_JMP;
                program[program_size] = instruction;
                set_value(&instruction, head, "JMP", program_size);
                break;
            case TYPE_ZJMP:
                head = head->next;
                instruction.type = INST_ZJMP;
                program[program_size] = instruction;
                set_value(&instruction, head, "ZJMP", program_size);
                break;
            case TYPE_NZJMP:
                head = head->next;
                instruction.type = INST_NZJMP;
                program[program_size] = instruction;
                set_value(&instruction, head, "NZJMP", program_size);
                break;
            case TYPE_PRINT:
                instruction.type = INST_PRINT;
                program[program_size] = instruction;
                printf("Add instruction %d = PRINT\n", program_size);
                break;
            case TYPE_INT:
                assert(false && "ERROR: Should not be INT\n");
                break;
            case TYPE_FLOAT:
                assert(false && "ERROR: Should not be FLOAT\n");
                break;
            case TYPE_CHAR:
                assert(false && "ERROR: Should not be CHAR\n");
                break;
            case TYPE_LABEL_DEF:
                assert(false && "ERROR: Should not be LABEL_DEF\n");
                break;
            case TYPE_LABEL:
                assert(false && "ERROR: Should not be LABEL\n");
                break;
            case TYPE_HALT:
                instruction.type = INST_HALT;
                program[program_size] = instruction;
                break;
        }
        //printf("\tJust added instruction type %d\n", program[program_size].type);
        head = head->next;
        ++program_size;
    }
    return program_size;
}


int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr, "Usage: %s <file_name.rasm>\n", argv[0]);
        exit(1);
    }
    char *file_name = argv[1];
    Inst program[MAX_PROGRAM_SIZE] = {0};
    Lexer lex = lexer(file_name);
    ParseList list = parser(lex);
    int program_size = generate_instructions(&list, program);
    Machine machine = {.instructions = program, .program_size = program_size};
    //printf("program_size = %d\n", machine.program_size); 
    run_instructions(&machine);
    write_program_to_file(&machine, "machine.rvm");
    //print_stack(&machine);
    return 0;
}