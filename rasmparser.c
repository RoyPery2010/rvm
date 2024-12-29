#include "rasmparser.h"

void append(ParseList *head, Token value)
{
    ParseList *ptr = malloc(sizeof(ParseList));
    ptr->value = value;
    ptr->next = NULL;

    ParseList *tmp = head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = ptr;
    
}

void handle_token_def(Lexer *lex, int index, HashMap* hashmap) {
    int line_num = lex->token_stack[index].line - 1;
    printf("%d-------\n", line_num);
    char *text = lex->token_stack[index].text;
    insert(hashmap, text, line_num);
    printf("\tMap label %s to %d\n", text, line_num);
    lex->token_stack[index].type = TYPE_NOP;
}

void generate_list(ParseList *root, Lexer *lexer, HashMap* hashmap)
{
    assert(root != NULL && "Root cannot equal NULL\n");
    for (int index = 1; index <= lexer->stack_size; index++)
    {
        printf("PARSE %d: ", index); 
        //print_token(lexer->token_stack[index]);
        switch (lexer->token_stack[index].type) {
            case TYPE_NONE:
                assert(false && "Token should not be NONE\n");
                break;
            case TYPE_NOP:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_PUSH:
                //printf("Parser: PUSH\n");
                append(root, lexer->token_stack[index]);
                index++;
                if (lexer->token_stack[index].type != TYPE_INT && lexer->token_stack[index].type != TYPE_FLOAT && lexer->token_stack[index].type != TYPE_CHAR && lexer->token_stack[index].type != TYPE_LABEL) {
                    fprintf(stderr, "ERROR: Expected type INT but found %s\n", "TODO: Implement token print");
                    exit(1);
                }
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_POP:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_DUP:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_INDUP:
                append(root, lexer->token_stack[index]);
                index++;
                if (lexer->token_stack[index].type != TYPE_INT && lexer->token_stack[index].type != TYPE_LABEL) {
                    fprintf(stderr, "ERROR: Expected type INT but found %s\n", "TODO: Implement token print");
                    exit(1);
                }
                break;
            case TYPE_SWAP:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_INSWAP:
                append(root, lexer->token_stack[index]);
                index++;
                if (lexer->token_stack[index].type != TYPE_INT && lexer->token_stack[index].type != TYPE_LABEL) {
                    fprintf(stderr, "ERROR: Expected type INT but found %s\n", "TODO: Implement token print");
                    exit(1);
                }
                break;
            case TYPE_ADD:
                //printf("Parser: ADD\n");
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_SUB:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_MUL:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_DIV:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_MOD:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_ADD_F:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_SUB_F:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_MUL_F:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_DIV_F:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_MOD_F:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_CMPE:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_CMPNE:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_CMPG:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_CMPL:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_CMPGE:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_CMPLE:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_JMP:
                append(root, lexer->token_stack[index]);
                index++;
                if (lexer->token_stack[index].type != TYPE_INT && lexer->token_stack[index].type != TYPE_LABEL) {
                    fprintf(stderr, "ERROR: Expected type INT but found %s\n", "TODO: Implement token print");
                    exit(1);
                }
                break;
            case TYPE_ZJMP:
                append(root, lexer->token_stack[index]);
                index++;
                if (lexer->token_stack[index].type != TYPE_INT && lexer->token_stack[index].type != TYPE_LABEL) {
                    fprintf(stderr, "ERROR: Expected type INT but found %s\n", "TODO: Implement token print");
                    exit(1);
                }
                break;
            case TYPE_NZJMP:
                append(root, lexer->token_stack[index]);
                index++;
                if (lexer->token_stack[index].type != TYPE_INT && lexer->token_stack[index].type != TYPE_LABEL) {
                    fprintf(stderr, "ERROR: Expected type INT but found %s\n", "TODO: Implement token print");
                    exit(1);
                }
                break;
            case TYPE_PRINT:
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_INT:
                index -= 2;
                break;
            case TYPE_FLOAT:
                index -= 2;
                break;
            case TYPE_CHAR:
                index -= 2;
                break;
            case TYPE_LABEL_DEF:
                handle_token_def(lexer, index, hashmap);
                append(root, lexer->token_stack[index]);
                break;
            case TYPE_LABEL:
                index -= 2;
                break;
            case TYPE_HALT:
                append(root, lexer->token_stack[index]);
                break;
            default:
                assert(false && "UNREACHABLE");
        }
    }
}

void print_list(ParseList *head)
{
    printf("Start ParseList\n");
    while (head != NULL)
    {
        print_token(head->value);
        head = head->next;
    }
    printf("End ParseList\n");
}

void check_labels(ParseList *head, HashMap* hashmap) {
    while(head != NULL) {
        if (head->value.type == TYPE_LABEL){
            int label_index = get(hashmap, head->value.text);
            printf("%d\n", label_index);
            if (label_index == -1) {
                fprintf(stderr, "ERROR: Undeclared label: %s\n", head->value.text);
                exit(1);
            }
            printf("\tReplace label %s with %d\n", head->value.text, label_index);
            head->value.type = TYPE_INT;
            sprintf(head->value.text, "%d", label_index);
        }
        head = head->next;
    }
}

ParseList parser(Lexer lexer)
{
    HashMap *label_map = createHashMap();
    if (lexer.token_stack[0].type == TYPE_LABEL_DEF) {
        handle_token_def(&lexer, 0, label_map);
    }
    ParseList root = {.value = lexer.token_stack[0], .next = NULL};
    generate_list(&root, &lexer, label_map);
    check_labels(&root, label_map);
    //print_list(&root);
    return root;
}