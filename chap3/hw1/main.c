#include <stdio.h>
#include <string.h>
#include "copy.h"

int main() {
    char lines[5][MAXLINE];
    char temp[MAXLINE];
    int count = 0;

    while (count < 5 && fgets(lines[count], MAXLINE, stdin) != NULL) {
        lines[count][strcspn(lines[count], "\n")] = '\0';
        count++;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (strlen(lines[j]) < strlen(lines[j + 1])) {
                copy(lines[j], temp);
                copy(lines[j + 1], lines[j]);
                copy(temp, lines[j + 1]);
            }
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", lines[i]);
    }

    return 0;
}
