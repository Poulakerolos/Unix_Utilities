#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define count 1000
int cp_main(int argc, char *argv[])
{
    if (argc != 3)
    {
        write(1, "few arguments", strlen("few arguments"));
        return -1;
    }
    char buf[count];
   
    int copy = open(argv[1], O_RDONLY);
    if (copy < 0)
    {
        return -1;
    }
    int paste = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // int paste=open(argv[2],O_WRONLY);
    if (paste < 0)
    {
        return -1;
    }

    int readnum = 0;
    while ((readnum = read(copy, buf, count)) > 0)
        write(paste, buf, readnum);
        close(copy);
    return 0;
}