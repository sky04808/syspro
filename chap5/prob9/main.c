#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    char savedText[10][100];
    char buf;
    int row = 0, col = 0;

    if (argc < 2) {
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        return 1;
    }

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

    for (int i = row - 1; i >= 0; i--) {
        printf("%s\n", savedText[i]);
    }

    close(fd);
    return 0;
}
