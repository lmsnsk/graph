#ifndef STACK
#define STACK

typedef struct node {
    char value;
    struct node* next;
} Node;

Node* init(char num);
Node* push(Node* root, char num);
char pop(Node* root);
void destroy(Node* root);

#endif
