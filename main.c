#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "svsh_structs.h"
#include "svsh.h"
#include "y.tab.h"

extern int yyparse(void);

int main(int argc, char * argv[])
{
    initCmdPrompt();
    printCmdPrompt();
    int myStatus = yyparse();
    printf("\n");
    return myStatus;
}

























