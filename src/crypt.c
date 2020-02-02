#include "crypt.h"
const char alphabet[_ALPHABET_COUNT] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char smileDictionary[_ALPHABET_COUNT+1][10]={
        {":-)"}, //0
        {":-("}, //1
        {"\\:D/"}, //2
        {":-o"}, //3
        {"=("}, //4
        {":-Q"}, //5
        {":P"}, //6
        {"=-O"}, //7
        {":-E"}, //8
        {"=D"}, //9
        {";-)"}, //10
        {"^_^"}, //11
        {"(-:"}, //12
        {"<3"}, //13
        {":-)(-:"}, //14
        {"=/"}, //15
        {"<:3)~"}, //16
        {"<><"}, //17
        {"B-)"}, //18
        {":-#"}, //19
        {"X-("}, //20
        {"O:-)"}, //21
        {">:)"}, //22
        {"$_$"}, //23
        {"8-)"}, //24
        {":-*"}, //25
        {" "} //26
    };

void encrypt(Options* options){
 	char* text = options->plaintext;
 	char* key = options->key;
 	int textLength = strlen(text);

 	//equalize length
    char* newKey=generateValidKey(key, textLength);
 	int**  matrix = generateMatrix(_ALPHABET_COUNT);
    char* ciphertext=(char*)calloc(1, 1000);
    ciphertext[0] = '\0';

    for (int i=0; i<textLength; i++)
    {	
    	if(text[i] >= 'A' && text[i] <= 'Z'){
    		int smileCode = matrix[orderFromAlphabet(newKey[i])][orderFromAlphabet(text[i])];
    		char* smileText = smileTextByCode(smileCode);
    		strcat(ciphertext, smileText);
	    	strcat(ciphertext, " ");
    	}else
    		ciphertext[strlen(ciphertext)] = text[i];
    }
	strcat(ciphertext, " ");


	FILE* ciphertextFile = _openFile("../output/ciphertext.txt", "wb", "ERROR: Could not open output file...");
    fwrite(ciphertext, strlen(ciphertext), 1, ciphertextFile);
    FILE* tableFile = _openFile("../output/matrix.txt", "wb", "ERROR: Could not open output file...");
    for (int i=0; i<_ALPHABET_COUNT; i++)
    {
    	for (int j=0; j<_ALPHABET_COUNT; j++)
	    	fprintf(tableFile, "%d ", matrix[i][j]);
	    fprintf(tableFile, "\n");
    }

  //   printMatrix(_ALPHABET_COUNT, _ALPHABET_COUNT, matrix);
 	// printf("%s\n", text);
 	// printf("%s\n", newKey);
 	// printf("%s\n", ciphertext);

 	logAll(options, _ALPHABET_COUNT, _ALPHABET_COUNT, matrix, ciphertext);

 	printf("\n\nCiphertext:\t%s\n", ciphertext);
}

void decrypt(Options* options){
	char* ciphertextFileName = options->cipherTextFile;
	char* tableFileName = options->tableFile;


	FILE* ciphertextFP = _openFile(ciphertextFileName, "r", "ERROR: Could not open output file...");
    FILE* tableFP = _openFile(tableFileName, "r", "ERROR: Could not open output file...");
 	int**  matrix = generateMatrix(0);
    char* ciphertext=(char*)calloc(1, 1000);
	for (int i=0; i<_ALPHABET_COUNT; i++)
 		for (int j=0; j<_ALPHABET_COUNT; j++)
 			fscanf(tableFP, "%d  ", &matrix[i][j]);
 	fscanf(ciphertextFP, "%[^\n]", ciphertext);


 	// int textLength = getSizeOfCipherText(ciphertext);
 	int textLength = strlen(ciphertext);
	char* key = options->key;
    char* newKey=generateValidKey(key, textLength);

    char* smileText=(char*)calloc(1, 10);
    smileText[0]='\0';
    char* plaintext=(char*)calloc(1, 1000);
    plaintext[0]='\0';
    int smileCode, keyCode;
    int counter=0;

 	for (int i=0; i<strlen(ciphertext); i++)
 	{
 		if(ciphertext[i] == ' '){
 			smileCode = smileCodeByText(smileText);

 			if(smileCode != -1){
 				keyCode = orderFromAlphabet(newKey[counter++]);
 				char ch = findLetterByKeyAndSmile(keyCode, smileCode, _ALPHABET_COUNT, _ALPHABET_COUNT, matrix);
 				strncat(plaintext, &ch, 1); 
 			}else{
 				counter+=(strlen(smileText)+1);
 				strcat(plaintext, smileText); 
	    		strcat(plaintext, " ");
 			}
 			smileText[0]='\0';
 		}else{
 			strncat(smileText, &ciphertext[i], 1); 
 		}

 		if(ciphertext[i] == ' ' && ciphertext[i+1] == ' '){
 			i++;
 			counter++;
	    	strcat(plaintext, " ");
 		}
 	}

 	logAll(options, _ALPHABET_COUNT, _ALPHABET_COUNT, matrix, ciphertext);
 	printf("\n\nPlaintext:\t%s\n", plaintext);
}

