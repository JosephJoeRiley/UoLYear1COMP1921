#ifndef __PGM_TILE__
#define __PGM_TILE__
#define TILED 0
#include <math.h>
#include "pgmImage.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
const char *createFileName(const char *spec, unsigned int x, unsigned int y, char mID);
void writeTile(PgmImage source, const char *origin_file_name, 
const char *target_file_name, unsigned int factor, int *return_value);
#endif //__PGM_TITLE__