#include <stdio.h>

#define FILENAME "b.eep"
#define MAX_DATA_LENGTH 16          // TODO: make sure this does not exceed 0XFF 
                                    // In bytes
#define INTIAL_ADDRESS 0

// TODO: add data from file
unsigned char data[24] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16, 0x50};

int main(void){
    // Global variables
    unsigned long data_remainig = sizeof(data);         // The raw data that remains not formatted
    unsigned short word16_counter = 0;                  // Counst how many 16 bit long data have we formatted
    unsigned short address = INTIAL_ADDRESS;            // Here you can change where your data addres starts, the it will increment by 16 (or the length of the remainin data if <16)

    // Create and open the file
    FILE *eeprom_file;
    eeprom_file = fopen(FILENAME,"w");

    while(data_remainig != 0){

        unsigned int checksum = 0;

        // Byte count and address if the data remainin is greater than the max data lenght
        if(data_remainig >= MAX_DATA_LENGTH){
            // Start code
            fputc(':',eeprom_file);

            // Byte count
            fprintf(eeprom_file, "%X", MAX_DATA_LENGTH);
            checksum += MAX_DATA_LENGTH;

            data_remainig -= MAX_DATA_LENGTH;

            // Address
            address += MAX_DATA_LENGTH;
            // The checksum algoritm sums the data byte by byte, so we need to separate each byte of the 2 byte address and sum the up
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);

            // We need to add the necessary padding to the addres to make it 2 bytes long in our file
            if(address <= 0xF){
                fprintf(eeprom_file,"000%X",address);
            } else if(address <= 0xFF){
                fprintf(eeprom_file,"00%X",address);
            } else if(address <= 0xFFF){
                fprintf(eeprom_file,"0%X",address);
            } else {
                fprintf(eeprom_file,"%X",address);
            }

            // Record type
            // We assume data only
            fputs("00",eeprom_file);

            // Data
            for(long i = 0; i < MAX_DATA_LENGTH; i++){
                // We format the data byte as we did with the address
                if(data[MAX_DATA_LENGTH * word16_counter + i] <= 0x0F) { 
                    fprintf(eeprom_file,"0%X", data[MAX_DATA_LENGTH * word16_counter + i]);         // We add a 0 for padding
                    checksum += data[MAX_DATA_LENGTH * word16_counter + i];                         // As data here is only 1 byte long we do not need formatting for the cheacksum
                } else {
                    fprintf(eeprom_file,"%X", data[MAX_DATA_LENGTH * word16_counter +  i]);         // We do not add padding
                    checksum += data[MAX_DATA_LENGTH * word16_counter + i];
                }
            }
            word16_counter++;                                // This way we shift the data that will be formatted by MAX_DATA_LENGTH bytes each time we increment the variable


        } else {

            // Here we do the same as above but when we have less than MAX_DATA_LENGTH bytes left, that means that this is our last data write to the file

            // Start code
            fputc(':', eeprom_file);

            // Byte count
            fprintf(eeprom_file,"0%X", (unsigned int)data_remainig);
            checksum += (unsigned int) data_remainig;

            // Address
            address += data_remainig;
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);

            if(address <= 0xF){
                fprintf(eeprom_file,"000%X",address);
            } else if(address <= 0xFF){
                fprintf(eeprom_file,"00%X",address);
            } else if(address <= 0xFFF){
                fprintf(eeprom_file,"0%X",address);
            } else {
                fprintf(eeprom_file,"%X",address);
            }

            // Record type
            fputs("00",eeprom_file);

            // Data
            for(int i = 0; i < (unsigned int)data_remainig; i++){
                if(data[16 * word16_counter + i] <= 0x0F) { 
                    fprintf(eeprom_file,"0%X", data[16 * word16_counter + i]);
                    checksum += data[16 * word16_counter + i];
                } else {
                    fprintf(eeprom_file,"%X", data[16 * word16_counter + i]);
                    checksum += data[16 * word16_counter + i];
                }

            }
            data_remainig = 0;
        }


        // If you want to see the pre-calculated checksum besides the calculated checksum for debug reasons 
        // printf("%X\t%X\n", checksum, ((~checksum)+1) & 0xFF);
        
        // The checksum is calculated by summing each byte we written, calculating its two's complement and keeping the LSByte
        fprintf(eeprom_file,"%X\n", ((~checksum)+1) & 0xFF);
    }
    
    // EOF
    fputs(":00000001FF", eeprom_file);
    fclose(eeprom_file);
}
