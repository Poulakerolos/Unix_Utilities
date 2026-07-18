#include <unistd.h>
#include <fcntl.h>
#define count 1000
int mv_main(int argc, char *argv[])
{
    char buf[count];
    if (argc != 3)
        return -1;
   
    int cut = open(argv[1], O_RDONLY);

    if (cut < 0)
        return -1;

    int paste = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (paste < 0)
        return -1;

    int readnum = 0;
    while ((readnum = read(cut, buf, count)) > 0)
        write(paste, buf, readnum);

    int remove = unlink(argv[1]);
    if (remove == 0){
        close(paste);
        return 0;
        }
    else
        return -1;
}