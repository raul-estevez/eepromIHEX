/*
    Copyright 2021 Raúl Estévez Gómez

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in 
    the Software without restriction, including without limitation the rights to 
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
    the Software, and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all 
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    Written by Raúl Estévez Gómez <estevezgomezraul@gmail.com>
*/
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
    while((option = getopt_long_only(argc, argv, "o:O:d:D:a:A:vVhHG", 
                    long_options, NULL)) != -1){ 
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
                helpHandler();
                break;
            case 'G':
                versionHandler();
                break;
            case '?':
                if(optopt == 'o'){
                    fprintf (stderr, "Option -%c requires an argument.\n",
                            optopt);
                } else if(optopt == 'd'){
                    fprintf (stderr, "Option -%c requires an argument.\n", 
                            optopt);
                } else if(optopt == 'a'){
                    fprintf (stderr, "Option -%c requires an argument.\n", 
                            optopt);
                }
            default:
                abort ();
        }
    }
    
    // Handle the mandatory ouput file argument
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

int helpHandler(){
    printf("eepromIHEX\t%s\n\n", VERSION);
    printf("USAGE\n\teepromIHEX [options] filename \n");
    printf("-----------------------------------------------------");
    printf("\nDESCRIPTION\n");
    printf("\tTakes data from a CSV file and formats it to IHEX. This file is then ready to be uploaded into an avr eeprom using avrdude.\n");
    printf("-----------------------------------------------------");
    printf("\nOPTIONS\n");
    printf("\t--output\t-o\t\tThe name of the output file. DEFAULT: a.eep\n");
    printf("\t--datalenght\t-d\t\tMaximum number of bytes per IHEX entry. DEFAULT: 16\n");
    printf("\t--address\t-a\t\tThe first address you want to write to. DEFAULT: 0\n");
    printf("\t--verbose\t-v\t\tPrints verbose messages\n");
    printf("\t--help\t\t-h\t\tDisplays this information\n");
    printf("\t--version\t  \t\tDisplays the version message\n");
    printf("-----------------------------------------------------");
    printf("\nEXAMPLES\n");
    printf("\tDefaults, read from data.eep\n");
    printf("\t\teepromIHEX data.eep\n\n");
    printf("\tOutput to eeprom.eep, datalenght of 8, start writting from address 0xFF\n");
    printf("\t\teepromIHEX -o eeprom.eep -d 8 -a 255 data.eep\n");
    printf("-----------------------------------------------------");
    printf("\nCreated by: Raúl Estévez Gómez    <estevezgomezraul@gmail.com>\n\n");
    printf("If you encounter any bug, please refer to https://github.com/SarKing/eepromIHEX\n");
    exit(EXIT_SUCCESS);
}
