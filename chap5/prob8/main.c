#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    char savedText[10][100];
    char buf;
    int row = 0, col = 0;
    char input[100];

    if (argc < 2) {
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        return 1;
    }
    printf("File read success\n");

    while (read(fd, &buf, 1) > 0) {
        if (buf == '\n') {
            savedText[row][col] = '\0';
            row++;
            col = 0;
        } else {
            savedText[row][col] = buf;
            col++;
        }
    }

    printf("Total Line : %d\n", row);
    printf("You can choose 1 ~ %d Line\n", row);
    printf("Pls 'Enter' the line to select : ");
    scanf("%s", input);

    if (strcmp(input, "*") == 0) {
        for (int i = 0; i < row; i++) {
            printf("%s\n", savedText[i]);
        }
    } else if (strchr(input, '-') != NULL) {
        int start, end;
        sscanf(input, "%d-%d", &start, &end);
        for (int i = start; i <= end; i++) {
            if (i >= 1 && i <= row) {
                printf("%s\n", savedText[i - 1]);
            }
        }
    } else if (strchr(input, ',') != NULL) {
        char *token = strtok(input, ",");
        while (token != NULL) {
            int line = atoi(token);
            if (line >= 1 && line <= row) {
                printf("%s\n", savedText[line - 1]);
            }
            token = strtok(NULL, ",");
        }
    } else {
        int line = atoi(input);
        if (line >= 1 && line <= row) {
            printf("%s\n", savedText[line - 1]);
        }
    }

    close(fd);
    return 0;
}
