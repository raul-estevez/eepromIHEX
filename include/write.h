#include <stdio.h>

void writeLine(FILE *eeprom_file, unsigned char byte_count, unsigned short address);
unsigned char * getData(unsigned long start_point, unsigned char data_lenght, char *data_array, char *data_file);
