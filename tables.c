#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"

command_table commandTable =

    {{"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"},
     {MOV_OP, CMP_OP, ADD_OP, SUB_OP, LEA_OP, CLR_OP, NOT_OP, INC_OP, DEC_OP,
      JMP_OP, BNE_OP, JSR_OP, RED_OP, PRN_OP, RTS_OP, STOP_OP},
     {NONE_FUNCT, NONE_FUNCT, ADD_FUNCT, SUB_FUNCT, NONE_FUNCT, CLR_FUNCT,
      NOT_FUNCT, INC_FUNCT, DEC_FUNCT, JMP_FUNCT, BNE_FUNCT, JSR_FUNCT, NONE_FUNCT, NONE_FUNCT, NONE_FUNCT, NONE_FUNCT},
     {NON_RELATIVE_ADDR, NON_RELATIVE_ADDR, NON_RELATIVE_ADDR, NON_RELATIVE_ADDR, DIRECT_ONLY_ADDR, NONE_ADDR, NONE_ADDR,
      NONE_ADDR, NONE_ADDR, NONE_ADDR, NONE_ADDR, NONE_ADDR, NONE_ADDR, NONE_ADDR, NONE_ADDR, NONE_ADDR},
     {DIRECT_OR_REGISTER_ADDR, NON_RELATIVE_ADDR, DIRECT_OR_REGISTER_ADDR, DIRECT_OR_REGISTER_ADDR, DIRECT_OR_REGISTER_ADDR,
      DIRECT_OR_REGISTER_ADDR, DIRECT_OR_REGISTER_ADDR, DIRECT_OR_REGISTER_ADDR, DIRECT_OR_REGISTER_ADDR, DIRECT_OR_RELATIVE_ADDR,
      DIRECT_OR_RELATIVE_ADDR, DIRECT_OR_RELATIVE_ADDR, DIRECT_OR_REGISTER_ADDR, NON_RELATIVE_ADDR, NONE_ADDR, NONE_ADDR}};

/*************************function: freeSymbolTable************************/

void freeSymbolTable(symbol_table *symbolTable)
{
    symbol_table *temp;

    while (symbolTable != NULL)
    {
        temp = symbolTable->next;

        free(symbolTable->symbol);

        free(symbolTable);
        
        symbolTable = temp;
    }
}

/**************************function: freeCodeImage*************************/
                                      
void freeCodeImage(code_image *codeImage)
{
    code_line *temp;

     while (codeImage->lastDataLine != NULL)
    {
        temp = codeImage->lastDataLine->next;

        free(codeImage->lastDataLine->code);

        free(codeImage->lastDataLine);
        
        codeImage->lastDataLine = temp;
    }
}
