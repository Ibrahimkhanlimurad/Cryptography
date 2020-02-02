#include "helper.h"
#include "opt.h"

Options* newOptions(char* plaintext, char* key, uint8_t method, char* tableFile, char* cipherTextFile){
    Options* newOptions = (Options*)calloc(1, sizeof(Options));
    newOptions->plaintext = plaintext;
    newOptions->key = key;
    newOptions->method=method;
    newOptions->tableFile = tableFile;
    newOptions->cipherTextFile = cipherTextFile;

    return newOptions;
}

void printOptions(Options* options){
    printf("\n\n*****Options Information*****\n\n");
    printf("Plaintext: %s\n", options->plaintext);
    printf("Key: %s\n", options->key);
    printf("Method: %s\n", options->method ? "Encryption" : "Decryption");
    printf("Table file: %s\n", options->tableFile);
    printf("Ciphertext file: %s\n", options->cipherTextFile);
}

void printManPageAndExit(){
    FILE* man_file;
    if ( ! (man_file = fopen("man.txt", "r")) )
        _softError("Cannot open man file", _FILE_ERROR);
  
    // Read contents from file 
    char ch = fgetc(man_file); 
    while (ch != EOF) { 
        fprintf(stderr, "%c", ch); 
        ch = fgetc(man_file); 
    } 
  
    fclose(man_file); 
    exit(_USER_ERROR);
}

Options* getOptions(int argc, char *argv[]){
    if(argc < 2 || (argv[1] && (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "-help")==0))){
        if (argv[1] && (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "-help")==0)) optind--;
        else{
            fprintf(stderr, "For usage information: %s -h/-help\n", argv[0]);
            exit(_USER_ERROR);
        }
    }

    static struct option longopts[] = {
        { "help", no_argument, NULL, 'h' },
        { "key", required_argument, NULL, 'k' },
        { "text", required_argument, NULL, 't' },
        { "e", no_argument, NULL, 'e' },
        { "d", no_argument, NULL, 'd' },
        { "table", required_argument, NULL, 'f' },
        { "ciphertext", required_argument, NULL, 'c' },
        { 0,         0,           0, 0 }
    };

    char* plaintext=(char*)calloc(1, 1000);
    plaintext[0] = '\0';
    char* key=(char*)calloc(1, 1000);
    key[0] = '\0';
    uint8_t method;

    char* tableFile=(char*)calloc(1, 200);
    tableFile[0] = '\0';
    char* cipherTextFile=(char*)calloc(1, 200);
    cipherTextFile[0] = '\0';

    char ch;
    char error[1000];   
    while (optind<argc) {
        if((ch = getopt_long_only(argc, argv, "ht:k:edf:c:", longopts, NULL)) != -1){
            switch (ch) {
                case 'h':
                    printManPageAndExit();
                case 't':
                    for(int i = 0; optarg[i]; i++){
                        plaintext[i] = toupper(optarg[i]);
                        plaintext[i+1] = '\0';
                    }
                    break;
                case 'd':
                    method = 0;
                    break;
                case 'e':
                    method = 1;
                    break;
                case 'f':
                    strcpy(tableFile, optarg);
                    break;
                case 'c':
                    strcpy(cipherTextFile, optarg);
                    break;
                case 'k':
                    for(int i = 0; optarg[i]; i++){
                        key[i] = toupper(optarg[i]);
                        key[i+1] = '\0';
                    }
                    break;
                case ':':
                    fprintf(stderr, "Option '%s' has missing argument", argv[optind - 1]);
                    exit(_USER_ERROR);
                    break;
                case '?':
                    if (optopt == 0) 
                        fprintf(stderr, "Unknown option '%s'\n", argv[optind - 1]);
                    else 
                        fprintf(stderr, "Error parsing option '%s'\n", argv[optind - 1]);
                    fprintf(stderr, "Run ./%s -help for usage information...\n", argv[0]);
                    exit(_USER_ERROR);
                    break;
            }
        }
        else{
            fprintf(stderr, "\nWARNING: Option '%s' is unknown. Ignoring.\n", argv[optind]);
            optind+=strlen(argv[optind]);
        }
    }


    if( _check(key[0]=='\0') || (_check(method==1 && plaintext[0]=='\0')) || (_check(method==0 && (tableFile[0]=='\0' || cipherTextFile[0]=='\0'))) ){
        fprintf(stderr, "Some arguments missing...\n");
        fprintf(stderr, "For usage information: %s -h/-help\n", argv[0]);
        exit(_USER_ERROR);
    }else if(_check(tableFile[0]!='\0') && _check(method==0)){
        char* temp = (char*)calloc(1, 200);
        strcpy(temp, "../output/");
        strcat(temp, tableFile);
        strcpy(tableFile, temp);

        strcpy(temp, "../output/");
        strcat(temp, cipherTextFile);
        strcpy(cipherTextFile, temp);
    }

    Options* options = newOptions(plaintext, key, method, tableFile, cipherTextFile);
    return options;
}

