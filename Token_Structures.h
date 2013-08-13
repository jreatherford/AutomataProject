/*
James Reatherford
Token_Structures.h
_____________________
Header File

Contains the structures to create tokens and a symbol table of tokens
along with functions to perform the following operations:
      -Add a token to the table
      -Get Token's Code given its ID
      -A hashing Function to be called by the previous two functions
*/

#ifndef TOKEN_STRUCTURES_H
#define TOKEN_STRUCTURES_H

#include <stdbool.h>
#include <stdio.h>

/*
Constants
*/
#define TABLE_SIZE 64
#define BUFFER_SIZE 16

/*
Token Structure
*/
typedef struct tokenNode
{
       char id [BUFFER_SIZE];
       int code;
       int location;
       struct tokenNode* next;
} token;

/*
Defined operations for symbol table
*/
int hash (char tokenName[BUFFER_SIZE]);
void addToTable (token* table[TABLE_SIZE], char tokenName[BUFFER_SIZE], int tokenCode);
int getTokenCode(token* table [TABLE_SIZE], char tokenName[BUFFER_SIZE]);
void populateSymbolTable(token* table[TABLE_SIZE]);

#endif
