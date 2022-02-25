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

/*************************function: findSizeSymbol**************************/ 

/* The input is a string (line of an assembly code that contains a lable)
   The output is the amount of chars in the label's name                   */

int findSizeSymbol(char* currentLine);

/*************************function: extractLabel****************************/ 

/* The input is a string (line of assembly code that contains a lable)
   The output is the label (string)                                        */
    
char* extractLabel(char* currentLine);

/*************************function: processToSymbolTable********************/ 

/* This function adds a label (which is part of the input) to  
   symbolTable struct defined in "tables.h".

   The input is a string (the label), pointer to int (the IC -
   Instruction Counter), symbol_attribute (defined in "tables.h") and
   pointer to symbolTable (defined in "tables.h") where it will be added.

   The output is a pointer to symbolTable
   (the same one the function added the label to)                          */    

symbol_table *processToSymbolTable(char* label, int* IC, symbol_attribute attribute, symbol_table *symbolTable);

/*************************function: amountOfData****************************/ 

/* The input is a string (part of assembly code's line that contains data)
                                          */   

int amountOfData(char* currentLine);

/*******************************extractData*******************************/    

void extractDataToCodeImage(char* currentLine, int* DC);

#endif