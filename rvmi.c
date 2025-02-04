#include "./rvm.c"
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./rvmi <input.rvm>\n");
        fprintf(stderr, "ERROR: expected input\n");
        exit(1);
    }
    rvm_load_program_from_file(&rvm, argv[1]);
    rvm_dump_stack(stdout, &rvm);
    for (int i = 0; i < 69 && !rvm.halt; ++i) {
        Err err = rvm_execute_inst(&rvm);
        rvm_dump_stack(stdout, &rvm);
        if (err != ERR_OK)
        {
            fprintf(stderr, "ERROR: %s\n", err_as_cstr(err));
            exit(1);
        }
    }
    return 0;
}
