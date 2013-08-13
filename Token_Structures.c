/*
James Reatherford
Token_Structures.c
_____________________
Implementation File

Contains the structures to create tokens and a symbol table of tokens
along with functions to perform the following operations:
      -Add a token to the table
      -Get Token's Code given its ID
      -A hashing Function to be called by the previous two functions

*/

#include "Token_Structures.h"

/*--------------------------------------------------------------------------------------------------------------
hash

Inputs: the id to be hashed
Outputs: the hashed value

Simply hashes a token's id into a value that will
work as an address in the symbol table
--------------------------------------------------------------------------------------------------------------*/
int hash (char tokenName[BUFFER_SIZE])
{
    int total = 0;
    int counter = 0;
    
    for (counter; counter < BUFFER_SIZE; counter++)
    {
        //for the love of god, stop at a null character
        if (tokenName[counter] == '\0')
             break;
        else
             total += (int)tokenName[counter];
    }
    return total%TABLE_SIZE;
}
//--------------------------------------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------------------------------------
getTokenCode

Inputs: the symbol table to be appended, the id of the token to add, the code of the token to add
Outputs: None

Searches the symbol table for a token whoose id matches the second paramter.
If it is found, it exits *without changing the token's information*.
Otherwise, it adds the token to the table
--------------------------------------------------------------------------------------------------------------*/
void addToTable (token* table[TABLE_SIZE], char tokenName[BUFFER_SIZE], int tokenCode)
{
     int hashLocation = hash(tokenName);
     token* currentToken;
     //If the new token hashes to an empty space...
     if (table[hashLocation] == NULL)
     {
         //...allocate new memory from that space and select it
         table[hashLocation] = malloc(sizeof(token)); 
         currentToken = table[hashLocation];               
     }
     //Else If the new token hashes to a place that would cause a collission...
     else
     {   
         //start at the first token in the slot
         currentToken = table[hashLocation];
         //while the token we are at does not point to an empty space
         while (currentToken->next != NULL)
         {
               //go to the empty space
               currentToken = currentToken->next;
               //if this space matches our token, then exit the function      
               if (strcmp(tokenName, currentToken->id) == 0)           
                    return;              
         }
         //now that we have found an empty space, 
         //allocate memory for it and travel there
         currentToken->next = malloc(sizeof(token));
         currentToken = currentToken->next;
     }
     //set token information            
     strcpy(currentToken->id,tokenName);  
     currentToken->code = tokenCode;
     currentToken->next = NULL;  
     return;
     
}
//--------------------------------------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------------------------------------
getTokenCode

Inputs: the symbol table to be searched, the id of the token being searched for
Outputs: The code of the desired token | 0

Searches the symbol table for a token whoose id matches the second paramter.
If found, it returns that token's code.  Otherwise, it returns 0.
--------------------------------------------------------------------------------------------------------------*/
int getTokenCode(token* table [TABLE_SIZE], char tokenName[BUFFER_SIZE])
{
     int hashLocation = hash(tokenName);
     token* currentToken = table[hashLocation];
     //if it hashes to an empty token, it must not be here
     if (table[hashLocation] == NULL)
        return -1;
     else
      {
         //while we don't have a match...
         while(strcmp(tokenName, currentToken->id) != 0)
         {
              //if our current token is the last in the line, then it must not be here
              if (currentToken->next == NULL)
                 return -1;
              else
                  currentToken = currentToken->next;
         }
         //Yay!  Found it!
         return currentToken->code;
      } 
}
//--------------------------------------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------------------------------------
populateSymbolTable

Inputs: the symbol table to be filled
Outputs: None

This functions simply fills a symbol table with the default tokens
--------------------------------------------------------------------------------------------------------------*/
void populateSymbolTable(token* table[TABLE_SIZE])
{
     addToTable(table, "PROGRAM",1); 
     addToTable(table, "VAR",2); 
     addToTable(table, "BEGIN",3); 
     addToTable(table, "END",4); 
     addToTable(table, "END.",5); 
     addToTable(table, "INTEGER",6); 
     addToTable(table, "FOR",7); 
     addToTable(table, "READ",8); 
     addToTable(table, "WRITE",9); 
     addToTable(table, "TO",10); 
     addToTable(table, "DO",11); 
     addToTable(table, ";",12); 
     addToTable(table, ":",13); 
     addToTable(table, ",",14); 
     addToTable(table, ":=",15); 
     addToTable(table, "+",16); 
     addToTable(table, "-",17); 
     addToTable(table, "*",18); 
     addToTable(table, "DIV",19); 
     addToTable(table, "(",20); 
     addToTable(table, ")",21);      
}
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
