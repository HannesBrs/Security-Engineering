#include <stdio.h>

int main(int argc, char **argv) {
    printf("-----------------------------------------------\n");
    printf("Hello from prog, executed by the child process!\n");
    printf("Here are your args:\n");
    for (int i = 0; i < argc; i++) {
        printf("arg%d = %s\n", i, argv[i]);
    }
    printf("-----------------------------------------------\n");

    return 0;
}
