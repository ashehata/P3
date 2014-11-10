#ifndef _SVSH_H_
#define _SVSH_H_

#include <stdlib.h>

void initCmdPrompt(void);
void printCmdPrompt(void);
void user_command(ARG_LIST * argList, char * inputRedirect, char * outputRedirect);
void builtIn(int cmd, char * str, char * varName);

ENVIRON_LIST * environList;

#endif 
