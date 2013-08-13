#include "parser.h"

void printError (char* expected, token* found, bool* errorFree)
{
    printf("Error: Program Expecting %s found %s (line# %i)\n", expected ,found->id, found->location);
    *errorFree = false;
}
/*--------------------------------------------------------------------------------------------------------------
parse
Inputs:  -the scanner that is generating tokens
Parses the input being scanned by a scanner
--------------------------------------------------------------------------------------------------------------*/
bool parse (scanner* scan)
{
     
     bool errorFree = true;
     
     //get the first token
     token curr_token = runScanner(scan);
     
     //check for PROGRAM
     if (curr_token.code != 1)
        printError("PROGRAM",&curr_token, &errorFree);
       
     //get the next token and <prog-name>
     //I'm not putting this in a function because
     //I'm just checking for one token that is used
     //nowhere else in the program
     curr_token = runScanner(scan);
     if (curr_token.code != 22)
        printError("<program name>",&curr_token, &errorFree);
     
     //check for VAR
     curr_token = runScanner(scan);
     if (curr_token.code != 2)
        printError("VAR",&curr_token, &errorFree);
        
     //get the next token and check for <dec-list>
     curr_token = runScanner(scan);
     parse_dec_list (scan,&curr_token, &errorFree);
        
     //check for BEGIN
     if (curr_token.code != 3)
        printError("BEGIN",&curr_token, &errorFree);
        
     //get the next token and check for <stmt-list>
     curr_token = runScanner(scan);
     parse_stmt_list (scan,&curr_token, &errorFree);
        
     //check for END.
     if (curr_token.code != 5)
        printError("END.",&curr_token,  &errorFree);
      
     return errorFree;
     
}

