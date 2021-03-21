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
*/

#include <stdio.h>
#include <stdlib.h>

#include "../include/cliargs.h"
#include "../include/write.h"

int main(int argc, char *argv[]){

    VERSION = "v1.0.0";

    // Handle command line args 
    handleCLIargs(argc, argv);
    
    // Verbose info
    if(verbose_flag){
        printf("Version: %s\n", VERSION);
        printf("Input file: %s\n", IN_FILENAME);
        printf("Output file: %s\n", FILENAME);
        printf("Data length: 0x%X\n", MAX_DATA_LENGTH);
        printf("Initial address: %X\n", INTIAL_ADDRESS);
    }

     ////////////////////
    /// Main program ///
   ////////////////////
   
    // Openig the files 
    FILE *input_file;
    FILE *output_file;

    if((input_file = fopen(IN_FILENAME, "r")) == NULL){
        perror("Error opening the input file");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        puts("Successfully openned input file");
    }
    if((output_file = fopen(FILENAME, "w")) == NULL){
        perror("Error opening the input file");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        puts("Successfully openned output file");
    }
    
    // Count data
    unsigned int data_remainign = countData(input_file);
    // This enables reading from the start of the file
    rewind(input_file);

    unsigned int data_counter = 0;
    char data[2*MAX_DATA_LENGTH];
    while(data_remainign >= MAX_DATA_LENGTH) {
        // Get data
        getData(MAX_DATA_LENGTH, data, input_file);
        // Write data
        writeLine(output_file, MAX_DATA_LENGTH, MAX_DATA_LENGTH*data_counter, 
                data);

        data_remainign -= MAX_DATA_LENGTH;
        data_counter++;
    }

    // This is the last write to the file 
    char last_data[2*data_remainign];
    // Get last data 
    getData(data_remainign, last_data, input_file);
    // Write last data 
    writeLine(output_file, data_remainign, MAX_DATA_LENGTH*data_counter, 
            last_data);
    // Write EOF 
    if((fputs(":00000001FF\n", output_file)) == EOF){
        perror("Erro writting EOF code");
    }else if(verbose_flag){
        puts("Writted EOF code");
    }
    // Close files
    if(fclose(input_file) == EOF){
        perror("Error closing the input file");
        exit(EXIT_FAILURE);
    }else if(verbose_flag){
        puts("Closed input file");
    }
    if(fclose(output_file) == EOF){
        perror("Error closing the output file");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        puts("Closed output file");
    }

    // Closing message
    puts("File writted successfully, exiting program");
    return EXIT_SUCCESS;
}

