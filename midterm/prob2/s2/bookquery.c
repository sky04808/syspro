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
	printf("%-5s %-10s  %-4s %-10s %-15s %-5s\n", "id", "bookname", "author", "year", "numofborrow", "borrow");

	while (fscanf(fp, "%d %s %s %d %d %s", &rec.id, rec.bookname, rec.author, &rec.year, &rec.numofborrow, rec.borrow) == 6) {
		printf(" %-5d %-10s %-3s %-10d %-15d %-5s\n", rec.id, rec.bookname, rec.author, rec.year, rec.numofborrow, rec.borrow);
	}

		fclose(fp);
		return 0;
}
