#include "secondPass.h"
#include <string.h>

int labelcmp(code_image *codeImage, symbol_table *symbolPointer)
{
    int dest_label_match = 1, dest_and_origin_match = 1, origin_label_match = 1;

    int counter = 0, close_to_null = 0;


    if ((strlen(codeImage->currCodeLine->label_2) == strlen(symbolPointer->symbol)))
    {
        for (counter = 0; counter < strlen(symbolPointer->symbol); counter++)
        {
            if (*(codeImage->currCodeLine->label_2 + counter) != *(symbolPointer->symbol + counter))
            {
                dest_label_match = 0;

                break;
            }
        }
    }

    else
    {
        dest_label_match = 0;
    }

    if (symbolPointer->next == NULL)
    {
        close_to_null = 1;
    }

    if (dest_label_match == 1 && close_to_null == 0)
    {
        if (strlen(codeImage->currCodeLine->label_1) == strlen(symbolPointer->next->symbol))
        {
            for (counter = 0; counter < strlen(symbolPointer->next->symbol); counter++)
            {
                if (*(codeImage->currCodeLine->label_1 + counter) != *(symbolPointer->next->symbol + counter))
                {
                    dest_and_origin_match = 0;

                    break;
                }
            }
        }

        else
        {
            dest_and_origin_match = 0;
        }
    }

    else
    {
        dest_and_origin_match = 0;
    }

    if (dest_and_origin_match == 0)
    {
        if (strlen(codeImage->currCodeLine->label_1) == strlen(symbolPointer->symbol))
        {
            for (counter = 0; counter < strlen(symbolPointer->symbol); counter++)
            {
                if (*(codeImage->currCodeLine->label_1 + counter) != *(symbolPointer->symbol + counter))
                {
                    origin_label_match = 0;

                    break;
                }
            }
        }

        else
        {
            origin_label_match = 0;
        }
    }

    else
    {
        origin_label_match = 0;
    }

    if (dest_and_origin_match == 1)
    {
        return (dest_and_origin_match + 2*dest_label_match);
    }

    else if (dest_label_match == 1)
    {
        return 2*dest_label_match;
    }

    else if (origin_label_match == 1)
    {
        return origin_label_match;
    }

    else
    {
        return dest_and_origin_match;
    }
}



code_line *reverseLinkedList(code_line *newCodeLine)
{
    code_line *tempLine;

    while (newCodeLine->next != NULL)
    {
        tempLine = newCodeLine->next;

        tempLine->back = newCodeLine;

        newCodeLine = newCodeLine->next;
    }

    return tempLine;
}