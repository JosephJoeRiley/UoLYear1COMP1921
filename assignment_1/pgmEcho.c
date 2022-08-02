#include "pgmEcho.h"


int main(int argc, char** argv)
{
    if(argc == 1)
    {
            return printOutMsg(USAGE_ERROR, argv[0], "", "");
    }
    else if(argc != 3)
    {
            return printOutMsg(BAD_ARG_NO, argv[0], "", "");
    }
    
    //We don't need to initialise this:
    //we will always call pgmRead() in which
    //all paths assign a value to it
    int error_return;
    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    PgmImage echoImage = pgmRead(input_filename, &error_return);
    
    //We print an error message and 
    //exit if read wasn't successful
    if(error_return != 0)
    {
        return printOutMsg(error_return, argv[0], input_filename, "");
    }
    
    pgmWrite(output_filename, echoImage, &error_return);
    
    if(error_return != 0)
    {
        return printOutMsg(error_return, argv[0], output_filename, "");
    }

    return printOutMsg(error_return, argv[0], "", "");
}
