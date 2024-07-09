#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void parse_input(char *line, char **args) {
    while (*line != '\0') {
        while (*line == ' ' || *line == '\t' || *line == '\n') {
            *line++ = '\0';
        }
        *args++ = line;
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
            line++;
        }
    }
    *args = '\0';
}

void execute_command(char **args) {
    // Check for built-in commands
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        if (execvp(args[0], args) < 0) {
            perror("Exec failed");
            exit(1);
        }
    } else {
        wait(NULL);
    }
}

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("simple_shell> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        if (line[0] == '\n') {
            continue;
        }
        line[strlen(line) - 1] = '\0';  // Remove the newline character
        parse_input(line, args);

        if (strcmp(args[0], "exit") == 0) {
            break;
        }
        execute_command(args);
    }

    return 0;
}
