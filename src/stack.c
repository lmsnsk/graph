#include "stack.h"

#include <stdlib.h>

Node* init(char num) {
    Node* tmp;
    tmp = (Node*)malloc(sizeof(Node));
    tmp->value = num;
    tmp->next = NULL;
    return tmp;
}

Node* push(Node* root, char num) {
    Node* tmp = (Node*)malloc(sizeof(Node));
    Node* p = root;
    int n = 1;
    while (n) {
        if (p->next == NULL) {
            n = 0;
        } else
            p = p->next;
    }
    p->next = tmp;
    tmp->value = num;
    tmp->next = NULL;
    return tmp;
}

char pop(Node* root) {
    Node* tmp = root;
    Node* p;
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    Node* t = tmp;
    p = tmp->next;
    char z = tmp->value;
    tmp->next = NULL;
    free(p);
    return z;
}

void destroy(Node* root) {
    while (root != NULL) {
        Node* p = root;
        root = root->next;
        free(p);
    }
}
