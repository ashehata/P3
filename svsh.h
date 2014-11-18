#ifndef _SVSH_H_
#define _SVSH_H_

#include <stdlib.h>

void initCmdPrompt(void);
void printCmdPrompt(void);
void user_command(ARG_LIST * argList, char * inputRedirect, char * outputRedirect);
void builtIn(int cmd, char * str, char * varName);
void makeTokenList(char * tokenType, char* tokenList, char * usage, ARG_LIST * argList); 
//void showTokens(TOKEN_LIST * tokenList);

#define __NR_SaveVariable 314
#define __NR_GetVariable 315
#define __NR_NextVariable 316

TOKEN_LIST * tokenList;
ENVIRON_LIST * environList;

#endif 
