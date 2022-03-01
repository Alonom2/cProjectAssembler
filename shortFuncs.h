#ifndef _SHORT__FUNCS_HEADER_
#define _SHORT__FUNCS_HEADER_

#include "firstPass.h"
#include "tables.h"
#include "macro.h"

/************************************************************************************************************************************************************************/    

typedef enum bool
{
    FALSE = 0,
    TRUE = 1

} bool;

/************************************************************************************************************************************************************************/    

/* changes an integer to its hexadecimal value                                                                                                                          */
                                       
char *changeIntToHexa(int n);

/************************************************************************************************************************************************************************/    

/* changes char (representing a one digit integer) to its hexadecimal value                                                                                             */                                                                                    

char changeIntToHexaChar(int n);

/************************************************************************************************************************************************************************/    
    

/* checks if string ".entry" is part of the input.
   retruns 1 if it is, 0 if it isn't                                                                                                                                    */

int is_entry(char* currentLine);

/************************************************************************************************************************************************************************/    
  

/* checks if string ".extern" is part of the input.
   retruns 1 if it is, 0 if it isn't                                                                                                                                    */

int is_extern(char* currentLine);

/************************************************************************************************************************************************************************/    

/* checks if char ':' is part of the input (which indicates a label)
   retruns 1 if it is, 0 if it isn't                                                                                                                                    */

int is_label(char* currentLine);

/************************************************************************************************************************************************************************/    

/* checks if string ".string" is part of the input.
   retruns 1 if it is, 0 if it isn't                                                                                                                                    */

int is_string(char* currentLine);

/************************************************************************************************************************************************************************/    
 

/* checks if string ".data" is part of the input.
   retruns 1 if it is, 0 if it isn't                                                                                                                                    */

int is_data(char* currentLine);

#endif