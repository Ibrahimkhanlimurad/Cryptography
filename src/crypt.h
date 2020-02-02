#ifndef _CRYPT_H  // prevent recursive inclusion
#define _CRYPT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "opt.h"
#include "helper.h"

void encrypt(Options* options);
void decrypt(Options* options);
char* smileTextByCode(int smileCode);
int smileCodeByText(char* smileText);
char letterFromAlphabet(int order);
int orderFromAlphabet(char letter);
int** generateMatrix(int base);
void printMatrix(int rows, int columns, int** matrix);
int* createRandomArray(int base);
int getSizeOfCipherText(char* ciphertext);
char* generateValidKey(char* key, int textLength);
char findLetterByKeyAndSmile(int keyOrder, int smileNumber, int row, int column, int** matrix);

#endif