#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3
#define BUFFER_SIZE 100

int main() {
    int pipefd[NUM_CHILDREN][2];
    pid_t pid[NUM_CHILDREN];
    char buffer[BUFFER_SIZE];
    const char *message = "Hello from parent";

    // Create pipes and fork child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid[i] = fork();

        if (pid[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid[i] == 0) { // Child process
            close(pipefd[i][1]); // Close write end of the first pipe
            read(pipefd[i][0], buffer, BUFFER_SIZE);
            printf("Child %d received message: %s\n", i, buffer);
            close(pipefd[i][0]); // Close read end of the first pipe

            int pipefd_response[2];
            if (pipe(pipefd_response) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            pid_t response_pid = fork();
            if (response_pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (response_pid == 0) {
                close(pipefd_response[0]); // Close read end of the response pipe
                snprintf(buffer, BUFFER_SIZE, "Hello from child %d", i);
                write(pipefd_response[1], buffer, strlen(buffer) + 1);
                close(pipefd_response[1]); // Close write end of the response pipe
                exit(EXIT_SUCCESS);
            } else {
                close(pipefd_response[1]); // Close write end of the response pipe
                wait(NULL); // Wait for the response child to send the message
                read(pipefd_response[0], buffer, BUFFER_SIZE);
                close(pipefd_response[0]); // Close read end of the response pipe
                printf("Parent received message: %s\n", buffer);
                exit(EXIT_SUCCESS);
            }
        } else { // Parent process
            close(pipefd[i][0]); // Close read end of the first pipe
            write(pipefd[i][1], message, strlen(message) + 1);
            close(pipefd[i][1]); // Close write end of the first pipe
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_CHILDREN; i++) {
        waitpid(pid[i], NULL, 0);
    }

    return 0;
}