char* smileTextByCode(int smileCode){
    char* smileText = (char*)calloc(1, 10); 
    strcpy(smileText, smileDictionary[smileCode]);
    return smileText;
}

int smileCodeByText(char* smileText){
	for (int i=0; i<_ALPHABET_COUNT; i++)
		if (strcmp(smileDictionary[i], smileText) == 0)
			return i;

	return -1;
}

char letterFromAlphabet(int order){

	return alphabet[order];
}

int orderFromAlphabet(char letter){
	for (int i=0; i<_ALPHABET_COUNT; i++)
		if(alphabet[i] == letter)
			return i;
}

int** generateMatrix(int base){
	int rows = _ALPHABET_COUNT; int columns = _ALPHABET_COUNT;
    int** matrix=(int**)malloc(sizeof(int*) * rows);


    for (int i=0; i<rows; i++){
        matrix[i]=(int*)malloc(sizeof(int)*columns);
    	int* rowValues=(int*)malloc(sizeof(int) * _ALPHABET_COUNT);
        rowValues=createRandomArray(base);
        for (int j=0; j<columns; j++)
            matrix[i][j] = rowValues[j];

        free(rowValues);
    }

    return matrix;
}

void printMatrix(int rows, int columns, int** matrix){
	printf("   ");
    for (int j=0; j<columns; j++)
        printf("+--%c--", alphabet[j]);

    printf("+\n");
    for (int i=0; i<rows; i++){
    	printf("%c  ", alphabet[i]);
        for (int j=0; j<columns; j++)
            printf("| %3d ", matrix[i][j]);

        printf("|\n   +");

        for (int j = 0; j < columns; j++)
            printf("-----+");

        printf("\n");
    }
}

//MODIFY THIS FUNCTION, rand() is not secure!
int* createRandomArray(int base){
    int* arr=(int*)calloc(1, sizeof(int) * _ALPHABET_COUNT);
    for (int i=0; i<base; i++)
    	arr[i]=i;

    for (size_t i=0; i<_ALPHABET_COUNT; i++) 
    {
      size_t j = i+rand() / (RAND_MAX/(_ALPHABET_COUNT-i)+1);
      int t = arr[j];
      arr[j] = arr[i];
      arr[i] = t;
    }

    return arr;
}

//THIS FUNCTION NEEDS CONSTRUCTION! WRONG ANSWRER FOR TEXT CONTAINING NONLETTERS
int getSizeOfCipherText(char* ciphertext){
    char* temp=(char*)calloc(1, 1000);
	strcpy(temp, ciphertext);

	// if (temp[i] == ' ' || (text[i] < 'A' || text[i] > 'Z'))
	int counter=0;
	for (int i=0; i<strlen(temp); i++)
		if (temp[i] == ' ')
			counter++;

	free(temp);
	return counter;
}

char* generateValidKey(char* key, int textLength){
 	int keyLength = strlen(key);

 	//equalize length
    char* newKey=(char*)calloc(1, 1000);
    int i, j;
    for(i=0, j=0; i<textLength; ++i, ++j){
        if(j == keyLength)
            j=0;
        newKey[i] = key[j];
    }
 	newKey[i] = '\0';

 	return newKey;
}

char findLetterByKeyAndSmile(int keyOrder, int smileNumber, int row, int column, int** matrix){
	int* matrixRow = matrix[keyOrder];

	for (int i=0; i<column; i++)
		if(matrixRow[i] == smileNumber)
			return letterFromAlphabet(i);
}	