/*--------------------------------------------------------------------------------------------------------------
parse_dec_list
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
dec_list production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_dec_list (scanner* scan, token* curr_token, bool* errorFree)
{
     //look for <dec>
     parse_dec (scan, curr_token, errorFree);
     
     //optionally, there may be more <dec>s deliminated by semicolons
     while (curr_token->code == 12)
     {
         *curr_token = runScanner(scan);
         parse_dec (scan,curr_token, errorFree);
          
     }  
}
/*--------------------------------------------------------------------------------------------------------------
parse_dec
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
dec production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_dec (scanner* scan, token* curr_token, bool* errorFree)
{
     
     //<id:list>
     parse_id_list (scan, curr_token, errorFree);
 
     //:
     if (curr_token->code != 13)
        printError(":",curr_token, errorFree);
     
     //<type>
     *curr_token = runScanner(scan);
     parse_type(scan,curr_token,errorFree);
     
}
/*--------------------------------------------------------------------------------------------------------------
parse_id_list
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
id_list production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_id_list (scanner* scan, token* curr_token, bool* errorFree)
{
     //check for identifier
     if (curr_token->code != 22)
        printError("Identifier",curr_token,errorFree);
     
     //look for more identifiers seperated by commas
     *curr_token = runScanner(scan);
     while (curr_token->code == 14)
     {
         *curr_token = runScanner(scan);
         if (curr_token->code != 22)
             printError("Identifier",curr_token,errorFree);
         *curr_token = runScanner(scan);
     }
     
}
/*--------------------------------------------------------------------------------------------------------------
parse_type
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
type production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_type(scanner* scan, token* curr_token, bool* errorFree)
{
     //check for INTEGER
     if (curr_token->code != 6)
        printError("INTEGER",curr_token, errorFree);
     *curr_token = runScanner(scan);
     
}
/*--------------------------------------------------------------------------------------------------------------
parse_stmt_list
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
stmt_list production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_stmt_list (scanner* scan, token* curr_token, bool* errorFree)
{
     //look for <dec>
     parse_stmt (scan, curr_token, errorFree);
     
     //optionally, there may be more <dec>s deliminated by semicolons
     while (curr_token->code == 12)
     {
         *curr_token = runScanner(scan);
         parse_stmt (scan,curr_token, errorFree);
          
     }
}

/*--------------------------------------------------------------------------------------------------------------
parse_assign
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
assign production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_assign (scanner* scan, token* curr_token, bool* errorFree)
{
     
  //id
     //:=
 //    *curr_token = runScanner(scan);
     if (curr_token->code != 15)
        printError(":=",curr_token, errorFree);
        
     //<exp>
     *curr_token = runScanner(scan);
     parse_expression (scan, curr_token, errorFree);
     
}
/*--------------------------------------------------------------------------------------------------------------
parse_expression
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
expression production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_expression (scanner* scan, token* curr_token, bool* errorFree)
{
     //<term>
     parse_term (scan, curr_token, errorFree);
     
     //optionally, there may be more <terms> deliminated by + or -
     while ((curr_token->code == 16) || (curr_token->code == 17))
     {
         *curr_token = runScanner(scan);
         parse_term (scan,curr_token, errorFree);
          
     }
}
/*--------------------------------------------------------------------------------------------------------------
parse_term 
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
term production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_term (scanner* scan, token* curr_token, bool* errorFree)
{
     //<factor>
     parse_factor (scan, curr_token, errorFree);
     
     //optionally, there may be more <factor> deliminated by * or DIV
     while ((curr_token->code == 18) || (curr_token->code == 19))
     {
         *curr_token = runScanner(scan);
         parse_factor (scan,curr_token, errorFree);
     }   
}
/*--------------------------------------------------------------------------------------------------------------
parse_factor
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
factor production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_factor (scanner* scan, token* curr_token, bool* errorFree)
{
     switch (curr_token->code)
     {
         //ID
         case 22:
              //found an identifier, so you are done
              break;
         //INTEGER
         case 23:
              //found an integer, so you are done
              break;
         //(EXPRESSION)
         case 20: //(
              //<exp>
              *curr_token = runScanner(scan);
              parse_expression (scan,curr_token, errorFree);
              //)
              *curr_token = runScanner(scan);
              if (curr_token->code != 21)
                 printError(")",curr_token, errorFree); 
              break;
         default:
              printError("Id, Expression, or expression",curr_token,errorFree);
         }
         
         *curr_token = runScanner(scan);      
}
/*--------------------------------------------------------------------------------------------------------------
parse_read
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
read production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_read_write (scanner* scan, token* curr_token, bool* errorFree)
{
     //(
     if (curr_token->code != 20)
         printError(")",curr_token, errorFree); 
    //<exp>
    *curr_token = runScanner(scan);
    parse_expression (scan,curr_token, errorFree);
    //)
    if (curr_token->code != 21)
        printError(")",curr_token, errorFree);
        
    *curr_token = runScanner(scan); 
     
}
/*--------------------------------------------------------------------------------------------------------------
parse_for
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
for production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_for (scanner* scan, token* curr_token, bool* errorFree)
{
     parse_index_exp (scan, curr_token, errorFree);
     
     //do
     if (curr_token->code != 11)
        printError("DO",curr_token, errorFree);
        
     //<body>
     *curr_token = runScanner(scan);
     parse_body (scan, curr_token, errorFree); 
     
}
/*--------------------------------------------------------------------------------------------------------------
parse_index_exp
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
index_exp production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_index_exp (scanner* scan, token* curr_token, bool* errorFree)
{
     //identifier
     if (curr_token->code != 22)
        printError("identifier",curr_token, errorFree);
     
     //:=
     *curr_token = runScanner(scan);
     if (curr_token->code != 15)
        printError(":=",curr_token, errorFree);
        
     //<exp>
     *curr_token = runScanner(scan);
     parse_expression (scan, curr_token, errorFree);
     
     //TO
     if (curr_token->code != 10)
        printError("to",curr_token, errorFree);
        
     //<exp>
     *curr_token = runScanner(scan);
     parse_expression (scan, curr_token, errorFree); 
      
}
/*--------------------------------------------------------------------------------------------------------------
parse_body
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
body production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_body (scanner* scan, token* curr_token, bool* errorFree)
{
     //BEGIN
     if (curr_token->code == 3)
     {            
         //<stmt-list>
         *curr_token = runScanner(scan);
         parse_stmt_list (scan, curr_token, errorFree);
         
         //END
         if (curr_token->code != 4)
            printError("END",curr_token, errorFree);
         *curr_token = runScanner(scan);     
                   
     }
     else
         parse_stmt (scan, curr_token, errorFree);
  
}
/*--------------------------------------------------------------------------------------------------------------
parse_stmt
Inputs:  -the scanner that is generating tokens
         - The current token being examined
Parses the input starting at the point marked by the current token to see if it matches the format of the
stmt production rule of the MINI-P grammar
--------------------------------------------------------------------------------------------------------------*/
void parse_stmt (scanner* scan, token* curr_token, bool* errorFree)
{
     
     switch (curr_token->code)
     {
         //assign
         case 22:
              *curr_token = runScanner(scan);  
              parse_assign (scan, curr_token, errorFree);
              break;
         //read / write
         case 8:
         case 9:
              *curr_token = runScanner(scan);  
              parse_read_write (scan, curr_token, errorFree);
              break;
         //for
         case 7:
              *curr_token = runScanner(scan);  
              parse_for (scan, curr_token, errorFree);
              break;
         default:
              printError("Start of statement",curr_token,errorFree);    
     }
     
}
//     printf("\n4|%s (%i) -%i|\n", curr_token->id, curr_token->location ,curr_token->location);
