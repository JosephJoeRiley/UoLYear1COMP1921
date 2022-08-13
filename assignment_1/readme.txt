COMP1921 Assignment 1 PGM Utilities
Author_name: Joseph Riley 
Author_SID: 201519259
creation_date: 18/07/2022
Contained executables 
	pgmEcho
		Copies an input pgm file to an output pgm file
		Works by reading in the input file and then writing it 
		to an ouput file. For read and write functionalities,
		see the pgmIO module
	pgmComp
		Returns whether 2 pgms are logically identical
		Works by reading in 2 pgm files to pgmImage obejects and
		comparing the width, height and max gray; returning whether
		these values are logically identical 
	
	Both of the below modules work by simply changing the magic number
	(and if nessacary the file name) since the representation in the 
	pgmImage is format neutral 
	pgma2b
		Converts a pgm image from ascii to binary  
	pgmb2a
		Converts a pgm image from binary to ascii

	pgmReduce
		Reads in a pgmFile and returns the same file, but with the 
		image data (along with the width and height) altered so that 
		only the nth pixels are shown 
	pgmTile
		Reads in a pgm and writes n*n images that are tiles of the 
		input image. The image data is altered so that only every nth
		pixel is printed to the output image
	pgmAssemble
		Reads in several pgm files and returns one image ...
Contained libraries
	pgmErrors
		
	pgmImage
	
	pgmIO
