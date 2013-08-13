/*
James Reatherford
Buffers.c
_____________________
Implementation File

Contains the structures to buffers as well as operations that may need
to be performed on those buffers such as:
      -Fill a preprocessing buffer with information from a file
      -Insert information to a buffer (and optionally copy that data to a file)
      -Move data from a preprocessing buffer to a look ahead buffer
      -Analyze a look ahead buffer and add recognized tokens to a symbol table
*/
#include "Buffers.h"
#include <ctype.h>


void initBuffer(buffer* buf)
{
    buf->cursor = 0;
    buf->base = 0;
    buf->lineNumber = 1;
    buf->isInWhitespace = false;
    buf->isEmptyLine = false;
    buf->addedChar = false;
    buf->end = false;
}


/*--------------------------------------------------------------------------------------------------------------
fillPreBuffer

Inputs:  -pointer to the file the input it coming from
         -(optional)pointer to a file to write information to (If you do not wish to output, set this to NULL)
                - If used, this file must be in a read/write mode
         -the preprocessing buffer
Outputs: -OEF on end of file, otherwise 0.

Fills a preprocessing buffer with input from a file.  This removes extra whitespace and comments as well as 
converting all thext to uppercase.  Optionally, this function also copies the contents of the input buffer to
a file (for creating a formatted source listing)
--------------------------------------------------------------------------------------------------------------*/
int fillPreBuffer(FILE* input, FILE* output, buffer* pbuf)
{
    char currChar;     

     while (((pbuf->base == pbuf->cursor) || (pbuf->text[pbuf->cursor] == ' ') || (pbuf->text[pbuf->cursor] == '\n')) && (pbuf->addedChar == true))
     {
        if ((pbuf->text[pbuf->cursor] == '\n'))
           pbuf->lineNumber++;
        pbuf->cursor = (pbuf->cursor + 1)%BUFFER_SIZE;
        
     }
     
     //***begin the loop to fill the buffer***//
     while((pbuf->base != pbuf->cursor) || (pbuf->addedChar == false)){ 
        currChar = getc(input);
        
        //handle EOF
        if (currChar == EOF)
        {
            addCharToBuff(output, pbuf, '\0');
            pbuf->end = true;
            pbuf->base = pbuf->cursor;
            return EOF;              
        }
        
        if (avoidComments(input,output,pbuf,currChar) == true)
           continue;
           
        if (avoidWhitespace(input,output,pbuf,currChar) == true)
           continue;

        //handle lowecase letters
        if ((currChar >= 'a') && (currChar <= 'z'))
           currChar = (currChar - ('a'-'A'));            
        
        //whatever's left, send it through...
        addCharToBuff(output, pbuf, currChar);
        pbuf->isInWhitespace = false;
        pbuf->isEmptyLine = false;
        pbuf->addedChar = true;
     }      
     
     return 0;    
}
//--------------------------------------------------------------------------------------------------------------
bool avoidComments(FILE* input, FILE* output, buffer* pbuf, char currChar)
{
    //handle { } comments
    if(currChar == '{')
    {
        while (currChar != '}')
              currChar = getc(input);
        return true;          
    }
    else if (currChar == '}')
       return true;
    
    //handle // comments
    if (currChar == '/')
    {
       currChar = getc(input);
       if (currChar == '/')
       {
          while ((currChar != '\n') && (currChar != '\r'))
             currChar = getc(input);
          fseek(input, -(sizeof(char)), SEEK_CUR);
          return true;
       }
       else
       {
          fseek(input, -(sizeof(char)), SEEK_CUR);
          currChar = '/';
       }            
    } 
                 
    //handle (* *) comments
    if (currChar == '(')
    {
       currChar = getc(input);
       if (currChar == '*')
       {
          do 
          {
              currChar = getc(input);
              if (currChar == '*')
                 currChar = getc(input); 
                              
           }while(currChar != ')');
           
           return true;       
       }
       else
       {
          fseek(input, -(sizeof(char)), SEEK_CUR);
          currChar = '(';
       }            
    }
    else if (currChar == '*')
    {
       currChar = getc(input);
       if (currChar == ')')
           return true;       
       else
       {
          fseek(input, -(sizeof(char)), SEEK_CUR);
          currChar = '*';
       }            
    }
    return false;
}
//--------------------------------------------------------------------------------------------------------------
bool avoidWhitespace(FILE* input, FILE* output, buffer* pbuf, char currChar)
{
    
    //handle newline
    if ((currChar == '\r') || (currChar == '\n'))
    {
       if ((pbuf->isEmptyLine == false) && (pbuf->addedChar == true))
       {
          if (pbuf->isInWhitespace == true)
              fseek(input, -(sizeof(char)), SEEK_CUR);               
          addCharToBuff(output, pbuf, '\n');
       }
       pbuf->isEmptyLine = true;  
       pbuf->isInWhitespace = true;
       return true;    
    }    

    //handle whitespace
    if ((currChar == ' ') || (currChar == '\t') || (currChar == '\v') || (currChar == '\f'))
    {   
        if  (pbuf->isInWhitespace == false)
           addCharToBuff(output, pbuf, ' ');     
        pbuf->isInWhitespace = true; 
        return true;     
    }
    
    return false;
}
/*--------------------------------------------------------------------------------------------------------------
getNextToken

Inputs:
Outputs:

--------------------------------------------------------------------------------------------------------------*/
token* getNextToken(FILE* input, FILE* output, buffer* pbuf, token* table [TABLE_SIZE])
{
       
       bool bufferOverflow = false;

       //check for end of input
       if ((pbuf->text[pbuf->base] == '\0') && (pbuf->end == true))
          return NULL;

       //skip past whitespace at the start
       int start = pbuf->base;
       while ((pbuf->text[pbuf->cursor] == ' ') || (pbuf->text[pbuf->cursor] == '\n'))
       {
          if ((pbuf->text[pbuf->cursor] == '\n'))
              pbuf->lineNumber++;
          start = (start + 1)%BUFFER_SIZE;
          pbuf->cursor = (pbuf->cursor + 1)%BUFFER_SIZE;
       }
          
       //scan through until a delimiter is hit
       while ( (pbuf->text[pbuf->cursor] != ' ') &&
                (pbuf->text[pbuf->cursor] != ';') &&
                (pbuf->text[pbuf->cursor] != ',') &&
                (pbuf->text[pbuf->cursor] != '(') &&
                (pbuf->text[pbuf->cursor] != ')') &&
                (pbuf->text[pbuf->cursor] != '+') &&
                (pbuf->text[pbuf->cursor] != '-') &&
                (pbuf->text[pbuf->cursor] != '*') &&
                (pbuf->text[pbuf->cursor] != '\0') &&
                (pbuf->text[pbuf->cursor] != '\n')){
               
         pbuf->cursor = ((pbuf->cursor + 1)%BUFFER_SIZE);
         
         //check for buffer overflow
         if (pbuf->cursor == pbuf->base)
         {
            pbuf->cursor = ((pbuf->cursor - 1)%BUFFER_SIZE); 
            fillPreBuffer(input,output,pbuf);
            bufferOverflow = true;
         }
       }
       
       //create the token
       token* tmpToken = malloc(sizeof(token));   
       
       //set the token's name
       if(start == pbuf->cursor)
       {
                tmpToken->id[0]= pbuf->text[pbuf->cursor];
                tmpToken->id[1]= '\0';
                pbuf->cursor = ((pbuf->cursor + 1)%BUFFER_SIZE);
       }
       else
       {
           int i;
           for (i = 0; (i+start)%BUFFER_SIZE != pbuf->cursor; i++)
                  tmpToken->id[i] = pbuf->text[(start+i)%BUFFER_SIZE];
           tmpToken->id[i] = '\0';
       }
       
       //set the token location
       tmpToken->location = pbuf->lineNumber;
       
       //set the token code
       if (bufferOverflow == true)
          tmpToken->code = -2;
       else
           tmpToken->code = recognizeToken(tmpToken->id,table);
       
       //add the token to the symbol table
       addToTable (table, tmpToken->id, tmpToken->code);
       
//******************************************************************************
//                             DEBUG INFORMATION
//******************************************************************************
    /*  int t = 0;
      printf("\t<%s>\t",tmpToken->id);
       printf("<");
       for (t = 0; t < BUFFER_SIZE; t++)
       {
           if (t == pbuf->base)
              printf("[");
       
        if (pbuf->text[t] == '\n')
           printf("_");
        else if (pbuf->text[t] == '\0')
           printf("#");
        else
           printf("%c",pbuf->text[t]);
          
          if (t == pbuf->cursor)
              printf("]");
       }
       printf(">\n"); */
//******************************************************************************
//******************************************************************************
       
       //replace the text that has been read with new data
       if (pbuf->end == false)
           fillPreBuffer(input,output,pbuf);                        
       else if (pbuf->base == pbuf->cursor)
           pbuf->cursor = (pbuf->cursor+1)%BUFFER_SIZE;       
       else      
           pbuf->base = pbuf->cursor;

       return tmpToken;
       
}
//--------------------------------------------------------------------------------------------------------------
int recognizeToken(char* tokenName, token* table [TABLE_SIZE])
{
    //First check to see if the token is already in the table
     int inTable = getTokenCode(table,tokenName);
     if (inTable != -1)
         return inTable;
     
     bool isInt = true;
     bool isId = true;
     
     if (isalpha(tokenName[0]) == false)
        isId = false;
        
     int cursor;  
     for (cursor = 0; cursor < BUFFER_SIZE; cursor++)
     {
        if (tokenName[cursor] == '\0')
           break;
        else if (isdigit(tokenName[cursor]) == false)
           isInt = false;
        else if ((isalpha(tokenName[cursor]) == false) && 
                (tokenName[cursor] != '_') && 
                (isdigit(tokenName[cursor]) == false))
           isId = false;   
     } 
     
     if (isInt == true)
        return 23;
     else if (isId == true)
        return 22;
     else
        return -1;
}
//--------------------------------------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------------------------------------
addCharToBuff

Inputs:  -(optional)ile to output the character to (if you do not wish to output, set to NULL)
                 - If used, this file must be in a read/write mode
         -The buffer to add the character to
         -the character to add
Outputs: N/A

Adds a character to a buffer and (optinally) adds the character to a file
--------------------------------------------------------------------------------------------------------------*/
void addCharToBuff (FILE* output, buffer* pbuf, char c)
{
   if (output != NULL)
      putc(c, output);
   pbuf->text[pbuf->base] = c;
   pbuf->base = ((pbuf->base + 1)%BUFFER_SIZE);           
}
//--------------------------------------------------------------------------------------------------------------
