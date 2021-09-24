#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 16
#define MIN 2
#define PRECISION 100000

const char badInput[] = "bad input";
const char dot = '.';
const char alphabet[] = "0123456789abcdefABCDEF.";

int from;
int to;
char rawInputNumber[13];
int *arrayOfNumbers = NULL;
char *arrayOfTranslatedIntegerPart = NULL;
char *arrayOfTranslatedFractionalPart = NULL;
char *reversedArrayOfTranslatedIntegerPart = NULL;
int dotPosition;
double decimalFractionalNumber = 0;
long long decimalIntegerNumber = 0;
long long integerLength = 0;
int fractionalLength = 0;

void read();
void checkInput();
void exitProgramWithBadInput();
void checkNotation();
int charToInt(char);
int findDotPosition();
void getDecimalNumbers();
double negativePow(int, int);
void getTranslatedIntegerPart();
void getTranslatedFractionalPart();
char intToChar(long long);
long long longPow(long long, int);
void printAnswer();

int main() {
    read();
    dotPosition = findDotPosition();
    checkInput();
    getDecimalNumbers();
    getTranslatedIntegerPart();
    getTranslatedFractionalPart();
    printAnswer();
}

void read() {
    if(!scanf("%i %i", &from, &to)) exit(0);
    if(!scanf("%13s", &*rawInputNumber)) exit(0);
    arrayOfNumbers = malloc(sizeof(int) * strlen(rawInputNumber));
}

void checkInput() {
    if (from > MAX || from < MIN || to > MAX || to < MIN) {
        exitProgramWithBadInput();
    }
    for (unsigned int i = 0; i < strlen(rawInputNumber); i ++) {
        int checker = 0;
        for (unsigned int j = 0; j < strlen(alphabet); j ++) {
            if (rawInputNumber[i] == alphabet[j]) checker = 1;
        }
        if (!checker) exitProgramWithBadInput();
    }

    checkNotation();
}

void checkNotation() {
    for (unsigned int i = 0; i < strlen(rawInputNumber); i ++) {
        if ((int) i == dotPosition) continue;
        *(arrayOfNumbers + i) = charToInt(rawInputNumber[i]);
        if (arrayOfNumbers[i] >= from) exitProgramWithBadInput();
    }
}

int charToInt(char c) {
    switch (c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        case 'a': return 10;
        case 'b': return 11;
        case 'c': return 12;
        case 'd': return 13;
        case 'e': return 14;
        case 'f': return 15;
        default: return 0;
    }
}

char intToChar (long long c) {
    switch (c) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'a';
        case 11: return 'b';
        case 12: return 'c';
        case 13: return 'd';
        case 14: return 'e';
        case 15: return 'f';
        default: return '0';
    }
}

void exitProgramWithBadInput() {
    printf(badInput);
    exit(0);
}

int findDotPosition() {
    int dotCounter = 0;
    for (unsigned int i = 0; i < strlen(rawInputNumber); i ++) {
        if (rawInputNumber[i] == dot) dotCounter ++;
    }
    if (dotCounter > 1) exitProgramWithBadInput();
    for (unsigned int i = 0; i < strlen(rawInputNumber); i ++) {
        if (rawInputNumber[i] == dot) {
            if (i == strlen(rawInputNumber) - 1 || i == 0) exitProgramWithBadInput();
            return (int) i;
        }
    }
    return (int) strlen(rawInputNumber);
}

void getDecimalNumbers() {
    int integerPartLength = dotPosition - 1;
    for (int i = 0; i < dotPosition; i ++) {
        decimalIntegerNumber += arrayOfNumbers[i] * longPow((long long) from, (integerPartLength - i));
    }
    integerPartLength ++;
    for (unsigned int i = dotPosition + 1; i < strlen(rawInputNumber); i ++) {
        double p = negativePow(from, abs(integerPartLength - (int) i));
        p *= (double) arrayOfNumbers[i] * PRECISION;
        decimalFractionalNumber += p;
    }
}

long long longPow (long long a, int degree) {
    long long o = 1;
    for (int i = 0; i < degree; i ++) {
        o *= a;
    }
    return o;
}

double negativePow(int start, int degree) {
    double k = 1;
    double s = (double) start;
    k /= pow(s, degree);
    return k;
}

void getTranslatedIntegerPart() {
    long long k = decimalIntegerNumber;
    int i = 0;
    if (!k) {
        reversedArrayOfTranslatedIntegerPart = (char *) realloc(reversedArrayOfTranslatedIntegerPart, sizeof(char) * (i + 1));
        if (reversedArrayOfTranslatedIntegerPart == NULL) exit(0);
        reversedArrayOfTranslatedIntegerPart[i] = intToChar((k % to));
        i++;
    }
    while (k) {
        reversedArrayOfTranslatedIntegerPart = (char *) realloc(reversedArrayOfTranslatedIntegerPart, sizeof(char) * (i + 1));
        if (reversedArrayOfTranslatedIntegerPart == NULL) exit(0);
        reversedArrayOfTranslatedIntegerPart[i] = intToChar((k % to));
        i++;
        k /= to;
    }
    arrayOfTranslatedIntegerPart = malloc(sizeof(char) * i);
    for (int j = 0; j < i; j ++) {
        arrayOfTranslatedIntegerPart[i - j - 1] = reversedArrayOfTranslatedIntegerPart[j];
    }
    integerLength = i;
}

void getTranslatedFractionalPart () {
    double k = decimalFractionalNumber;
    int counter = 0;
    while (k != 0 && counter < 12) {
        k *= to;
        int o = (int) (k / PRECISION);
        k -= o * PRECISION;
        arrayOfTranslatedFractionalPart = (char *) realloc(arrayOfTranslatedFractionalPart, sizeof(char) * (counter + 1));
        if (arrayOfTranslatedFractionalPart == NULL) exit(0);
        arrayOfTranslatedFractionalPart[counter] = intToChar(o);
        counter ++;
    }
    fractionalLength = counter;
}

void printAnswer() {
    for (int i = 0; i < integerLength; i ++) {
        printf("%c", arrayOfTranslatedIntegerPart[i]);
    }
    if (fractionalLength) printf(".");
    for (int i = 0; i < fractionalLength; i ++) {
        printf("%c", arrayOfTranslatedFractionalPart[i]);
    }
    printf("\n");
}
