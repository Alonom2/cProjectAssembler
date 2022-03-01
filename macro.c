
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"

/****************************************************************************insertMacros********************************************************************************/    

void insertMacros(char *inputFileName, char *outputFileName)
{
	int number_of_macros;

	number_of_macros = count_macros(inputFileName);

	paste_macros(inputFileName, outputFileName, number_of_macros);
        
}    
/****************************************************************************remove_spaces*******************************************************************************/    

void remove_spaces(char* str)
{
    int count = 0;
    int i;

    for (i = 0; i < str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

/****************************************************************************count_macros********************************************************************************/    

int count_macros(char *inputFileName)
{
	char currentLine[SIZE_LINE];
	char *pMacro;
	int number_of_macros = 0;
	FILE* fPointer;

	fPointer = fopen(inputFileName, "r");

	while(!feof(fPointer))
	{
	    fgets(currentLine, SIZE_LINE, fPointer);
	    pMacro = strstr(currentLine, "macro");
	    
	    if (pMacro)
	    {
	        number_of_macros++;	        
	    }
	}
	
	fclose(fPointer);
	return number_of_macros;
}

/****************************************************************************paste_macros********************************************************************************/    

void paste_macros(char *inputFileName, char *outputFileName, int number_of_macros)
{
	char currentLine[SIZE_LINE];
	char *pMacro, *pEndm, *pMention;
	int insideMacroFlag = 0, printFlag = 0, macroNumber = 0, number_of_lines = 0;
	int counterNames, counterLines;
	preAssemblerMacros *pointerPreAssemblerMacros;
	FILE* fPointer1;
    FILE* fPointer2;

	pointerPreAssemblerMacros = (preAssemblerMacros*)malloc(number_of_macros * sizeof(preAssemblerMacros));
	fPointer1 = fopen(inputFileName, "r");
	fPointer2 = fopen(outputFileName, "w");

	while(!feof(fPointer1))
	{
	    fgets(currentLine, SIZE_LINE, fPointer1);
	    pMacro = strstr(currentLine, "macro");
	    pEndm = strstr(currentLine, "endm");
	    
	    if (pMacro && (insideMacroFlag == 0))
	    {
	        remove_spaces(pMacro);
	        strncpy((pointerPreAssemblerMacros + macroNumber)->name, pMacro + 5, SIZE_LINE);
	        insideMacroFlag = 1;
	        continue;
	    }
	    
	    else if (insideMacroFlag == 1 && (!pEndm))
	    {
	        strncpy((pointerPreAssemblerMacros + macroNumber)->content[number_of_lines], currentLine, SIZE_LINE); 
	        number_of_lines++;
	        continue;
	    }
	    
	    else if (insideMacroFlag == 1 && (pEndm))
	    {
	        insideMacroFlag = 0;
	        (pointerPreAssemblerMacros + macroNumber)->numOfLines = number_of_lines;
	        number_of_lines = 0;
	        macroNumber++;
	        continue;
	    }
	    
	    for (counterNames = 0; counterNames < macroNumber; counterNames++)
	    {
	        pMention = strstr(currentLine, (pointerPreAssemblerMacros + counterNames)->name);
	        
	        if (pMention)
	        {
	            for (counterLines = 0; counterLines < (pointerPreAssemblerMacros + counterNames)->numOfLines; counterLines++)
	            {
	                fprintf(fPointer2, "%s", (pointerPreAssemblerMacros + counterNames)->content[counterLines]);
	            }
	            
	            printFlag = 1;
	        }

	    }
	    
	    if (printFlag == 0)
	    {
            fprintf(fPointer2, "%s", currentLine);
	    }
	    
	    printFlag= 0;
	}

	free(pointerPreAssemblerMacros);
	fclose(fPointer1);
	fclose(fPointer2);
}