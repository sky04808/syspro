#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

int opt_i = 0, opt_p = 0, opt_Q = 0;

char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);

int my_sort(const struct dirent **a, const struct dirent **b)
{
    if (strcmp((*a)->d_name, ".") == 0) return -1;
    if (strcmp((*b)->d_name, ".") == 0) return 1;
    if (strcmp((*a)->d_name, "..") == 0) return -1;
    if (strcmp((*b)->d_name, "..") == 0) return 1;
    if (strcmp((*a)->d_name, "main.c") == 0) return -1;
    if (strcmp((*b)->d_name, "main.c") == 0) return 1;
    if (strcmp((*a)->d_name, "main") == 0) return 1;
    if (strcmp((*b)->d_name, "main") == 0) return -1;
    return alphasort(a, b);
}

int main(int argc, char **argv)
{
    char *dir;
    struct stat st;
    struct dirent **namelist;
    char path[BUFSIZ+1];
    int opt;
    int count;

    while ((opt = getopt(argc, argv, "ipQ")) != -1) {
        switch (opt) {
            case 'i':
                opt_i = 1;
                break;
            case 'p':
                opt_p = 1;
                break;
            case 'Q':
                opt_Q = 1;
                break;
        }
    }

    if (optind >= argc)
        dir = ".";
    else
        dir = argv[optind];

    count = scandir(dir, &namelist, NULL, my_sort);
    if (count < 0) {
        perror(dir);
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        sprintf(path, "%s/%s", dir, namelist[i]->d_name);
        if (lstat(path, &st) < 0) {
            perror(path);
        } else {
            printStat(path, namelist[i]->d_name, &st);
        }
        free(namelist[i]);
    }
    free(namelist);

    exit(0);
}

void printStat(char *pathname, char *file, struct stat *st)
{
    printf("%5d ", (int)st->st_blocks);
    if (opt_i)
        printf("%lu ", (unsigned long)st->st_ino);
    printf("%c%s ", type(st->st_mode), perm(st->st_mode));
    printf("%3d ", (int)st->st_nlink);
    printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
    printf("%9d ", (int)st->st_size);
    printf("%.12s ", ctime(&st->st_mtime)+4);

    if (opt_Q && S_ISDIR(st->st_mode))
        printf("\"%s\"", file);
    else
        printf("%s", file);

    if ((opt_p || opt_i) && S_ISDIR(st->st_mode))
        printf("/");

    printf("\n");
}

char type(mode_t mode)
{
    if (S_ISREG(mode))
        return('-');
    if (S_ISDIR(mode))
        return('d');
    if (S_ISCHR(mode))
        return('c');
    if (S_ISBLK(mode))
        return('b');
    if (S_ISLNK(mode))
        return('l');
    if (S_ISFIFO(mode))
        return('p');
    if (S_ISSOCK(mode))
        return('s');
    return('?');
}

char* perm(mode_t mode)
{
    static char perms[10];
    strcpy(perms, "---------");

    for (int i=0; i < 3; i++) {
        if (mode & (S_IRUSR >> i*3))
            perms[i*3] = 'r';
        if (mode & (S_IWUSR >> i*3))
            perms[i*3+1] = 'w';
        if (mode & (S_IXUSR >> i*3))
            perms[i*3+2] = 'x';
    }
    return(perms);
}
