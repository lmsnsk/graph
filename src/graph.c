#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define ROW 25
#define COL 80

void drawing();
float func(float x);
char* get_number_string(char* str, int* pos);
char* char_to_string(char c);
int is_operator(char c);
int check_priority(char c);

int main() {
    int a = 1;
    // drawing();
    // char* x = get_number_string("e76842w23wer", &a);
    // printf("%s", x);
    // free(x);
    return 0;
}

float func(float x) { return sin(x); }

// void drawing() {
//     int y;
//     y = (ROW - 1) / 2;
//     int t = 12;

//     for (int i = y; i >= -y; i--) {
//         for (int j = 0; j < COL; j++) {
//             float k = i / 12.0;
//             float l = j / (float)(COL - 1) * 4 * M_PI;
//             if (round(k * t) == round(func(l) * t)) {
//                 j == COL - 1 ? printf("*\n") : printf("*");
//             } else {
//                 j == COL - 1 ? printf(".\n") : printf(".");
//             }
//         }
//     }
// }

char* to_postfix(char* infix_str) {
    char* postfix_str = "";
    Node* operators = init('z');
    for (int i = 0; i < strlen(postfix_str); i++) {
        char c = infix_str[i];

        if ((int)c > 47 && (int)c < 58) {
            char* tmp_str = get_number_string(postfix_str, i);
            strcat(postfix_str, tmp_str);
            strcat(postfix_str, " ");
            free(tmp_str);

        } else if (c == '(') {
            push(operators, c);

        } else if (c == ')') {
            Node* tmp = operators;
            while (tmp->value != '(' && tmp->value != 'z') {  //проверить
                char* s = char_to_string(pop(operators));
                strcat(postfix_str, s);  // перевести в строку
                tmp = tmp->next;
                free(s);
            }
            pop(operators);

        } else if (is_operator(c)) {  //проверка списка операторов
            char op = c;
            if (op == '-' && (i == 0 || (i > 0 && is_operator(infix_str[i - 1]) || i > 0 && op == '(')))
                op = '~';  //проверка списка операторов
            Node* tmp = operators;
            int count = 1;
            while (tmp->next != NULL) count++;
            while (count > 1 && (check_priority(operators->value) >= check_priority(op))) {  // проверить
                char* s = char_to_string(pop(operators));
                strcat(postfix_str, s);
                tmp = tmp->next;
                free(s);
            }
        }
    }

    Node* tmp = operators;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    while (tmp->value != 'z') {
        char* s = char_to_string(pop(operators));
        strcat(postfix_str, s);
    }

    return postfix_str;
}

char* get_number_string(char* str, int* pos) {
    char* str_number = malloc(100 * sizeof(char));
    str_number[0] = 0;
    for (; *pos < (int)strlen(str); *pos += 1) {
        char number = str[*pos];
        char A[2] = {number, 0};
        if ((int)number > 47 && (int)number < 58) {
            strcat(str_number, A);
        } else {
            *pos -= 1;
            break;
        }
    }
    return str_number;
}

char* char_to_string(char c) {
    char* str_number = malloc(100 * sizeof(char));
    str_number[0] = c;
    str_number[1] = 0;
    return str_number;
}

int is_operator(char c) {
    int x = 0;
    char* op = "(+-*/sctnlq~";
    for (int i = 0; i < (int)strlen(op); i++) {
        if (c == op[i]) x = 1;
    }
    return x;
}

int check_priority(char c) {
    int priority;
    if (c == '(') priority = 0;
    if (c == '+') priority = 1;
    if (c == '-') priority = 1;
    if (c == '*') priority = 2;
    if (c == '/') priority = 2;
    if (c == 's') priority = 3;
    if (c == 'c') priority = 3;
    if (c == 't') priority = 3;
    if (c == 'n') priority = 3;
    if (c == 'l') priority = 3;
    if (c == 'q') priority = 4;
    if (c == '~') priority = 5;
    return priority;
}