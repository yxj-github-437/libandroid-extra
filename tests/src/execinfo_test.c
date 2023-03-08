#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BACKTRACE_SIZE 100

void print_backtrace()
{
    void* buffer[BACKTRACE_SIZE] = {0};
    int pointer_num = backtrace(buffer, BACKTRACE_SIZE);
    char** string_buffer = backtrace_symbols(buffer, pointer_num);
    if (string_buffer == NULL) {
        printf("backtrace_symbols error\n");
        exit(-1);
    }

    printf("print backtrace begin\n");
    for (int i = 0; i < pointer_num; i++) {
        printf("%s\n", string_buffer[i]);
    }
    printf("print backtrace end\n");

    free(string_buffer);

    return;
}

void func(int num)
{
    if (num > 0) {
        func(--num);
    }
    else {
        print_backtrace();
    }
}

int main(int argc, char* argv[])
{
    int input_num = 1;
    func(input_num);

    return 0;
}
