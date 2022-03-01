
#ifndef _MACRO_HEADER_
#define _MACRO_HEADER_

#define SIZE_LINE 81

/************************************************************************************************************************************************************************/

/* insert all macros decleared in the input file on the output file                                                                                                     */

void insertMacros(char *inputFileName, char *outputFileName); 

/************************************************************************************************************************************************************************/

/* deletes all the empty spaces inside the string                                                                                                                       */

void remove_spaces(char* str); 

/************************************************************************************************************************************************************************/

/* counts the amount of macro decleration inside the file                                                                                                               */

int count_macros(char *inputFileName); 

/************************************************************************************************************************************************************************/

/* opens a new file inside of which the text of the input file is copied after inserting the macros                                                                     */

void paste_macros(char *inputFileName, char *outputFileName, int number_of_macros); 

/************************************************************************************************************************************************************************/

typedef struct preAssemblerMacros {
    
    char name[SIZE_LINE];
    char content[SIZE_LINE][SIZE_LINE];
    int numOfLines;
    
} preAssemblerMacros;

/************************************************************************************************************************************************************************/

#endif