#ifndef _OPT_H_  // prevent recursive inclusion
#define _OPT_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

typedef struct {
	char* plaintext;
	char* key;
	unsigned char method;
	char* tableFile;
	char* cipherTextFile;
} Options;

Options* newOptions(char* plaintext, char* key, unsigned char method, char* tableFile, char* cipherTextFile);
void printOptions(Options* options);
void printManPageAndExit();
Options* getOptions(int argc, char *argv[]);

#endif