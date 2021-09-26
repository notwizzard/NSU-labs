#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const char alphabet[10] = "0123456789";

void ExitWithBadInput() {
    printf("bad input");
    exit(0);
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

int GeneratePermutation(int *number, int length) {
    for (int i = length - 2; i >= 0; i--) {
        if (number[i] < number[i + 1]) {
            int minIndex = i + 1;
            for (int j = length - 1; j > i; j--) {
                if (number[j] < number[minIndex] && number[j] > number[i])
                    minIndex = j;
            }

            swap(&number[i], &number[minIndex]);

            for (int k = i + 1; k < length; k++) {
                for (int p = length - 1; p > k; p--) {
                    if (number[p - 1] > number[p])
                        swap(&number[p - 1], &number[p]);
                }
            }

            return 1;
        }
    }
    return 0;
}

int main(void) {

    bool mapOfNumbers[10] = {false};
    int numberOfPermutations;
    int inputLength = -1;
    int number[10];

    char temp;
    while(scanf("%c", &temp)) {
        if (temp == '\n') break;

        bool isGoodTemp = false;
        for (int i = 0 ; i < 10; i++) {
            if (temp == alphabet[i]) {
                isGoodTemp = true;
                break;
            }
        }

        if (!isGoodTemp) ExitWithBadInput();

        inputLength++;
        if (temp < '0' || temp > '9' || mapOfNumbers[temp - '0'] || inputLength >= 10)
            ExitWithBadInput();

        mapOfNumbers[temp - '0'] = true;
        number[inputLength] = temp - '0';
    }

    inputLength++;
    if (!scanf("%d", &numberOfPermutations)) return 0;

    for (int i = 0; i < numberOfPermutations; i++) {
        if (!GeneratePermutation(number, inputLength))
            return 0;
        else {
            for (int j = 0; j < inputLength; j ++)
                printf("%i", number[j]);
            printf("\n");
        }
    }
}
