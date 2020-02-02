#ifndef _HELPER_H_  // prevent recursive inclusion
#define _HELPER_H_
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "crypt.h"

#define _TRUE 1
#define _FALSE 0
#define _ALPHABET_COUNT 26
#define _USER_ERROR 20
#define _FILE_ERROR 30
#define _MEMORY_ERROR 40
#define _ENCRYPT_ERROR 50
#define _DECRYPT_ERROR 51
#define _LOGGING_ERROR 60

void _softError(char* message, int code);
void _handleError(char* errorMessage, int code, FILE* fp);
void _cleanUp(FILE *fp);
uint8_t _check(uint8_t condition);
char* getDate();
void printAll(Options* options, int row, int column, int** matrix, char* ciphertext);
void logAll(Options* options, int row, int column, int** matrix, char* ciphertext);
FILE* _openFile(const char* filename, const char* mode, char* errorMessage);

#endif