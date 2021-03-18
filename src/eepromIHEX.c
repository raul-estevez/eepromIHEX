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
                    printf("Error, value given to MAX_DATA_LENGTH too big (>0xFF)\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'a':
                INTIAL_ADDRESS = atoi(optarg);
                if(INTIAL_ADDRESS > 0xFFFF){
                    printf("Error, value given to INTIAL_ADDRESS too big (>0xFFFF)\n");
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

    // Default name for the output file
    if(file_flag == 0){
        FILENAME = "c.eep";
    }
    // Versbose initial config
    if(verbose_flag){
        printf("Output file: %s\n", FILENAME);
        printf("MAX_DATA_LENGTH: %d\n", MAX_DATA_LENGTH);
        printf("INTIAL_ADDRESS: %d\n", INTIAL_ADDRESS);
    }


    // ################# Start of main program #################  

    // Global variables
    unsigned long data_remainig = sizeof(data);         // The raw data that remains not formatted
    unsigned long word_counter = 0;                  // Counst how many 16 bit long data have we formatted
    unsigned short address = INTIAL_ADDRESS;            // Here you can change where your data addres starts, the it will increment by 16 (or the length of the remainin data if <16)



    // Create and open the file
    FILE *eeprom_file;
    eeprom_file = fopen(FILENAME,"w");
    if(eeprom_file == NULL){
        perror("Error opening the file\n");
        exit(EXIT_FAILURE);
    }else if (verbose_flag){
        printf("File opened: %s\n", FILENAME);
    }
    

    while(data_remainig != 0){

        unsigned int checksum = 0;

        // Byte count and address if the data remainin is greater than the max data lenght
        if(data_remainig >= MAX_DATA_LENGTH){
            // Start code
            if(fputc(':',eeprom_file) == EOF){
                perror("Error writing start code\n");
                exit(EXIT_FAILURE);
            } else if(verbose_flag){
                printf("Successfully written start code (':')\n");
            }

            // Byte count
            if(fprintf(eeprom_file, "%02X", MAX_DATA_LENGTH) < 0){
                perror("Error writing byte count\n");
                exit(EXIT_FAILURE);
            } else if(verbose_flag){
                printf("Successfully written byte count %02X\n",MAX_DATA_LENGTH);
            }
            checksum += MAX_DATA_LENGTH;
            if(verbose_flag){
                printf("Checksum sum: %X\n", checksum);
            }

            data_remainig -= MAX_DATA_LENGTH;
            if(verbose_flag){
                printf("Data remainin: %lu\n", data_remainig);
            }

            // Address
            address += MAX_DATA_LENGTH;
            // Write the address, format: Padding with zeros, 4 characters
            if(fprintf(eeprom_file,"%04X",address) < 0){
                perror("Error writing address\n");
                exit(EXIT_FAILURE);
            } else if(verbose_flag){
                printf("Successfully written address %X\n", address);
            }
            // The checksum algoritm sums the data byte by byte, so we need to separate each byte of the 2 byte address and sum the up
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);
            if(verbose_flag){
                printf("Checksum sum: %X\n", checksum);
            }

            // Record type
            // We assume data only
            if(fputs("00",eeprom_file) == EOF){
                perror("Error writting record type\n");
                exit(EXIT_FAILURE);
            }

            // Data
            for(long i = 0; i < MAX_DATA_LENGTH; i++){
                // We format the data byte as we did with the address
                //Format: padding with zeros, 2 characters
                if(fprintf(eeprom_file,"%02X", data[MAX_DATA_LENGTH * word_counter + i]) < 0){
                    perror("Error writing data\n");
                    exit(EXIT_FAILURE);
                }
                checksum += data[MAX_DATA_LENGTH * word_counter + i];                         // As data here is only 1 byte long we do not need formatting for the checksum
                if(verbose_flag){
                    printf("Successfully written data %02X\n", data[MAX_DATA_LENGTH * word_counter + i]);
                    printf("Checksum sum: %X\n", checksum);
                }
            }
            word_counter++;                                // This way we shift the data that will be formatted by MAX_DATA_LENGTH bytes each time we increment the variable
            if(verbose_flag){
                printf("Word counter %lu\n", word_counter);
            }

        } else {

            // Here we do the same as above but when we have less than MAX_DATA_LENGTH bytes left, that means that this is our last data write to the file

            // Start code
            if(fputc(':', eeprom_file) == EOF){
                perror("Error wrinting final start code\n");
                exit(EXIT_FAILURE);
            } else if(verbose_flag){
                printf("Successfully written start code (':')\n");
            }

            // Byte count
            if(fprintf(eeprom_file,"%02X", (unsigned int)data_remainig) < 0){
                perror("Error writing final byte count\n");
                exit(EXIT_FAILURE);
            } else if(verbose_flag){
                printf("Successfully written byte count %02X\n",(unsigned int)data_remainig);
            }
            checksum += (unsigned int) data_remainig;
            if(verbose_flag){
                printf("Checksum sum: %X\n", checksum);
            }

            // Address
            address += data_remainig;
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);

            if(fprintf(eeprom_file,"%04X",address) < 0){
                perror("Error writing final address\n");
                exit(EXIT_FAILURE);
            } else if(verbose_flag){
                printf("Successfully written address %X\n", address);
                printf("Checksum sum: %X\n", checksum);
            }

            // Record type
            if(fputs("00",eeprom_file) == EOF){
                perror("Error writing final record type\n");
                exit(EXIT_FAILURE); 
            }

            // Data
            for(int i = 0; i < (unsigned int)data_remainig; i++){
                    if(fprintf(eeprom_file,"%02X", data[MAX_DATA_LENGTH * word_counter + i]) < 0){
                        perror("Error writing final data\n");
                        exit(EXIT_FAILURE);
                    }
                    checksum += data[16 * word_counter + i];
                    if(verbose_flag){
                        printf("Successfully written data %02X\n", data[MAX_DATA_LENGTH * word_counter + i]);
                        printf("Checksum sum: %X\n", checksum);
                    }
            }
            data_remainig = 0;
        }


        // If you want to see the pre-calculated checksum besides the calculated checksum for debug reasons 
        // printf("%X\t%X\n", checksum, ((~checksum)+1) & 0xFF);
        
        // The checksum is calculated by summing each byte we written, calculating its two's complement and keeping the LSByte
        if(fprintf(eeprom_file,"%X\n", ((~checksum)+1) & 0xFF) < 0){
            perror("Error writing checksum\n");
            exit(EXIT_FAILURE);
        } else if(verbose_flag){
            printf("FINAL CHECKSUM: %02X\n", checksum);
        }
    }
    
    // EOF
    if(fputs(":00000001FF", eeprom_file) == EOF){
        perror("Error writing EOF\n");
        exit(EXIT_FAILURE);
    } else if(verbose_flag){
        printf("Successfully written EOF code\n");
    }
    if(fclose(eeprom_file) == EOF){
        perror("Error closing the file, exiting program anyway\n");
        exit(EXIT_FAILURE);
    } else {
        printf("File successfully formatted, exiting program\n");
        exit(EXIT_SUCCESS);
    }
}
