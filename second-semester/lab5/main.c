#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ASCII_SIZE 256
#define BITS_IN_LONG_LONG 63
#define BUFFER_SIZE 1000

FILE *in;
FILE *out;

typedef struct codeTreeNode codeTreeNode;
struct codeTreeNode {
    int weight;
    unsigned char character;
    bool empty;
    codeTreeNode *leftChild;
    codeTreeNode *rightChild;
};

void swap(codeTreeNode *a, codeTreeNode *b) {
    codeTreeNode c = *a;
    *a = *b;
    *b = c;
}

typedef struct codeTreeNodeList_t {
    codeTreeNode data[ASCII_SIZE];
    int top;
} codeTreeNodeList_t;

void syntax_error() {
    //printf("syntax error");
    exit(0);
}

void add(codeTreeNodeList_t *list, codeTreeNode value) {
    list->top++;
    list->data[list->top] = value;
}

bool empty(codeTreeNodeList_t *list) {
    return list->top < 0;
}

codeTreeNode pop(codeTreeNodeList_t *list) {
    if (empty(list)) {
        syntax_error();
    }
    return list->data[list->top--];
}

codeTreeNode peek(codeTreeNodeList_t *list) {
    if (empty(list)) {
        syntax_error();
    }
    return list->data[list->top];
}

void sort(codeTreeNodeList_t *list) {
    for (int i = 0; i <= list->top; i++) {
        for (int j = 0; j < list->top; j++) {
            if (list->data[j].weight < list->data[j + 1].weight) {
                swap(&list->data[j], &list->data[j + 1]);
            }
        }
    }
}

int NextCharacters(unsigned char buffer[BUFFER_SIZE]) {
    return fread(buffer, 1, BUFFER_SIZE, in);
}

void FreeTreeDFS(codeTreeNode *root) {
    if (!root)
        return;
    FreeTreeDFS(root->rightChild);
    FreeTreeDFS(root->leftChild);
    free(root);
}

codeTreeNodeList_t GenerateTreeFromTable(const int frequencyTable[ASCII_SIZE], const unsigned char characters[ASCII_SIZE], int charactersNumber) {
    codeTreeNodeList_t listOfNodes;
    listOfNodes.top = -1;

    for (int i = 0; i < charactersNumber; i++) {
        codeTreeNode node = (codeTreeNode) {frequencyTable[(int) characters[i]], characters[i], false, NULL, NULL};
        add(&listOfNodes, node);
    }

    if (charactersNumber == 1) {
        codeTreeNode emptyNode = (codeTreeNode) {0, 0, false, NULL, NULL};
        add(&listOfNodes, emptyNode);
    }

    return listOfNodes;
}

void RecalculateTree(codeTreeNodeList_t *listOfNodes) {
    sort(listOfNodes);

    codeTreeNode *left = malloc(sizeof(codeTreeNode));
    codeTreeNode *right = malloc(sizeof(codeTreeNode));
    *left = pop(listOfNodes);
    *right = pop(listOfNodes);
    codeTreeNode newNode = (codeTreeNode) {left->weight + right->weight, 0, true, left, right};

    add(listOfNodes, newNode);
}

unsigned long long AddZeroToCode(unsigned long long code, char codeLength) {
    code &= ~(1llu << (BITS_IN_LONG_LONG - codeLength));
    return code;
}

unsigned long long AddOneToCode(unsigned long long code, char codeLength) {
    code |=  (1llu << (BITS_IN_LONG_LONG - codeLength));
    return code;
}

void GetCharacterCode(unsigned long long rootCode, codeTreeNode node, char codeLength, char mapOfCodeLength[ASCII_SIZE], unsigned long long mapOfCodes[ASCII_SIZE], char bitToWrite, unsigned short *bitLengthOfEncodedTree) {
    if (bitToWrite == 0) {
        rootCode = AddZeroToCode(rootCode, codeLength);
        codeLength++;
    }
    if (bitToWrite == 1) {
        rootCode = AddOneToCode(rootCode, codeLength);
        codeLength++;
    }

    if (!node.empty) {
        if (codeLength == 0) {
            rootCode = AddZeroToCode(rootCode, codeLength);
            codeLength++;
        }
        mapOfCodes[(int) node.character] = rootCode;
        mapOfCodeLength[(int) node.character] = codeLength;
        (*bitLengthOfEncodedTree) += 8;
    }
    else {
        if (node.leftChild != NULL) {
            GetCharacterCode(rootCode, *node.leftChild, codeLength, mapOfCodeLength, mapOfCodes, 0, bitLengthOfEncodedTree);
        }
        if (node.rightChild != NULL) {
            GetCharacterCode(rootCode, *node.rightChild, codeLength, mapOfCodeLength, mapOfCodes, 1, bitLengthOfEncodedTree);
        }
    }

    (*bitLengthOfEncodedTree)++;
}

