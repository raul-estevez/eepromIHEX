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
#include <stdio.h>
#include <stdlib.h>

#include "cliargs.h"



void writeLine(FILE *fp, unsigned char byte_count, 
               unsigned short address, char data[]){

    unsigned int checksum = 0;
    

    // Start code
    if(fputc(':',fp) == EOF){
        perror("Error writing start code\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("Successfully written start code (':')\n");
    }

    // Byte count
    if(fprintf(fp, "%02X", byte_count) < 0){
        perror("Error writing byte count\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("Successfully written byte count %02X\n",byte_count);
    }
    checksum += byte_count;

    // Address
    if(fprintf(fp,"%04X",address) < 0){
        perror("Error writing address\n");
    exit(EXIT_FAILURE);
    } 
    
    // The checksum algoritm sums the data byte by byte, so we need 
    // to separate each byte of the address and sum them up
    checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);
    if(verbose_flag){
        printf("Successfully written address %X\n", address);
        printf("Checksum sum: %X\n", checksum);
    }

    // Record type
    // We assume data only
    if(fputs("00",fp) == EOF){
        perror("Error writting record type\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("Successfully written record type 00\n");
    }

    // Data 
    for(int i = 0; i < 2*byte_count; i++){
        //Format: padding with zeros, 2 characters
        if((fprintf(fp,"%c", data[i])) < 0){
            perror("Error writing data\n");
            exit(EXIT_FAILURE);
        } else if(verbose_flag){
            printf("Data address: %d\n", i);
            printf("Data: %c\n", data[i]);
        }

        checksum += data[i];                         
        if(verbose_flag){
            printf("Successfully written data %c\n", data[i]);
            printf("Checksum sum: %X\n", checksum);
        }
    }

    // The checksum is calculated by summing each byte we written, 
    // calculating its two's complement and keeping the LSByte
    if(fprintf(fp,"%02X\n", ((~checksum)+1) & 0xFF) < 0){
        perror("Error writing checksum\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("FINAL CHECKSUM: %02X\n", ((~checksum)+1) & 0xFF);
    }

}


// For saving the cursur posisiton between data reads
long pos = SEEK_SET;
void getData(unsigned char data_lenght_bytes, 
             char *data_array, FILE *fp){

    unsigned char data_counter = 0;
    char c;
    // Set previous posisiton
    fseek(fp, pos, SEEK_SET);
    // While we have data
    while((c = fgetc(fp)) != EOF){
        // See if we fetched enough data
        if(data_counter < 2*data_lenght_bytes){
            // Only if the data is a number or a A-F letter
            if((c >= 0x30 && c <= 0x39) || (c >= 0x41 && c <= 0x46)){
                if(verbose_flag){
                    printf("Data written: %c\n", c);
                }
                // Write the data to the array
                data_array[data_counter] = c;
                data_counter++;
            }
        } else {
            break;
        }
    }
    // Save current position
    pos = ftell(fp);
}




unsigned int countData(FILE *fp){
    unsigned int char_counter = 0;
    char c;
    
    // While we have data 
    while((c = getc(fp)) != EOF){
        // Only if the data is a number or a A-F letter
        if((c >= 0x30 && c <= 0x39) || (c >= 0x41 && c <= 0x46)){
            char_counter++;
        }
    }
    if(verbose_flag){
        printf("COUNTED DATA: %d\n", char_counter/2);
    }
    return char_counter/2;
}
