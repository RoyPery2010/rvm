set -xe
gcc rvm.c rasmlexer.c rasmparser.c rasm.c hashmap.c -o rasm -Wall -Wextra
gcc rvm.c time.c -o rvm -Wall -Wextra