void GetAllCodes(codeTreeNodeList_t *listOfNodes, unsigned long long mapOfCodes[ASCII_SIZE], char mapOfCodeLength[ASCII_SIZE], unsigned short *bitLengthOfEncodedTree) {
    long long code = 0;
    GetCharacterCode(code, listOfNodes->data[0], 0, mapOfCodeLength, mapOfCodes, -1, bitLengthOfEncodedTree);
}

int CountBitLengthOfEncodedText(unsigned char buffer[BUFFER_SIZE], const char mapOfCodeLength[ASCII_SIZE]) {
    int sum = 0;
    int readLength = 0;
    while ((readLength = NextCharacters(buffer))) {
        for (int i = 0; i < readLength; i++)
        sum += mapOfCodeLength[(int) buffer[i]];
    }
    fseek(in, 1, SEEK_SET);

    return sum;
}

void AddBitToByteAndWrite(char *bitPosition, unsigned char *byte, int bit) {
    if (bit == 1) {
        *byte |= 1 << (7 - *bitPosition);
    }
    if (bit == 0) {
        *byte &= ~(1 << (7 - *bitPosition));
    }

    (*bitPosition)++;
    if (*bitPosition == 8) {
        fprintf(out, "%c", *byte);
        (*bitPosition) = 0;
    }
}

void WriteBitLengthOfEncodedText(int length, char *currentBitNumber, unsigned char *currentByte) {
    for (int shift = 24; shift < 32; shift++) {
        if (length & (1u << (31 - shift))) {
            AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
        }
        else {
            AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
        }
    }
}

void WriteEncodedTree(codeTreeNode node, char *currentBitNumber, unsigned char *currentByte) {
    if (!node.empty) {
        AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
        for (int shift = 0; shift < 8; shift++) {
            if (node.character & (1 << (7 - shift))) {
                AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
            }
            else {
                AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
            }
        }
    }
    else {
        AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
        if (node.leftChild != NULL) {
            WriteEncodedTree(*node.leftChild, currentBitNumber, currentByte);
        }
        if (node.rightChild != NULL) {
            WriteEncodedTree(*node.rightChild, currentBitNumber, currentByte);
        }
    }
}

void WriteEncodedText(unsigned char buffer[BUFFER_SIZE], const unsigned long long mapOfCodes[ASCII_SIZE], const char mapOfCodeLength[ASCII_SIZE], char *currentBitNumber, unsigned char *currentByte) {
    int readLength = 0;
    while ((readLength = NextCharacters(buffer))) {
        for (int i = 0; i < readLength; i++) {
            for (int shift = 0; shift < mapOfCodeLength[(int) buffer[i]]; shift++) {
                if (mapOfCodes[(int) buffer[i]] & (1llu << (BITS_IN_LONG_LONG - shift))) {
                    AddBitToByteAndWrite(currentBitNumber, currentByte, 1);
                }
                else {
                    AddBitToByteAndWrite(currentBitNumber, currentByte, 0);
                }
            }
        }
    }
    fseek(in, 1, SEEK_SET);
}

