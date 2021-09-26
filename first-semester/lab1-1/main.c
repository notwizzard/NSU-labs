#include <stdlib.h>
#include <stdio.h>

unsigned char *ReadSample(int *sampleLength) {
    unsigned char *thisSample = malloc(sizeof(unsigned char) * 16);
    unsigned char temporary;
    while (fscanf(stdin, "%c", &temporary)) {
        if (temporary == '\n') break;
        thisSample[*sampleLength] = temporary;
        (*sampleLength)++;
    }
    return thisSample;
}

int SampleHashCounter(const unsigned char *sample, int sampleLength, const int *powsOfThree) {
    int sampleHash = 0;
    for (int i = 0; i < sampleLength; i++) {
        sampleHash += powsOfThree[i] * ((int) sample[i] % 3);
    }
    return sampleHash;
}

int main(void) {

    int powsOfThree[16] = {1};
    for (int i = 1; i < 16; i++) {
        powsOfThree[i] = powsOfThree[i - 1] * 3;
    }

    unsigned char *sample;
    int sampleLength = 0;
    int sampleHash;

    sample = ReadSample(&sampleLength);

    int textLength = sampleLength * 2;
    unsigned char text[textLength];
    int textHash = 0;

    sampleHash = SampleHashCounter(sample, sampleLength, powsOfThree);
    printf("%d ", sampleHash);

    int addedSymbols = 0;

    unsigned int count = fread(text, 1, textLength, stdin);

    if (count < (unsigned) sampleLength) exit(0);

    for (int i = 0; i < sampleLength; i++) {
        textHash += powsOfThree[i] * ((int) text[i] % 3);
    }

    if (sampleHash == textHash) {
        for (int i = 0; i < sampleLength; i++) {
            printf("%i ", i + 1);
            if (text[i] != sample[i]) break;
        }
    }

    int powForNewSymbol = powsOfThree[sampleLength - 1];
    int nowShift;
    int readed = (signed) count - sampleLength;

    do {
        for (int add = sampleLength; add < sampleLength + readed; add++) {
            nowShift = add - sampleLength;
            textHash = (textHash - ((int) text[nowShift] % 3)) / 3 +
                       powForNewSymbol * ((int) text[add] % 3);

            if (sampleHash == textHash) {
                for (int i = 0; i < sampleLength; i++) {
                    printf("%i ", i + addedSymbols + nowShift + 2);
                    if (text[i + nowShift + 1] != sample[i]) break;
                }
            }
        }
        addedSymbols += sampleLength;

        for (int i = 0; i < sampleLength; i++) {
            text[i] = text[sampleLength + i];
        }

    }
    while ((readed = (signed) fread(&text[sampleLength], 1, sampleLength, stdin)));

}
