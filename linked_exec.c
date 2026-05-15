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

    if (argc < 2) {
        printf("Usage: %s prog1 prog2 ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        Node *n = malloc(sizeof(Node));
        strcpy(n->name, argv[i]);
        n->next = NULL;
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
    }

    printf("Linked list:\n");
    Node *print_cur = head;
    while (print_cur) {
        printf("- %s\n", print_cur->name);
        print_cur = print_cur->next;
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
