#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define NUM_CHILDREN 3

int main() {
    pid_t pids[NUM_CHILDREN];

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            printf("Child %d started, PID: %d\n", i, getpid());
            sleep(5 + i * 3);  // Simulate work; later children take longer
            printf("Child %d (PID %d) finished work.\n", i, getpid());
            exit(0);
        } else {
            // Parent process
            pids[i] = pid;
        }
    }

    // Parent monitors children
    int status;
    pid_t finished_pid;
    int remaining = NUM_CHILDREN;
    time_t start_time = time(NULL);
    const int TIMEOUT = 10;

    while (remaining > 0) {
        finished_pid = waitpid(-1, &status, WNOHANG);

        if (finished_pid > 0) {
            if (WIFEXITED(status)) {
                printf("Parent: Child PID %d exited normally with status %d\n",
                       finished_pid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Parent: Child PID %d was killed by signal %d\n",
                       finished_pid, WTERMSIG(status));
            }
            remaining--;
        } else {
            // Check for timeout - kill unresponsive children
            if (time(NULL) - start_time > TIMEOUT) {
                for (int i = 0; i < NUM_CHILDREN; i++) {
                    if (pids[i] > 0) {
                        printf("Parent: Terminating unresponsive child PID %d\n", pids[i]);
                        kill(pids[i], SIGTERM);
                    }
                }
                sleep(1);
                for (int i = 0; i < NUM_CHILDREN; i++) {
                    if (pids[i] > 0) {
                        kill(pids[i], SIGKILL);
                    }
                }
                break;
            }
            sleep(1);
        }
    }

    // Reap any remaining zombies
    while (waitpid(-1, &status, WNOHANG) > 0);

    printf("Parent: All children processed. Exiting.\n");
    return 0;
}
