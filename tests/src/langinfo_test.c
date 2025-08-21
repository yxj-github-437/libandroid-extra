#include <langinfo.h>
#include <stdio.h>

int main(void)
{
   printf("Current codeset is %s\n", nl_langinfo(CODESET));
   printf("%s\n", nl_langinfo(MON_1));
   return 0;
}
