#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/unistd.h>

#define SMALL_ASCII_SIZE 127

struct smp {
    char charValue;
    int intValue;
};

FILE *inputFile;

struct smp *ReadSample(int *sampleLength) {
    static struct smp thisSample[16];
    char temporary;
    while (fscanf(inputFile, "%c", &temporary)) {
        if (temporary == '\n') break;
        thisSample[*sampleLength].charValue = temporary;
        thisSample[*sampleLength].intValue = (int) temporary + SMALL_ASCII_SIZE;
        (*sampleLength) ++;
    }
    return thisSample;
}

void ShiftFiller(struct smp *sample, int sampleLength, int *mapOfShifts) {
    int lastTranslation = sampleLength;

    for (int i = sampleLength - 2; i >= 0; i --) {
        if (mapOfShifts[sample[i].intValue]) continue;
        int translation = sampleLength - i - 1;
        if (sample[i].charValue == sample[sampleLength - 1].charValue) lastTranslation = translation;
        mapOfShifts[sample[i].intValue] = translation;
    }
    mapOfShifts[sample[sampleLength - 1].intValue] = lastTranslation;
}

int GetTranslation(int index, int mapOfShifts[], const char *text, int sampleLength) {
    return mapOfShifts[(int) text[index] + SMALL_ASCII_SIZE] ? mapOfShifts[(int) text[index] + SMALL_ASCII_SIZE] : sampleLength;
}

int main() {
    inputFile = fopen("in.txt", "r");

    int sampleLength = 0;
    struct smp *sample = ReadSample(&sampleLength);
    char *text = malloc(sizeof(char) * sampleLength);

    int mapOfShifts[256] = {0};
    ShiftFiller(sample, sampleLength, mapOfShifts);

    int index = sampleLength - 1;
    int readTextLength = 0;
    int textToReadLength = sampleLength;
    while (fread(&text[sampleLength - textToReadLength], 1, textToReadLength, inputFile) == (unsigned) textToReadLength) {
        readTextLength += textToReadLength;
        bool equals = true;
        while (index >= 0) {
            printf("%d ", readTextLength - sampleLength + index + 1);
            if (sample[index].charValue == text[index]) index --;
            else {
                equals = false;
                break;
            }
        }

        if (equals) {
            textToReadLength = sampleLength;
        }
        else {
            if (index < sampleLength - 1) textToReadLength = GetTranslation(sampleLength - 1, mapOfShifts, text, sampleLength);
            else textToReadLength = GetTranslation(index, mapOfShifts, text, sampleLength);
        }

        index = sampleLength - 1;

        for (int i = 0; i < sampleLength - textToReadLength; i ++) {
            text[i] = text[i + textToReadLength];
        }
    }

    free(text);
    fclose(inputFile);
}
