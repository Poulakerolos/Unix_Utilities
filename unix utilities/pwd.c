#include <unistd.h>
#include <stdio.h>
#define count 1000
int pwd_main()
{
    char buf[count];
    char *out = getcwd(buf, count);
   
    if (out)
    {
        printf("%s\n", out);
        return 0;
    }
    else
    {
        printf("error\n");
        return -1;
    }
}