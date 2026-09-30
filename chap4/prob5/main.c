#include <stdio.h>
#include "student.h"

int main(int argc, char *argv[]) {
    struct student rec;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "How to use: %s FileName\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "File Open Error\n");
        return 2;
    }
    printf("%-10s %-8s %-4s\n", "학번", "이름", "점수");

    while (fscanf(fp, "%d %s %d", &rec.id, rec.name, &rec.score) == 3) {
        printf("%-10d %-8s %-4d\n", rec.id, rec.name, rec.score);
    }

    fclose(fp);
    return 0;
}
