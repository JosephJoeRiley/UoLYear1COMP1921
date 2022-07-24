#ifndef __PGM_A2B__
#define __PGM_A2B__
#include "pgmImage.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
PgmImage convertA2B(const char *outDir, PgmImage a, int *errorReturn);
#endif //__PGM_A2B__
