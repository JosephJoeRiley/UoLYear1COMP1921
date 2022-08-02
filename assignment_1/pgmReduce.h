#ifndef  __PGM_REDUCE__
#define __PGM_REDUCE__
#include <math.h>
#include "pgmImage.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
PgmImage pgmReduce(const char *read_filename, unsigned int factor, int *return_value);
int main(int argc, char** argv);
#endif //__PGM_REDUCE__

