#ifndef _FIRST__PASS__HEADR_
#define _FIRST__PASS__HEADR_

#include "macro.h"
#include "tables.h"

/*****************************bool_enum*************************************/    

typedef enum bool
{
    FALSE = 0,
    TRUE = 1

} bool;

/*************************function: is_entry********************************/    

/* checks if string ".entry" is part of the input.
   retruns 1 if it is, 0 if it isn't                                       */

int is_entry(char* currentLine);

/*************************function: is_extern*******************************/  

/* checks if string ".extern" is part of the input.
   retruns 1 if it is, 0 if it isn't                                       */

int is_extern(char* currentLine);

/*************************function: is_label********************************/ 

/* checks if char ':' is part of the input (which indicates a label)
   retruns 1 if it is, 0 if it isn't                                       */

int is_label(char* currentLine);

/*************************function: is_string*******************************/    

/* checks if string ".string" is part of the input.
   retruns 1 if it is, 0 if it isn't                                       */

int is_string(char* currentLine);

/*************************function: is_data*********************************/ 

/* checks if string ".data" is part of the input.
   retruns 1 if it is, 0 if it isn't                                       */

int is_data(char* currentLine);

/******************************findSizeSymbol*******************************/ 

int findSizeSymbol(char* currentLine);

/********************************extractLabel*******************************/    

char* extractLabel(char* currentLine);

/*******************************processSymbol*******************************/    

symbol_table *processToSymbolTable(char* label, int* IC, symbol_attribute attribute, symbol_table *symbolTable);

/*******************************amountOfData*******************************/    

int amountOfData(char* currentLine);

/*******************************extractData*******************************/    

void extractDataToCodeImage(char* currentLine, int* DC);

#endif