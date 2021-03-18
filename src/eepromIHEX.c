#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// TODO: add data from file
unsigned char data[24] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16, 0x50};

int main(int argc, char *argv[]){

    // ################# Command line arguments handling #################  
    char *FILENAME = NULL;
    unsigned char MAX_DATA_LENGTH = 0x10;               // Default 16 bytes 
    unsigned short INTIAL_ADDRESS = 0;                  // Initial address 0
    unsigned char verbose_flag = 0;
    unsigned char help_flag = 0;
    unsigned char file_flag = 0;

    int option;
    while((option = getopt(argc, argv, "o:d:a:vh")) != -1){ //get option from the getopt() method
        switch(option){
            case 'o':
                FILENAME = optarg;
                file_flag = 1;
                break;
            case 'd':
                MAX_DATA_LENGTH = atoi(optarg);
                if(MAX_DATA_LENGTH > 0xFF){
                    printf("Error, value given to MAX_DATA_LENGTH too big (>0xFF)");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'a':
                INTIAL_ADDRESS = atoi(optarg);
                if(INTIAL_ADDRESS > 0xFFFF){
                    printf("Error, value given to INTIAL_ADDRESS too big (>0xFFFF)");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case 'h':
                help_flag = 1;
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


    if(file_flag == 0){
        FILENAME = "c.eep";
    }







    // ################# Start of main program #################  


    // Global variables
    unsigned long data_remainig = sizeof(data);         // The raw data that remains not formatted
    unsigned short word16_counter = 0;                  // Counst how many 16 bit long data have we formatted
    unsigned short address = INTIAL_ADDRESS;            // Here you can change where your data addres starts, the it will increment by 16 (or the length of the remainin data if <16)



    // Create and open the file
    FILE *eeprom_file;
    eeprom_file = fopen(FILENAME,"w");
    if(eeprom_file == NULL){
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    while(data_remainig != 0){

        unsigned int checksum = 0;

        // Byte count and address if the data remainin is greater than the max data lenght
        if(data_remainig >= MAX_DATA_LENGTH){
            // Start code
            if(fputc(':',eeprom_file) == EOF){
                perror("Error writing start code" );
                exit(EXIT_FAILURE);
            }

            // Byte count
            fprintf(eeprom_file, "%X", MAX_DATA_LENGTH);
            checksum += MAX_DATA_LENGTH;

            data_remainig -= MAX_DATA_LENGTH;

            // Address
            address += MAX_DATA_LENGTH;
            // Write the address, format: Padding with zeros, 4 characters
            if(fprintf(eeprom_file,"%04X",address) < 0){
                perror("Error writing address");
                exit(EXIT_FAILURE);
            }
            // The checksum algoritm sums the data byte by byte, so we need to separate each byte of the 2 byte address and sum the up
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);

            // Record type
            // We assume data only
            if(fputs("00",eeprom_file) == EOF){
                perror("Error writting record type");
                exit(EXIT_FAILURE);
            }

            // Data
            for(long i = 0; i < MAX_DATA_LENGTH; i++){
                // We format the data byte as we did with the address
                //Format: padding with zeros, 2 characters
                if(fprintf(eeprom_file,"%02X", data[MAX_DATA_LENGTH * word16_counter + i]) < 0){
                    perror("Error writing data");
                    exit(EXIT_FAILURE);
                }
                checksum += data[MAX_DATA_LENGTH * word16_counter + i];                         // As data here is only 1 byte long we do not need formatting for the cheacksum
            }
            word16_counter++;                                // This way we shift the data that will be formatted by MAX_DATA_LENGTH bytes each time we increment the variable


        } else {

            // Here we do the same as above but when we have less than MAX_DATA_LENGTH bytes left, that means that this is our last data write to the file

            // Start code
            if(fputc(':', eeprom_file) == EOF){
                perror("Error wrinting final start code");
                exit(EXIT_FAILURE);
            }

            // Byte count
            if(fprintf(eeprom_file,"%02X", (unsigned int)data_remainig) < 0){
                perror("Error writing final byte count");
                exit(EXIT_FAILURE);
            }
            checksum += (unsigned int) data_remainig;

            // Address
            address += data_remainig;
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);

            if(fprintf(eeprom_file,"%04X",address) < 0){
                perror("Error writing final address");
                exit(EXIT_FAILURE);
            }

            // Record type
            if(fputs("00",eeprom_file) == EOF){
                perror("Error writing final record type");
                exit(EXIT_FAILURE); 
            }

            // Data
            for(int i = 0; i < (unsigned int)data_remainig; i++){
                    if(fprintf(eeprom_file,"%02X", data[16 * word16_counter + i]) < 0){
                        perror("Error writing final data");
                        exit(EXIT_FAILURE);
                    }
                    checksum += data[16 * word16_counter + i];
            }
            data_remainig = 0;
        }


        // If you want to see the pre-calculated checksum besides the calculated checksum for debug reasons 
        // printf("%X\t%X\n", checksum, ((~checksum)+1) & 0xFF);
        
        // The checksum is calculated by summing each byte we written, calculating its two's complement and keeping the LSByte
        if(fprintf(eeprom_file,"%X\n", ((~checksum)+1) & 0xFF) < 0){
            perror("Error writing checksum");
            exit(EXIT_FAILURE);
        }
    }
    
    // EOF
    if(fputs(":00000001FF", eeprom_file) == EOF){
        perror("Error writing EOF");
        exit(EXIT_FAILURE);
    }
    if(fclose(eeprom_file) == EOF){
        perror("Error closing the file, exiting program anyway");
        exit(EXIT_FAILURE);
    } else {
        printf("File successfully formatted, exiting program\n");
        exit(EXIT_SUCCESS);
    }
}
