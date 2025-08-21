#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

int main(void) {
    printf("%s\n", crypt("123456", "$1$"));
    return 0;
}
