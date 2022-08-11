#ifndef __PGM_COMP__
#define __PGM_COMP__
#define DIFFERENT 0
#define IDENTICAL 1
#include <stdlib.h>
#include "pgmImage.h"
#include "pgmRead.h"
#include "pgmConvert.h"
int comparePgms(const char *directory_of_imageA, const char *directory_of_imageB, int *errorReturn);
int main(int argc, char** argv);
#endif //__PGM_COMP__