void EncodeAlgo() {

    int frequencyTable[ASCII_SIZE] = {0};
    unsigned char characters[ASCII_SIZE] = {0};
    unsigned char buffer[BUFFER_SIZE];
    int charactersNumber = 0;

    int readLength;
    while ((readLength = NextCharacters(buffer))) {
        for (int i = 0; i < readLength; i++) {
            if (!frequencyTable[(int) buffer[i]]) {
                characters[charactersNumber] = buffer[i];
                charactersNumber++;
            }
            frequencyTable[(int) buffer[i]]++;
        }
    }
    fseek(in, 1, SEEK_SET);

    codeTreeNodeList_t listOfNodes = GenerateTreeFromTable(frequencyTable, characters, charactersNumber);
    while (listOfNodes.top > 0) {
        RecalculateTree(&listOfNodes);
    }

    unsigned long long mapOfCodes[ASCII_SIZE] = {0};
    char mapOfCodeLength[ASCII_SIZE] = {0};
    unsigned short bitLengthOfEncodedTree = 0;
    GetAllCodes(&listOfNodes, mapOfCodes, mapOfCodeLength, &bitLengthOfEncodedTree);

    char currentBitNumber = 0;
    unsigned char currentByte = 0;
    int bitLengthOfEncodedText = CountBitLengthOfEncodedText(buffer, mapOfCodeLength);

    /*
     * 1 byte for bitLengthOfEncodedText % 8
     * bits of encoded tree (which is rounded to bytes)
     * bits of encoded text
     */

    WriteBitLengthOfEncodedText(bitLengthOfEncodedText % 8, &currentBitNumber, &currentByte);

    // tree
    WriteEncodedTree(peek(&listOfNodes), &currentBitNumber, &currentByte);
    if (currentBitNumber != 0) {
        fprintf(out, "%c", currentByte);
        currentBitNumber = 0;
    }

    // text
    WriteEncodedText(buffer, mapOfCodes, mapOfCodeLength, &currentBitNumber, &currentByte);
    if (currentBitNumber != 0) {
        fprintf(out, "%c", currentByte);
    }

    FreeTreeDFS(peek(&listOfNodes).leftChild);
    FreeTreeDFS(peek(&listOfNodes).rightChild);
}

codeTreeNode *ReadNodesFromBuffer(unsigned char buffer[BUFFER_SIZE], int *bitNumber) {
    if ((unsigned char) (buffer[(int) *bitNumber / 8] << (*bitNumber % 8)) & (unsigned char) (1 << 7)) {
        (*bitNumber)++;
        unsigned char character = 0;
        character |= (char) (buffer[(int) *bitNumber / 8] << (*bitNumber % 8));
        (*bitNumber) += 8;
        character |= (char) (buffer[(int) *bitNumber / 8] >> (8 - (*bitNumber % 8)));
        codeTreeNode *node = malloc(sizeof(codeTreeNode));
        *node = (codeTreeNode) {0, character, false, NULL, NULL};

        return node;
    }
    else {
        (*bitNumber)++;
        codeTreeNode *leftChild = ReadNodesFromBuffer(buffer, bitNumber);
        codeTreeNode *rightChild = ReadNodesFromBuffer(buffer, bitNumber);

        codeTreeNode *node = malloc(sizeof(codeTreeNode));
        *node = (codeTreeNode) {0, 0, true, leftChild, rightChild};

        return node;
    }
}

int ReadBitRestOfEncodedText() {
    unsigned char rest;
    if (!fscanf(in, "%c", &rest)) {
        return 0;
    }
    return rest;
}

void DecodeText(codeTreeNode *root, codeTreeNode *currentNode, int rest, int *bitNumber) {
    unsigned char firstByte, secondByte;
    if (!rest)
        rest = 8;
    if (!fscanf(in, "%c", &firstByte))
        return;

    bool read = true;
    while (read) {
        int iterateTo;
        if (fscanf(in, "%c", &secondByte) != EOF) {
            iterateTo = 8;
        } else {
            iterateTo = rest;
            read = false;
        }
        for (int i = 0; i < iterateTo; (*bitNumber)++, i++) {

            if ((unsigned char) (firstByte << (*bitNumber % 8)) & (unsigned char) (1 << 7)) {
                currentNode = currentNode->rightChild;
            } else {
                currentNode = currentNode->leftChild;
            }
            if (!currentNode->empty) {
                fprintf(out, "%c", currentNode->character);
                currentNode = root;
            }
        }
        firstByte = secondByte;
    }

}

void DecodeAlgo() {
    unsigned char buffer[BUFFER_SIZE];

    int bitRestOfEncodedText = ReadBitRestOfEncodedText();
    int bitNumber = 0;
    if (!NextCharacters(buffer))
        return;

    codeTreeNode *treeRoot = ReadNodesFromBuffer(buffer, &bitNumber);
    fseek(in, (long) (2 + ceil((double) bitNumber / 8)), SEEK_SET);
    bitNumber = 0;
    DecodeText(treeRoot, treeRoot, bitRestOfEncodedText, &bitNumber);
    FreeTreeDFS(treeRoot);
}

int main() {
    in = fopen("in.txt", "r");
    out = fopen("out.txt", "w");

    char mode;
    if (!fscanf(in, "%c", &mode)) {
        return 0;
    }
    if (mode == 'c') {
        EncodeAlgo();
    }
    if (mode == 'd') {
        DecodeAlgo();
    }
}
