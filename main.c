#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "firstPass.h"
#include "shortFuncs.h"
#include "secondPass.h"



int main()
{
    char currentLine[SIZE_LINE];
    char previousLine[SIZE_LINE];
    FILE *fPointer1, *fPointer2, *fPointer3, *fPointer4;

    char command_table[16][5] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    int *IC, *DC, counter, wasLabel = 0, notSameSymbol = 1, close_to_null_code = 0, numOfCodeLines, numOfDataLines, baseFlag = 0;
    char *label;
    symbol_table *symbolPointer;

    command *commandPointer;
    code_image *codeImage; /*code image*/
    symbol_table **symbolTable; /*symbol table*/


/*************************Strings for tests*******************************/

    /*char currentLine2[] = "LOOP:  .data   +12, -5";
    char currentLine3[] = "OMER:  .data   +2, -9";
    char currentLine4[] = "STR: .string \"ab c  d\"";
    char currentLine5[] = "add r3 ,LIST";
    char currentLine6[] = "sub  r1, r4";
    char currentLine7[] = "bne END[r15]";
    char currentLine8[] = "stop";*/

/*************************************************************************/

    fPointer1 = fopen("eggs.am", "r");
    IC = (int *)malloc(sizeof(int)); 
    DC = (int *)malloc(sizeof(int));

    IC[0] = 100;
    DC[0] = 0;
    /*insertMacros("bacon.txt", "eggs.am");*/

    label = (char *)malloc(SIZE_LINE*sizeof(char));
    symbolTable = (symbol_table **)malloc(sizeof(symbol_table));
    codeImage = (code_image *)malloc(sizeof(code_image));
    commandPointer = (command *)malloc(sizeof(command));

    symbolTable[0] = NULL;
    codeImage->currCodeLine = NULL;
    codeImage->firstCodeLine = NULL;
    codeImage->firstDataLine = NULL;
    codeImage->lastCodeLine = NULL;
    codeImage->lastDataLine = NULL;


    /*label = extractLabel(currentLine); 
    symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, symbolTable[0]);*/

    /*pointer = symbolTable[0];*/ /*now the pointer gets the value of the first struct*/

    /*label = extractLabel(currentLine2);
    symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, symbolTable[0]);
    label = extractLabel(currentLine3);
    symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, symbolTable[0]);*/


/****************************************************FirstPass************************************************************/

    while(!feof(fPointer1))
    {
        for (counter = 0; counter < SIZE_LINE; counter++)
        {
            previousLine[counter] = currentLine[counter];
        }

        fgets(currentLine, SIZE_LINE, fPointer1);

        if (is_label(currentLine))
        {
            if (is_data(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, 0, symbolTable[0]);

                codeImage = extractDataToCodeImage(currentLine, IC, DC, codeImage);

                wasLabel = 1;

                continue;
            }

            else if (is_string(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, DATA_ATT, 0, symbolTable[0]);

                codeImage = extractStringToCodeImage(currentLine, IC, DC, codeImage);

                wasLabel = 1;

                continue;
            }

            else if (is_extern(currentLine))
            {
                label = extractLabel(currentLine);

                symbolTable[0] = processToSymbolTable(label, IC, EXTERN_ATT, 1, symbolTable[0]);

                if (wasLabel == 1)
                {
                    wasLabel = 0;
                }

                continue;
            }

            else if (is_entry(currentLine))
            {
                if (wasLabel == 1)
                {
                    wasLabel = 0;
                }

                continue;
            }
            
            else
            {
                remove_spaces(currentLine);

                if (*(currentLine) == '\0' || *(currentLine) == '\n')
                {
                    continue;
                }

                else
                {
                    label = extractLabel(currentLine);

                    symbolTable[0] = processToSymbolTable(label, IC, CODE_ATT, 0, symbolTable[0]);

                    updateCommandAndFunc(currentLine, command_table, commandPointer);

                    updateOperands(currentLine, commandPointer);

                    updateAddressingMethods(currentLine, commandPointer);

                    codeImage = extractCommandToCodeImage(IC, commandPointer, codeImage);

                    if (wasLabel == 1)
                    {
                        wasLabel = 0;
                    }
                }
            }
        }

        else
        {
            remove_spaces(currentLine);

                if (*(currentLine) == '\0' || *(currentLine) == '\n')
                {
                    continue;
                }

                else if (is_data(currentLine))
                {
                    if (is_data(previousLine) && wasLabel == 1)
                    {
                        codeImage = extractDataToCodeImage(currentLine, IC, DC, codeImage);
                    }
                }

                else if (is_string(currentLine))
                {
                    if (is_string(previousLine) && wasLabel == 1)
                    {
                        codeImage = extractStringToCodeImage(currentLine, IC, DC, codeImage);
                    }
                }

                else
                {
                    updateCommandAndFunc(currentLine, command_table, commandPointer);

                    updateOperands(currentLine, commandPointer);

                    updateAddressingMethods(currentLine, commandPointer);

                    codeImage = extractCommandToCodeImage(IC, commandPointer, codeImage);

                    if (wasLabel == 1)
                    {
                        wasLabel = 0;
                    }
                }
        }
    }

    fclose(fPointer1);

/****************************************************SecondPass***********************************************************/
    fPointer1 = fopen("eggs.am", "r");

    while(!feof(fPointer1))
    {
        fgets(currentLine, SIZE_LINE, fPointer1);

        if (is_entry(currentLine))
        {
            label = extractLabel(currentLine);

            symbolPointer = symbolTable[0];

            while (symbolPointer != NULL)
            {
                if (*(symbolPointer->symbol) == *(label) && strlen(symbolPointer->symbol) == strlen(label))
                {   
                    for (counter = 0; counter < strlen(label); counter++)
                    {
                        if (*(symbolPointer->symbol + counter) == *(label + counter))
                        {
                            continue;
                        }

                        else
                        {
                            notSameSymbol = 1;
                        }
                    }

                    if (notSameSymbol == 0)
                    {
                        symbolPointer->attribute_2 = ENTRY_ATT;
                    }
                }

                symbolPointer = symbolPointer->next;

                notSameSymbol = 0;
            }
        }
    }

    symbolPointer = symbolTable[0];

    while (symbolPointer != NULL)
    {
        codeImage->currCodeLine = codeImage->lastCodeLine;

        while (codeImage->currCodeLine != NULL)
        {
            if (codeImage->currCodeLine->next == NULL)
            {
                close_to_null_code = 1;
            }

            if (labelcmp(codeImage, symbolPointer) == 0 && close_to_null_code == 0)
            {
                codeImage->currCodeLine = codeImage->currCodeLine->next;

                continue;
            }

            else if (labelcmp(codeImage, symbolPointer) != 0)
            {
                if (codeImage->currCodeLine->code->hexaCode == NULL)
                {
                    if (labelcmp(codeImage, symbolPointer) > 0)
                    {
                        codeImage->currCodeLine->code->hexaCode = changeIntToHexa(symbolPointer->offset);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->code->ARE = EXTERNAL;
                        }

                        codeImage->currCodeLine->next->code->hexaCode = changeIntToHexa(symbolPointer->base);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->code->ARE = EXTERNAL;
                        }
                    }

                    if (labelcmp(codeImage, symbolPointer) == 3)
                    {
                        codeImage->currCodeLine->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->offset);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->next->code->ARE = EXTERNAL;
                        }

                        codeImage->currCodeLine->next->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->base);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->next->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->next->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->next->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->next->next->code->ARE = EXTERNAL;
                        }
                    }
                }

                else if (codeImage->currCodeLine->code->hexaCode != NULL)
                {
                    if (labelcmp(codeImage, symbolPointer) > 0)
                    {
                        codeImage->currCodeLine->next->code->hexaCode = changeIntToHexa(symbolPointer->offset);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->code->ARE = EXTERNAL;
                        }

                        codeImage->currCodeLine->next->next->code->hexaCode = changeIntToHexa(symbolPointer->base);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->next->code->ARE = EXTERNAL;
                        }
                    }

                    if (labelcmp(codeImage, symbolPointer) == 3)
                    {
                        codeImage->currCodeLine->next->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->offset);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->next->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->next->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->next->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->next->next->code->ARE = EXTERNAL;
                        }

                        codeImage->currCodeLine->next->next->next->next->code->hexaCode = changeIntToHexa(symbolPointer->next->base);

                        if (symbolPointer->attribute != EXTERN_ATT)
                        {
                            codeImage->currCodeLine->next->next->next->next->code->ARE = RELOCATEABLE;
                        }

                        else
                        {
                            if (baseFlag == 0)
                            {
                                symbolPointer->base = (codeImage->currCodeLine->next->next->next->next->address) - 1;
                                symbolPointer->offset = (codeImage->currCodeLine->next->next->next->next->address);
                                baseFlag++;
                            }

                            codeImage->currCodeLine->next->next->next->next->code->ARE = EXTERNAL;
                        }
                    }
                }
            }
            

            if (close_to_null_code == 1)
            {
                close_to_null_code = 0;

                baseFlag = 0;

                break;
            }

            codeImage->currCodeLine = codeImage->currCodeLine->next;
        }
        
        if (symbolPointer->next == NULL)
        {
            baseFlag = 0;

            break;
        }

        else
        {
            symbolPointer = symbolPointer->next;
        }
    }

    numOfCodeLines = codeImage->lastCodeLine->address - 100;

    numOfDataLines = codeImage->lastDataLine->address;

    codeImage->lastDataLine = reverseLinkedList(codeImage->lastDataLine);

    codeImage->lastCodeLine = reverseLinkedList(codeImage->lastCodeLine);

    fPointer2 = fopen("ps.ob", "w");

    for (counter = 0; counter <= numOfCodeLines; counter++)
    {
        if (codeImage->lastCodeLine->code->ARE == ABSOLUTE)
        {
            fprintf(fPointer2, "0%d  A%d-B%c-", 100 + counter, 4, codeImage->lastCodeLine->code->hexaCode[0]);
            fprintf(fPointer2, "C%c-", codeImage->lastCodeLine->code->hexaCode[1]);
            fprintf(fPointer2, "D%c-", codeImage->lastCodeLine->code->hexaCode[2]);
            fprintf(fPointer2, "E%c\n", codeImage->lastCodeLine->code->hexaCode[3]);

        }

        else if (codeImage->lastCodeLine->code->ARE == EXTERNAL)
        {
            fprintf(fPointer2, "0%d  A%d-B%c-", 100 + counter, 1, codeImage->lastCodeLine->code->hexaCode[0]);
            fprintf(fPointer2, "C%c-", codeImage->lastCodeLine->code->hexaCode[1]);
            fprintf(fPointer2, "D%c-", codeImage->lastCodeLine->code->hexaCode[2]);
            fprintf(fPointer2, "E%c\n", codeImage->lastCodeLine->code->hexaCode[3]);
        }

        else if (codeImage->lastCodeLine->code->ARE == RELOCATEABLE)
        {
            fprintf(fPointer2, "0%d  A%d-B%c-", 100 + counter, 2, codeImage->lastCodeLine->code->hexaCode[0]);
            fprintf(fPointer2, "C%c-", codeImage->lastCodeLine->code->hexaCode[1]);
            fprintf(fPointer2, "D%c-", codeImage->lastCodeLine->code->hexaCode[2]);
            fprintf(fPointer2, "E%c\n", codeImage->lastCodeLine->code->hexaCode[3]);
        }

        codeImage->lastCodeLine = codeImage->lastCodeLine->back; 
    }

    for (counter = 0; counter <= numOfDataLines; counter++)
    {
        fprintf(fPointer2, "0%d  A%d-B%c-", 101 + numOfCodeLines++, 4, codeImage->lastDataLine->code->hexaCode[0]);
        fprintf(fPointer2, "C%c-", codeImage->lastDataLine->code->hexaCode[1]);
        fprintf(fPointer2, "D%c-", codeImage->lastDataLine->code->hexaCode[2]);
        fprintf(fPointer2, "E%c\n", codeImage->lastDataLine->code->hexaCode[3]);

        codeImage->lastDataLine = codeImage->lastDataLine->back; 
    }

    fPointer3 = fopen("ps.ent", "w");

    fPointer4 = fopen("ps.ext", "w");

    while (symbolTable[0] != NULL)
    {
        if (symbolTable[0]->attribute == EXTERN_ATT)
        {
            fprintf(fPointer3, "%s BASE %d\n", symbolTable[0]->symbol, symbolTable[0]->base);
            fprintf(fPointer3, "%s OFFSET %d\n\n", symbolTable[0]->symbol, symbolTable[0]->offset);
        }

        if (symbolTable[0]->attribute_2 == ENTRY_ATT)
        {
            fprintf(fPointer4, "%s, %d, %d\n", symbolTable[0]->symbol, symbolTable[0]->base, symbolTable[0]->offset);
        }

        if (symbolTable[0]->next == NULL)
        {
            break;
        }
        
        symbolTable[0] = symbolTable[0]->next;
    }

    fclose(fPointer1);
    fclose(fPointer2);
    fclose(fPointer3);
    fclose(fPointer4);
    freeSymbolTable(symbolTable[0]);
    freeCodeImage(codeImage);
    free(IC);
    free(DC);
    freeCommand(commandPointer);
    

    return 0; 
}