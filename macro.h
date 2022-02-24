
#ifndef _MACRO_HEADER_
#define _MACRO_HEADER_


#define SIZE_LINE 81

/******************************************************************************/

void insertMacros(char *inputFileName, char *outputFileName); /*insert all macros decleared in the input file on the output file*/

/******************************************************************************/

void remove_spaces(char* str); /*deletes all the empty spaces inside the string*/

/******************************************************************************/

int count_macros(char *inputFileName); /*counts the amount of macro decleration inside the file*/

/******************************************************************************/

void paste_macros(char *inputFileName, char *outputFileName, int number_of_macros); /*opens a new file inside of which the text of the input file is copied after inserting the macros*/

/******************************************************************************/

typedef struct preAssemblerMacros {
    
    char name[SIZE_LINE];
    char content[SIZE_LINE][SIZE_LINE];
    int numOfLines;
    
} preAssemblerMacros;

/******************************************************************************/

#endif