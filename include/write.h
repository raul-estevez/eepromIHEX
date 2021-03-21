#include <stdio.h>

void writeLine(FILE *fp, unsigned char byte_count, unsigned short address, char data[]);
void getData(unsigned char data_lenght_bytes, char *data_array, FILE *fp);
unsigned int countData(FILE *fp);
