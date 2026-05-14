#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s prog1 prog2 ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < 1000; j++) {
            int pid = fork();
            if (pid == 0) {
                execlp(argv[i], argv[i], NULL);
                perror("execlp");
                exit(1);
            }
        }
    }

    while (wait(NULL) > 0);

    return 0;
}
