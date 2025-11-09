#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* buf = NULL;

char* write_to_buf(char* msg)
{
    size_t size = 0;

    FILE* fp = open_memstream(&buf, &size);

    printf("The file descriptor is %d, buf = %p\n", fileno(fp), buf);

    fwrite(msg, 1, strlen(msg), fp);

    printf("buf = %p\n", buf);

    fclose(fp);

    return buf;
}

int main(void)
{
    char* str = "hello, world!";
    char* buf = write_to_buf(str);

    puts(buf);

    free(buf);

    return 0;
}
