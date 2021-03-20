#include "../include/cliargs.h"
#include "../include/write.h"

int main(int argc, char *argv[]){

    VERSION = "v0.1-alpha";

    // Handle command line args
    handleCLIargs(argc, argv);

    // Default output filename
    if(!file_flag){
        FILENAME = "a.eep";
    }

    char data[32];
    getData(2,16,data,"data.eep");
    printf("%s\n", data);
}

