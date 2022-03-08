#ifndef _FIRST__PASS__HEADR_
#define _FIRST__PASS__HEADR_

#include "macro.h"
#include "tables.h"

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains a label/symbol (all chars that appear before the char ':').
   The output is the amount of chars in the label.                                                                                                                      */

int findSizeSymbol(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains a label/symbol.
   The output is the label/symbol.        
   
   This function uses the function above - "findSizeSymbol".                                                                                                              */
    
char* extractLabel(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* This function adds a label to symbolTable struct defined in "tables.h", and retruns a pointer to it afterward.

   The input is a string (label), pointer to int (the IC - Instruction Counter), symbol_attribute (defined in "tables.h") and pointer to symbolTable.                   */                                                                                                                        

symbol_table *processToSymbolTable(char* label, int* IC, symbol_attribute attribute, int isExtern, symbol_table *symbolTable);

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains integers (that may include '+' and '-' signs), seperated by ',' (commas).

   The output is the amount of chars in the first integer.
   if no integer is found inside the string, the output will be zero.       
   
   This function uses the function "remove_spaces" declared in "macro.h".                                                                                                 */   

int amountOfCharsInData(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains the word ".data:", and immediatly afterward integers (that may include '+' and '-' signs), seperated by ',' (commas).
   The output is the amount of integers declared inside the string after the word ".data:".      
   
   This function uses the function "remove_spaces" declared in "macro.h".                                                                                                 */

int amountOfDatas(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains the word ".string:", and immediatly afterward a sentence in quotation marks.
   The output is the amount of chars inside the quotation marks.                                                                                                        */
                                                                                               
int amountOfChars(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains the word ".data:", and immediatly afterward integers (that may include '+' and '-' signs), seperated by ',' (commas).
   
   This function allocates the needed amount of memory for all the integers inside the string, and puts them inside. The output is a pointer to the first integer.
   
   This function uses the functions "amountofDatas" and amountofCharsInData" declared above, and the function "remove_spaces" declared in "macro.h"                     */

int *turnDataLineToArray(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* The input is a string that contains the word ".string:", and immediatly afterward a sentence in quotation marks.

   This function allocates the needed amount of memory for all the chars inside the quotation marks, and puts them inside. The output is a pointer to the first char.
   
   This function uses the functions "amountofChars" declared above.                                                                                                     */

int *turnStringLineToArray(char* currentLine);

/************************************************************************************************************************************************************************/ 

/* This function updates the code_line "lastDataLine" inside a "code_image" struct defined in "tables.h", with the machine code translation
   of "currentLine" input (which contains '.data'), and returns a pointer to it afterward.

   The input is a string (assembly code with ".data"), pointer to int (the DC - Data Counter), and pointer to "code_image" struct.    
   
   This function uses the functions "amountOfDatas" and "turnDataLineToArray" declared above, and "extractDataLineToCodeImage" declared below.                          */                                                                                                                        

code_image *extractDataToCodeImage(char* currentLine, int *IC, int* DC, code_image *codeImage);

/************************************************************************************************************************************************************************/ 

/* This function updates the code_line "lastDataLine" inside a "code_image" struct defined in "tables.h", with the machine code translation
   of "currentLine" input (which contains '.string'), and returns a pointer to it afterward.

   The input is a string (assembly code with ".string"), pointer to int (the DC - Data Counter), and pointer to "code_image" struct.    
   
   This function uses the functions "amountOfDatas" and "turnStringLineToArray" declared above, and "extractDataLineToCodeImage" declared below.                          */

code_image *extractStringToCodeImage(char* currentLine, int *IC, int* DC, code_image *codeImage);

/************************************************************************************************************************************************************************/ 

/* This function updates a code_line with the machine code translation to input integer n.

   The input is an integer, pointer to int (the DC - Data Counter), and pointer to "code_line" struct.    
   
   This function uses the function "changeIntToHexa" declared in "shortFuncs.h"                                                                                         */                                                                                                                        

code_line *extractDataLineToCodeImage(int n, int *IC, int* DC, code_line *codeLine);

/************************************************************************************************************************************************************************/ 

code_image *extractCommandToCodeImage(int* IC, command* new_command, code_image *codeImage);

/************************************************************************************************************************************************************************/

code_line *extractCodeLineToCodeImage(int word, int* IC, char *hexaChar, code_line *codeLine, command* new_command);

/************************************************************************************************************************************************************************/

char CodeLineSecondWordToMachineCode(command *new_command, int which);

/************************************************************************************************************************************************************************/ 

int numOfRegisterIndex_ADDR(command *new_command);

#endif