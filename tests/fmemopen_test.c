#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* write_to_buf(char* msg)
{
    char* buf = (char*)malloc(1024);

    FILE* fp = fmemopen(buf, 1024, "w");
    fwrite(msg, 1, strlen(msg), fp);
    fclose(fp);

    return buf;
}

int main(void)
{
    char* str = "hello, world!";
    char* buf = write_to_buf(str);

    printf("%s\n", buf);

    free(buf);

    return 0;
}
