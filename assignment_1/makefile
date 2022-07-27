#We will add later modules as they are implemented  
CC=gcc
CCFLAGS=-g -std=c99 #-fmax-errors=1 -Wall -Werror
MODULES=pgmImage.o pgmErrors.o pgmRead.o pgmWrite.o pgmConvert.o 
MAINS=pgmEcho pgmComp pgma2b pgmb2a pgmReduce 
TARGETS=pgmEcho pgmComp pgma2b pgmb2a pgmReduce

all:	${TARGETS}


clean:
	rm ${TARGETS} *.o


pgmReduce:	pgmReduce.o ${MODULES}
	${CC}	${CCFLAGS}	-o pgmReduce pgmReduce.o ${MODULES}

pgmEcho:	pgmEcho.o	${MODULES}
	${CC}	${CCFLAGS}	-o pgmEcho	pgmEcho.o ${MODULES}

pgmComp:	pgmComp.o	${MODULES}
	${CC}	${CCFLAGS}	-o pgmComp	pgmComp.o ${MODULES}

pgma2b:		pgma2b.o	${MODULES}
	${CC}	${CCFLAGS}	-o pgma2b	pgma2b.o ${MODULES}

pgmb2a:		pgmb2a.o	${MODULES}
	${CC}	${CCFLAGS}	-o pgmb2a	pgmb2a.o ${MODULES}

#pgmTile:	pgmTile.o	${MODULES}
#	${CC}	${CCFLAGS}	-o pgmTile pgmTile.o ${MODULES}


pgmEcho.o:	pgmEcho.c	pgmWrite.h pgmRead.h pgmImage.h pgmErrors.h
	${CC}	-c ${CCFLAGS}	-o pgmEcho.o	pgmEcho.c 

pgmComp.o:	pgmComp.c	pgmRead.h pgmImage.h pgmErrors.h pgma2b.h
	${CC}	-c ${CCFLAGS}	-o pgmComp.o	pgmComp.c 

pgmReduce.o: pgmReduce.c	pgmWrite.h pgmRead.h pgmImage.h pgmErrors.h
	${CC}	-c ${CCFLAGS}	-o pgmReduce.o	pgmReduce.c 

#pgmTile.o: pgmTile.c	pgmWrite.h pgmRead.h pgmImage.h pgmErrors.h
#	${CC}	-c ${CCFLAGS}	-o pgmTile.o	pgmTile.c 

pgma2b.o:	pgma2b.c	pgmRead.h pgmWrite.h pgmErrors.h pgmImage.h
	${CC}	-c ${CCFLAGS}	-o pgma2b.o		pgma2b.c 	

pgmb2a.o:	pgmb2a.c	pgmRead.h pgmWrite.h pgmErrors.h pgmImage.h
	${CC}	-c ${CCFLAGS}	-o pgmb2a.o		pgmb2a.c 	


pgmImage.o:	pgmImage.c	pgmImage.h pgmErrors.h 
	${CC}	-c ${CCFLAGS}	-o pgmImage.o	pgmImage.c	 	 

pgmErrors.o: pgmErrors.c	pgmErrors.h
	${CC}   -c ${CCFLAGS}	-o pgmErrors.o	pgmErrors.c

pgmWrite.o:	pgmWrite.c	pgmImage.h pgmErrors.h 
	${CC}	-c ${CCFLAGS}	-o pgmWrite.o	pgmWrite.c 

pgmRead.o:	pgmRead.c	pgmImage.h pgmErrors.h 
	${CC}	-c ${CCFLAGS}	-o pgmRead.o	pgmRead.c 


