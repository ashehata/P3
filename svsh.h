#ifndef _SVSH_H_
#define _SVSH_H_

#include <stdlib.h>

void initCmdPrompt(void);
void printCmdPrompt(void);
void user_command(ARG_LIST * argList, char * inputRedirect, char * outputRedirect);
void builtIn(int cmd, char * str, char * varName);
void makeTokenList(char * tokenType, char* tokenList, char * usage, ARG_LIST * argList); 
//void showTokens(TOKEN_LIST * tokenList);

TOKEN_LIST * tokenList;
ENVIRON_LIST * environList;
PROCESS_LIST * processList;

#endif 
