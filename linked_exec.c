#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct Node {
    char name[256];
    struct Node *next;
} Node;

int main(int argc, char *argv[]) {
    Node *head = NULL, *tail = NULL;
    char buf[256];

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            Node *n = malloc(sizeof(Node));
            strcpy(n->name, argv[i]);
            n->next = NULL;
            if (!head) head = tail = n;
            else { tail->next = n; tail = n; }
        }
    } else {
        printf("Enter app names (one per line, Ctrl+D to stop):\n");

        while (fgets(buf, sizeof(buf), stdin)) {
            buf[strcspn(buf, "\n")] = 0;
            Node *n = malloc(sizeof(Node));
            strcpy(n->name, buf);
            n->next = NULL;
            if (!head) head = tail = n;
            else { tail->next = n; tail = n; }
        }
    }

    Node *cur = head;
    while (cur) {
        pid_t pid = fork();
        if (pid == 0) {
            execlp(cur->name, cur->name, NULL);
            perror("execlp");
            exit(1);
        }
        cur = cur->next;
    }

    while (wait(NULL) > 0);

    cur = head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }

    return 0;
}
