#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARG  64

void run_command(char *cmd)
{
    static char buf[MAXLINE * 3];
    char *args[MAXARG];
    char *saveptr, *token;
    char *infile = NULL, *outfile = NULL;
    int argc = 0, background = 0, status, fd;
    size_t len, i, j = 0;
    pid_t pid, child;

    len = strlen(cmd);
    while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
        cmd[--len] = '\0';
    if (len > 0 && cmd[len - 1] == '&') {
        background = 1;
        cmd[--len] = '\0';
    }

    for (i = 0; i < len && j < sizeof(buf) - 4; i++) {
        if (cmd[i] == '<' || cmd[i] == '>') {
            buf[j++] = ' ';
            buf[j++] = cmd[i];
            buf[j++] = ' ';
        } else {
            buf[j++] = cmd[i];
        }
    }
    buf[j] = '\0';

    token = strtok_r(buf, " \t", &saveptr);
    while (token != NULL && argc < MAXARG - 1) {
        if (strcmp(token, ">") == 0)
            outfile = strtok_r(NULL, " \t", &saveptr);
        else if (strcmp(token, "<") == 0)
            infile = strtok_r(NULL, " \t", &saveptr);
        else
            args[argc++] = token;
        token = strtok_r(NULL, " \t", &saveptr);
    }
    args[argc] = NULL;

    if (argc == 0)
        return;

    if (strcmp(args[0], "exit") == 0) {
        printf("[%d] Parent process end\n", getpid());
        printf("Exit\n");
        exit(0);
    }

    printf("[%d] Parent process start\n", getpid());
    fflush(stdout);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        printf("[%d] child process start\n", getpid());
        fflush(stdout);
        if (infile != NULL) {
            fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror(infile);
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (outfile != NULL) {
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror(outfile);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0], &args[0]);
        exit(127);
    }

    (void)background;
    child = waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
        printf("Parent process end\n");
        printf("Exit\n");
        exit(0);
    }
    printf("[%d] child process end %d\n", getpid(), child);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        printf("SUCCESS\n");
    else
        printf("FAIL\n");
    printf("\n");
}

int main(void)
{
    char line[MAXLINE];
    char *saveptr, *cmd;
    pid_t child;
    int status;

    while (1) {
        while ((child = waitpid(-1, &status, WNOHANG)) > 0)
            printf("[%d] background child process end %d\n", getpid(), child);

        printf("Pls input cmd : ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }
        line[strcspn(line, "\n")] = '\0';

        cmd = strtok_r(line, ";", &saveptr);
        while (cmd != NULL) {
            run_command(cmd);
            cmd = strtok_r(NULL, ";", &saveptr);
        }
    }
    return 0;
}
