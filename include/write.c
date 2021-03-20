#include <stdio.h>
#include <stdlib.h>

#include "cliargs.h"



void writeLine(FILE *eeprom_file, unsigned char byte_count, 
               unsigned short address, unsigned short data[]){

    unsigned int checksum = 0;
    
    // Start code
    if(fputc(':',eeprom_file) == EOF){
        perror("Error writing start code\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("Successfully written start code (':')\n");
    }

    // Byte count
    if(fprintf(eeprom_file, "%02X", byte_count) < 0){
        perror("Error writing byte count\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("Successfully written byte count %02X\n",byte_count);
    }
    checksum += byte_count;
    if(verbose_flag){
        printf("Checksum sum: %X\n", checksum);
    }

    // Address
    if(fprintf(eeprom_file,"%04X",address) < 0){
        perror("Error writing address\n");
        exit(EXIT_FAILURE);
    } 
    
    // The checksum algoritm sums the data byte by byte, so we need 
    // to separate each byte of the address and sum them up
    checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);
    if(verbose_flag){
        printf("Checksum sum: %X\n", checksum);
        printf("Successfully written address %X\n", address);
    }

    // Record type
    // We assume data only
    if(fputs("00",eeprom_file) == EOF){
        perror("Error writting record type\n");
        exit(EXIT_FAILURE);
    }

    for(unsigned short i = 0; i < byte_count; i++){
        //Format: padding with zeros, 2 characters
        if(fprintf(eeprom_file,"%02X", data[i]) < 0){
            perror("Error writing data\n");
            exit(EXIT_FAILURE);
        }

        checksum += data[i];                         
        if(verbose_flag){
            printf("Successfully written data %02X\n", data[i]);
            printf("Checksum sum: %X\n", checksum);
        }
    }

    // The checksum is calculated by summing each byte we written, 
    // calculating its two's complement and keeping the LSByte
    if(fprintf(eeprom_file,"%X\n", ((~checksum)+1) & 0xFF) < 0){
        perror("Error writing checksum\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("FINAL CHECKSUM: %02X\n", checksum);
    }

}




void getData(unsigned long start_byte, unsigned char data_lenght_bytes, 
             char *data_array, char *data_file){

    // Open the file
    FILE *fp;
    if((fp = fopen(data_file, "r")) == NULL){
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    
    unsigned char data_counter = 0;
    char c;
    while((c = fgetc(fp)) != EOF){
        if(data_counter >= 3*start_byte){
            if(data_counter < 2*data_lenght_bytes){
                if((c >= 0x30 && c <= 0x39) || (c >= 0x41 && c <= 0x46)){
                    data_array[data_counter - 3*start_byte] = c;
                    printf("Escrito %c\n", data_array[data_counter 
                            - 3*start_byte]);
                    data_counter++;
                }
            } else {
                break;
            }
        } else {
            data_counter++;
        }
    }
}
