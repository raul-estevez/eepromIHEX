#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "cliargs.h"

// Define cli args flags
unsigned char verbose_flag;
unsigned char help_flag;
unsigned char file_flag;
unsigned char version_flag;
unsigned char dataLength_flag;
unsigned char address_flag;

// Define program variables
unsigned char MAX_DATA_LENGTH;
unsigned short INTIAL_ADDRESS;
char *VERSION ;
char *FILENAME;
char *IN_FILENAME;

// Long and short options
static struct option long_options[] = {
    {"output", required_argument, NULL, 'O'},
    {"o", required_argument, NULL, 'o'},
    {"datalength", required_argument, NULL, 'D'},
    {"d", required_argument, NULL, 'd'},
    {"address", required_argument, NULL, 'A'},
    {"a", required_argument, NULL, 'a'},
    {"verbose", no_argument, NULL, 'V'},
    {"v", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'H'},
    {"h", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'G'},
    {0,0,0,0}
};


int handleCLIargs(int argc, char *argv[]){
    int option;
    while((option = getopt_long_only(argc, argv, "o:O:d:D:a:A:vVhHG", long_options, NULL)) != -1){ 
        switch(option){
            case 'O':
            case 'o':
                FILENAME = optarg;
                file_flag = 1;
                break;
            case 'D':
            case 'd':
                dataLength_flag = 1;
                MAX_DATA_LENGTH = atoi(optarg);
                if(MAX_DATA_LENGTH > 0xFF){
                    printf("Error, value given to MAX_DATA_LENGTH too big (>0xFF)\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'A':
            case 'a':
                address_flag = 1;
                INTIAL_ADDRESS = atoi(optarg);
                if(INTIAL_ADDRESS > 0xFFFF){
                    printf("Error, value given to INTIAL_ADDRESS too big (>0xFFFF)\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'V':
            case 'v':
                verbose_flag = 1;
                break;
            case 'H':
            case 'h':
                help_flag = 1;
                break;
            case 'G':
                versionHandler();
                break;
            case '?':
                if(optopt == 'o'){
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else if(optopt == 'd'){
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else if(optopt == 'a'){
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                }
            default:
                abort ();
        }
    }

    if(argv[optind] == NULL){
        puts("Error, input file is a mandatory argument");
        exit(EXIT_FAILURE);
    } else {
        IN_FILENAME = argv[optind];
    }


    // Default output filename
    if(!file_flag){
        FILENAME = "a.eep";
    }
    // Default data length
    if(!dataLength_flag){
        MAX_DATA_LENGTH = 0x10;
    }
    // Default initial address
    if(!address_flag){
        INTIAL_ADDRESS = 0;
    }
    return 0;
}

// The version page
int versionHandler(){
    printf("eepromIHEX %s\n", VERSION);
    printf("Copyright (C) 2021 Raúl Estévez Gómez\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
    exit(EXIT_SUCCESS);
}
