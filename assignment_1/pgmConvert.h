#ifndef __PGM_CONVERT__
#define __PGM_CONVERT__
#include "pgmImage.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
PgmImage convertB2A(const char *outDir, PgmImage a, int *errorReturn);
PgmImage convertA2B(const char *outDir, PgmImage a, int *errorReturn);
#endif //__PGM_CONVERT__include "pgmRead"
