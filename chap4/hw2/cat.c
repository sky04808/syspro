#include <stdio.h>
#include <string.h>

/* extended cat: multiple files, -n option for line numbers */
int main(int argc, char *argv[])
{
    FILE *fp;
    int c;
    int i = 1;
    int nflag = 0;
    int line = 1;
    int newline = 1;

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        nflag = 1;
        i = 2;
    }

    if (i >= argc) {
        while ((c = getc(stdin)) != EOF) {
            if (nflag && newline) {
                printf("%d ", line++);
                newline = 0;
            }
            putc(c, stdout);
            if (c == '\n')
                newline = 1;
        }
        return 0;
    }

    for (; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "File %s Open Error\n", argv[i]);
            continue;
        }
        while ((c = getc(fp)) != EOF) {
            if (nflag && newline) {
                printf("%d ", line++);
                newline = 0;
            }
            putc(c, stdout);
            if (c == '\n')
                newline = 1;
        }
        fclose(fp);
    }
    return 0;
}
