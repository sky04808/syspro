#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i;
    struct stat buf;

    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &buf) < 0) {
            perror("lstat()");
            continue;
        }

        if (S_ISREG(buf.st_mode))
            printf("Regular File\n");
        if (S_ISDIR(buf.st_mode))
            printf("Directory\n");
        if (S_ISCHR(buf.st_mode))
            printf("Character Device\n");
        if (S_ISBLK(buf.st_mode))
            printf("Block Device\n");
        if (S_ISFIFO(buf.st_mode))
            printf("FIFO\n");
        if (S_ISLNK(buf.st_mode))
            printf("Symbolic Link\n");
        if (S_ISSOCK(buf.st_mode))
            printf("Socket\n");
    }

    exit(0);
}
