nclude "pgmImage.h"


//Creates a null pgmImage with no values
////meant to trip errors
////if we want to create anything else
////we read in a pgm file or create it member
////by member in the relevant modulue
//PgmImage createDefaultPgmObject(void)
//{
//	PgmImage a;
//		a.magicNumber[0] = 'P';
//			a.magicNumber[1] = '0';
//				a.width = 0;
//					a.height = 0;
//						a.maxGray = DEFAULT_MAX_GRAY;
//							a.imageData = (unsigned char *) "";
//								a.numComments = 0;
//									a.comments = (char**) malloc(MAX_COMMENT_AMOUNT * sizeof(char));
//										return a;
//										}
//
//										PgmImage copyPgm(PgmImage b)
//										{
//											PgmImage a;
//												a.magicNumber[0] = (char) b.magicNumber[0];
//													a.magicNumber[1] = (char) b.magicNumber[1];
//														a.width = b.width;
//															a.height = b.height;
//																a.maxGray = b.maxGray;
//																	a.imageData = b.imageData;
//																		a.numComments = b.numComments;
//																			a.comments = b.comments;
//																				return a;
//																				}
//																				void printInConsole(PgmImage ref) 
//																				{
//																					printf("Data from pgm:\nMagicNumber: %s, Dimensions: %dx%d, MaxGray= %d\n\nImage data= \n%s\n", 
//																						ref.magicNumber, ref.width, ref.height, ref.maxGray, (char *) ref.imageData);
//																						}
//
//																						//Procedure for debugging purposes
//																						//to check if comments have been 
//																						//stored correctly
//																						void printComments(PgmImage *this)
//																						{
//																							for(int i=0; i<this->numComments; ++i) {
//																									printf("Data in comments[%d] = %s\n", i, this->comments[i]);
//																										}
//																										}
//
