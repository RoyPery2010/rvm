#include "./rvm.c"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: ./rasm <input.rasm> <output.rvm>\n");
        fprintf(stderr, "ERROR: expected input and output\n");
        exit(1);
    }
    const char *input_file_path = argv[1];
    const char *output_file_path = argv[2];
    
    String_View source = slurp_file(input_file_path);

    rvm.program_size = rvm_translate_source(source, rvm.program, RVM_PROGRAM_CAPACITY);

    rvm_save_program_to_file(rvm.program, rvm.program_size, output_file_path);

    return 0;
}
