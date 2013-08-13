/*
James Reatherford
scanner.h
_____________________
Implementation File

Wrapper for the scanner.  
Condenses the whole thing into two functions and a structure
*/

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdbool.h>
#include "Token_Structures.h"
#include "Buffers.h"


/*
Structure to hold scanner information
*/
typedef struct scannerStruct
{
    FILE* inFile;
    FILE* outFile;
    buffer preBuffer;
    token* symbolTable [TABLE_SIZE];
} scanner;

/*--------------------------------------------------------------------------------------------------------------
initScanner

Inputs:  -pointer to the file the input it coming from
         -(optional)pointer to a file to write information to (If you do not wish to output, set this to NULL)
                - If used, this file must be in a read/write mode
         -the preprocessing buffer

Initilizes the scanner
--------------------------------------------------------------------------------------------------------------*/
void initScanner(scanner* scan ,FILE* input, FILE* output);

/*--------------------------------------------------------------------------------------------------------------
fillPreBuffer

Outputs: -The next token

Gets the next token from the scanner
--------------------------------------------------------------------------------------------------------------*/
token runScanner();

#endif
