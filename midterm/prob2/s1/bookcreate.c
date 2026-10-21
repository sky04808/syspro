#include <stdio.h>
#include "student.h"

int main(int argc, char *argv[])
{
	struct student rec;
    FILE *fp;

	if (argc != 2) {
        fprintf(stderr, "How to use: %s FileName\n", argv[0]);
        return 1;
    }

	fp = fopen(argv[1], "w");
    if (fp == NULL) {
        fprintf(stderr, "File Open Error\n");
        return 2;
    }

	printf("%-9s %-7s %-4s %-9s %-7s %-7s\n", "id", "bookname", "author", "year", "numofborrow", "borrow");

	while (scanf("%d %s %s %d %d %s", &rec.id, rec.bookname, rec.author, &rec.year, &rec.numofborrow, rec.borrow) == 6) {
		fprintf(fp, "%d %s %s %d %d %s\n", rec.id, rec.bookname, rec.author, rec.year, rec.numofborrow, rec.borrow);
	}

	fclose(fp);
	return 0;
}
