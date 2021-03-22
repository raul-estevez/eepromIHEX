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
extern unsigned char verbose_flag;
extern unsigned char help_flag;
extern unsigned char file_flag;
extern unsigned char version_flag;
extern unsigned char dataLength_flag;
extern unsigned char address_flag;

extern unsigned char MAX_DATA_LENGTH;
extern unsigned short INTIAL_ADDRESS;
extern char *VERSION ;
extern char *FILENAME;
extern char *IN_FILENAME;

int handleCLIargs(int argc, char *argv[]);
int versionHandler(void);
int helpHandler(void);
