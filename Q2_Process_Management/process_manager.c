#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define NUM_CHILDREN 5
#define TIMEOUT_SECONDS 3

pid_t children[NUM_CHILDREN];

void reap_zombies(int sig) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[Parent] Reaped child PID %d (avoided zombie)\n", pid);
        fflush(stdout);
    }
}

int main() {
    signal(SIGCHLD, reap_zombies);

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            printf("[Child %d] PID %d started\n", i, getpid());
            fflush(stdout);
            if (i % 2 == 0) {
                sleep(1);
                printf("[Child %d] PID %d finished normally\n", i, getpid());
                fflush(stdout);
                exit(0);
            } else {
                while (1) { }
            }
        } else {
            children[i] = pid;
        }
    }

    sleep(TIMEOUT_SECONDS);

    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (kill(children[i], 0) == 0) {
            printf("[Parent] Child PID %d unresponsive. Sending SIGTERM.\n", children[i]);
            fflush(stdout);
            kill(children[i], SIGTERM);
            sleep(1);
            if (kill(children[i], 0) == 0) {
                printf("[Parent] Child PID %d still alive. Sending SIGKILL.\n", children[i]);
                fflush(stdout);
                kill(children[i], SIGKILL);
            }
        }
    }

    while (waitpid(-1, NULL, WNOHANG) > 0);

    printf("[Parent] All children handled. Exiting.\n");
    return 0;
}
