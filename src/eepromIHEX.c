#include <stdio.h>

unsigned char data[23] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16};

int main(void){
    unsigned long data_remainig = sizeof(data);
    unsigned short counter = 0;
    unsigned short address = 0;

    // Create the file
    FILE *eeprom_file;
    eeprom_file = fopen("b.eep","w");

    while(data_remainig != 0){

        unsigned int checksum = 0;

        // Byte count y address
        if(data_remainig >= 16){
            // Start code
            fputc(':',eeprom_file);

            // Byte count
            fputs("10",eeprom_file);
            checksum += 0x10;

            data_remainig -= 0x10;
            address += 0x10;
            checksum += (address & 0x00FF) + ((address & 0xFF00)>>8);

            // Address
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
            for(long i = 0; i < 16; i++){
                char string[2];
                if(data[16 * counter + i]==0){
                    fputs("00",eeprom_file);
                } else if(data[16 * counter + i] <= 0x0F) { 
                    fprintf(eeprom_file,"0%X", data[16 * counter + i]);
                    checksum += data[16*counter + i];
                } else {
                    fprintf(eeprom_file,"%X", data[16 * counter + i]);
                    checksum += data[16*counter + i];
                }
            }
            counter++;
        } else {
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
                char string[2];
                if(data[16 * counter + i]==0){
                    fputs("00",eeprom_file);
                } else if(data[16 * counter + i] <= 0x0F) { 
                    fprintf(eeprom_file,"0%X", data[16 * counter + i]);
                    checksum += data[16*counter + i];
                } else {
                    fprintf(eeprom_file,"%X", data[16 * counter + i]);
                    checksum += data[16*counter + i];
                }

            }
            data_remainig = 0;
        }
        // Checksum
        // Check checksum printf("%X\t%X\n", checksum, ((~checksum)+1) & 0xFF);
        fprintf(eeprom_file,"%X\n", ((~checksum)+1) & 0xFF);
    }
    
    // EOF
    fputs(":00000001FF", eeprom_file);
    fclose(eeprom_file);
}
