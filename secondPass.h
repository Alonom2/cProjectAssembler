#ifndef _SECOND__PASS__HEADR_
#define _SECOND__PASS__HEADR_

#include "macro.h"
#include "tables.h"
#include "shortFuncs.h"

int labelcmp(code_image *codeImage, symbol_table *symbolPointer);

code_line *reverseLinkedList(code_line *newCodeLine);

#endif