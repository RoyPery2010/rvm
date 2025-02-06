#include "./rvm.h"
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./rvme <input.rvm>\n");
        fprintf(stderr, "ERROR: expected input\n");
        exit(1);
    }
    RVM rvm = {0};
    rvm_load_program_from_file(&rvm, argv[1]);
    Err err = rvm_execute_program(&rvm, 69);
    rvm_dump_stack(stdout, &rvm);
    if (err != ERR_OK) {
        fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
        return 1;
    }
    return 0;
}
