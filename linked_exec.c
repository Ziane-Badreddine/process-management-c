#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct Node {
    char name[256];
    pid_t pid;
    struct Node *next;
} Node;

int main(int argc, char *argv[]) {
    Node *head = NULL, *tail = NULL;

    if (argc < 2) {
        printf("Usage: %s prog1 prog2 ...\n", argv[0]);
        return 1;
    }

    int i = 1;
    while (i < argc) {
        Node *n = malloc(sizeof(Node));
        strcpy(n->name, argv[i]);
        n->pid = -1;
        n->next = NULL;
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
        i++;
    }

    Node *cur = head;
    while (cur) {
        pid_t pid = fork();
        if (pid == 0) {
            execlp(cur->name, cur->name, NULL);
            perror("execlp");
            exit(1);
        }
        cur->pid = pid;
        cur = cur->next;
    }

    cur = head;
    while (cur) {
        printf("%s,%d%s", cur->name, cur->pid, cur->next ? " -> " : " -> NULL\n");
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
