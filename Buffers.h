/*
James Reatherford
Buffers.h
_____________________
Header File

Contains the structures to buffers as well as operations that may need
to be performed on those buffers such as:
      -Fill a preprocessing buffer with information from a file
      -Insert information to a buffer (and optionally copy that data to a file)
      -Move data from a preprocessing buffer to a look ahead buffer
      -Analyze a look ahead buffer and add recognized tokens to a symbol table
*/
#ifndef BUFFERS_H
#define BUFFERS_H

#include "Token_Structures.h"

/*
Buffer Structure
*/
typedef struct bufferStruct
{
  char text [BUFFER_SIZE];
  int cursor;
  int base;
  int lineNumber;
  bool isEmptyLine;  
  bool isInWhitespace;
  bool addedChar;
  bool end;
} buffer;

void initBuffer(buffer* buf);

int fillPreBuffer(FILE* input, FILE* output, buffer* pbuf);
void addCharToBuff (FILE* output, buffer* pbuf, char c);
bool avoidComments(FILE* input, FILE* output, buffer* pbuf, char currChar);
bool avoidWhitespace(FILE* input, FILE* output, buffer* pbuf, char currChar);
token* getNextToken(FILE* input, FILE* output, buffer* pbuf, token* table [TABLE_SIZE]);
int recognizeToken(char* tokenName, token* table [TABLE_SIZE]);

#endif
