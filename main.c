#include <stdio.h>
#include <stdbool.h>
//#include "Token_Structures.h"
//#include "Buffers.h"
#include "scanner.h"
#include "parser.h"


int main (int argc, char* argv [])
{   
    //Check the command line
    if (argc != 2)
    {
       printf("Incorrect Command Line Arguments\n");
       return 0;
    }
      
    //Open input and tmp files and verify 
    FILE* inFile = fopen(argv[1],"r");
    FILE* outFile = fopen("tmp.txt","w+");
    
    if((inFile == NULL) || (outFile == NULL))
    {
      printf("FILE ACCESS ERROR\n");
      return 0;
    }
    
    //create and initilize the scanner
    scanner inputScanner;
    initScanner(&inputScanner, inFile, outFile);
    
    //run the scanner
    bool parseResult = parse(&inputScanner);
    if (parseResult == true)
       printf("Parse Successful!\n");
    
    //Print the source to the screen
    char c;
    printf("\nLINE#\tSOURCE OUTPUT:\n");
    printf("____________________________\n");
    rewind(outFile);
    int i = 1;
    printf("%i\t",i++);
    while((c = getc(outFile)) != EOF)
    {
       printf("%c",c);
       if (c == '\n')
          printf("%i\t",i++);
    }
    printf("\n");
    
    //close the files and delete the temporary file
    fclose(inFile);
    fclose(outFile);
    remove("tmp.txt");
    
    system("pause");
    return 0;
}
