#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 1000
#define STACK_MAX_SIZE 1000

typedef struct stack_t {
    int data[STACK_MAX_SIZE];
    int top;
} stack_t;

void syntax_error() {
    printf("syntax error");
    exit(0);
}

void push(stack_t *stack, int value) {
    stack -> top++;
    stack -> data[stack -> top] = value;
}

bool empty(stack_t *stack) {
    return stack -> top < 0;
}

int pop(stack_t *stack) {
    if (empty(stack)) {
        syntax_error();
    }
    return stack -> data[stack -> top--];
}

int peek(stack_t *stack) {
    if (empty(stack)) {
        syntax_error();
    }
    return stack -> data[stack -> top];
}

int priority(char c) {
    switch (c) {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        default:
            return 0;
    }
}

bool isNumber(char c) {
    return c <= '9' && c >= '0';
}

bool isOperation(char c) {
    return c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/';
}

bool isGoodSymbol(char c) {
    return isNumber(c) || isOperation(c);
}

void calculate(stack_t *numbers, char operation) {
    int b = pop(numbers);
    int a = pop(numbers);

    switch (operation) {
        case '+':
            push(numbers, a + b);
            break;
        case '-':
            push(numbers, a - b);
            break;
        case '*':
            push(numbers, a * b);
            break;
        case '/':
            if (b == 0) {
                printf("division by zero");
                exit(0);
            }
            push(numbers, a / b);
            break;
        default:
            syntax_error();
    }
}

int CountNumberFromDigits(char *input, int inputLength, int *index) {
    int n = 0;
    while (*index < inputLength && isNumber(input[*index])) {
        n *= 10;
        n += input[*index] - '0';
        (*index)++;
    }
    (*index)--;
    return n;
}

int ShuntingYardAlgorithm(char *input, int inputLength) {
    stack_t numbers;
    stack_t operations;
    numbers.top = -1;
    operations.top = -1;

    for (int i = 0; i < inputLength; i++) {
        if (input[i] == '(')
            push(&operations, (int) input[i]);
        else if (input[i] == ')') {
            if (i == 0 || input[i - 1] == '(')
                syntax_error();
            char temp;
            while ((temp = (char) pop(&operations)) != '(')
                calculate(&numbers, temp);
        }
        else if (isOperation(input[i])) {
            while (!empty(&operations) && priority((char) peek(&operations)) >= priority(input[i]))
                calculate(&numbers, (char) pop(&operations));
            push(&operations, (int) input[i]);
        }
        else if (isNumber(input[i])) {
            int number = CountNumberFromDigits(input, inputLength, &i);
            push(&numbers, number);
        }
    }

    while (!empty(&operations)) {
        calculate(&numbers, (char) pop(&operations));
    }

    return pop(&numbers);
}

int main() {
    char input[MAX_INPUT_LENGTH];
    int inputLength = 0;

    for (int i = 0; i < MAX_INPUT_LENGTH; i++) {
        char temp;
        if (!scanf("%c", &temp))
            return 0;
        if (temp == '\n')
            break;
        if (!isGoodSymbol(temp)) {
            syntax_error();
        }

        input[inputLength] = temp;
        inputLength++;
    }

    printf("%i", ShuntingYardAlgorithm(input, inputLength));
}
