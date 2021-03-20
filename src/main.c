#include "../include/cliargs.h"

int main(int argc, char *argv[]){

    VERSION = "v0.1-alpha";
    // Handle command line args
    handleCLIargs(argc, argv);

    if(!file_flag){
        // Default output filename
        FILENAME = "a.eep";
    }
}

