/*
James Reatherford
scanner.c
_____________________
Implementation File

Wrapper for the scanner.  
Condenses the whole thing into two functions and a structure
*/
#include "Scanner.h"

/*--------------------------------------------------------------------------------------------------------------
initScanner

Inputs:  -pointer to the file the input it coming from
         -(optional)pointer to a file to write information to (If you do not wish to output, set this to NULL)
                - If used, this file must be in a read/write mode
         -the preprocessing buffer

Initilizes the scanner
--------------------------------------------------------------------------------------------------------------*/
void initScanner(scanner* scan, FILE* input, FILE* output)
{
    scan->inFile = input;
    scan->outFile = output;
      
    initBuffer(&scan->preBuffer);
    fillPreBuffer(scan->inFile,scan->outFile,&scan->preBuffer);
    
    int i;
    for (i = 0; i < TABLE_SIZE; i++)
        scan->symbolTable[i] = NULL;
    populateSymbolTable(scan->symbolTable);
}

/*--------------------------------------------------------------------------------------------------------------
fillPreBuffer

Outputs: -The next token

Gets the next token from the scanner
--------------------------------------------------------------------------------------------------------------*/
token runScanner(scanner* scan)
{
       return *getNextToken(scan->inFile,scan->outFile,&scan->preBuffer,scan->symbolTable);     
}
