#include "helper.h"

void _softError(char* message, int code){
    int err = open("../logs/error.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) perror("opening cerr.log");
    int save_err = dup(fileno(stderr));
    if (-1 == dup2(err, fileno(stderr))) perror("cannot redirect stderr");

    fprintf(stderr, "\n-------------------------------------------------------------------------------------------\n");
    fprintf(stderr, "%s\n", getDate());
    fprintf(stderr, "\n\n%s\n", message);

    fflush(stderr); close(err);
    dup2(save_err, fileno(stderr));
    close(save_err);

    exit(code);
}

void _handleError(char* errorMessage, int code, FILE *fp)
{
    int err = open("../logs/error.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) perror("opening error.log");
    int save_err = dup(fileno(stderr));
    if (-1 == dup2(err, fileno(stderr))) perror("cannot redirect stderr");

    fprintf(stderr, "\n-------------------------------------------------------------------------------------------\n");
    fprintf(stderr, "%s\n", getDate());
    fprintf(stderr, "\n\nERROR: %s\n", errorMessage);
    _cleanUp(fp);

    fflush(stderr); close(err);
    dup2(save_err, fileno(stderr));
    close(save_err);

    exit(code);
}

void _cleanUp(FILE *fp)
{
    if (fp != NULL) fclose(fp);
    // free();
}

uint8_t _check(uint8_t condition)
{
    uint8_t is_valid = _TRUE;
    if(!condition) is_valid = _FALSE;
    return is_valid;
}

char* getDate(){
	time_t rawtime;
 	struct tm* timeinfo;
  	time (&rawtime);
  	timeinfo = localtime(&rawtime);
  	char* curTime = asctime(timeinfo);
  	for (int i=0; curTime[i]!='\0'; i++) curTime[i]=tolower(curTime[i]);

  	return curTime;
}


void printAll(Options* options, int row, int column, int** matrix, char* ciphertext){
    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%s\n", getDate());
    printOptions(options);
    printf("\n\n\n*****~~~~~~~~~~~*****\n");
    printf("\n*****Ciphertext*****\n");
    printf("%s\n", ciphertext);
    printf("Size of ciphertext: %lu\n", strlen(ciphertext));
    printf("Size of plaintext: %lu\n", strlen(options->plaintext));
    printf("\n*****Encryption table*****\n");
    printMatrix(row, column, matrix);
}

void logAll(Options* options, int row, int column, int** matrix, char* ciphertext){
    int out = open("../logs/output.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) perror("opening output.log"); 
    int err = open("../logs/error.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) perror("opening error.log");

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) perror("cannot redirect stdout");
    if (-1 == dup2(err, fileno(stderr))) perror("cannot redirect stderr");

    printAll(options, row, column, matrix, ciphertext);

    fflush(stdout); close(out);
    fflush(stderr); close(err);
    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));
    close(save_out);
    close(save_err);
}

/*
 * Open file. In case of error, print message and exit.
 */
FILE* _openFile(const char* filename, const char* mode, char* errorMessage)
{
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) 
    	_softError(errorMessage, _FILE_ERROR);
    return fp;
}