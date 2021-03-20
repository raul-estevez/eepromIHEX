#include "../include/cliargs.h"
#include "../include/write.h"

int main(int argc, char *argv[]){

    VERSION = "v0.1-alpha";

    /// Handle command line args /// 
    handleCLIargs(argc, argv);

    /// Default output filename ///
    if(!file_flag){
        FILENAME = "a.eep";
    }
    if(!dataLength_flag){
        MAX_DATA_LENGTH = 16;
    }
    if(!address_flag){
        INTIAL_ADDRESS = 0;
    }
    
    if(verbose_flag){
        printf("Version: %s", VERSION);
        printf("Input file: %s", IN_FILENAME);
        printf("Output file: %s", FILENAME);
        printf("Data length: %X", MAX_DATA_LENGTH);
        printf("Initial address: %X", INTIAL_ADDRESS);
    }


    /// Main programm ///
    
}

