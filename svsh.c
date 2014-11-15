//
//  svsh.c
//  svsh
//
//  Created by Aly Shehata on 11/9/14.
//  Copyright (c) 2014 Aly Shehata. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "svsh_structs.h"
#include "svsh.h"
#include "y.tab.h"

char* showTokens = "0";

char* shellName = "svsh> ";

int argCount = 1;


void displayTokens(TOKEN_LIST * tokenList){
        TOKEN_LIST *myToken = tokenList;
	 while(myToken != NULL){
		printf("Token Type: %-20s Token: %-20s Usage: %-20s\n", myToken -> tokenType, myToken -> token, myToken->usage);
        myToken = myToken->next;
        }
}

void makeTokenList(char * tokenType, char * token, char * usage,ARG_LIST * argList )                   
{                                                                                                                  
	if(strcmp(showTokens,"1")==0){
		ARG_LIST * currentArg = argList;
		if(currentArg != NULL){
			while(currentArg != NULL){
				printf("Token Type: %-20s Token: %-20s Usage: arg %d\n", tokenType, currentArg->word, argCount);
				currentArg = currentArg->next;
				argCount++;
			}
		}
		else if(strcmp(usage,"arg")==0){
			printf("Token Type: %-20s Token: %-20s Usage: arg %d\n", tokenType, token, argCount);
			argCount++;
		}
		else{
			TOKEN_LIST * tokenList = NULL;
			TOKEN_LIST * newToken = malloc(sizeof(TOKEN_LIST));
			strncpy(newToken -> tokenType, tokenType, sizeof(newToken->tokenType));   
        		strncpy(newToken -> token, token, sizeof(newToken->token));
			strncpy(newToken -> usage, usage, sizeof(newToken->usage));
        		newToken->next = tokenList;
			tokenList = newToken;
			displayTokens(tokenList);
		}
	}                                                                               
}

void initCmdPrompt(void){
    
    
}
void printCmdPrompt(void){
    printf("%s", shellName);
    argCount = 1;
}


void addToEnvList(char * varName, char * varValue)
{
    /* Check if variable exists in ENVIRON_LIST */
    ENVIRON_LIST * currentEntry = environList;
    int found = 0;
    while(currentEntry != NULL && !found)
    {
        if(strcmp(currentEntry->varName, varName) == 0)
        {
            strncpy(currentEntry->varValue, varValue,
                    sizeof(currentEntry->varValue));
            found = 1;
            
        }
        currentEntry = currentEntry->next;
    }
    /*
     * If no existing entry was found, create a new entry
     * at the start of the list
     */
    if(!found)
    {
        /* Create new entry */
        ENVIRON_LIST * newEntry = malloc(sizeof(ENVIRON_LIST));
        strncpy(newEntry->varName, varName, sizeof(newEntry->varName));
        strncpy(newEntry->varValue, varValue, sizeof(newEntry->varValue));
        newEntry->prev = NULL;
        /* Point newEntry->next to start of global list */
        newEntry->next = environList;
        if(environList != NULL)
            environList->prev = newEntry;
        /* Change the global pointer to start at newEntry */
        environList = newEntry;
    }
}	

void listEnv(){
    ENVIRON_LIST *myEntry = environList;
    while (myEntry != NULL){
        printf("%s=%s\n", myEntry->varName, myEntry->varValue);
        myEntry = myEntry->next;
    }
}

void builtIn(int cmd, char * str, char * varName){
    switch(cmd){
        case(DEFPROMPT):{
            shellName = str;
            break;
        }
            
        case(CD):{
            if (chdir(str) < 0)
                perror("Change Directory");
            break;
        }
        case(EQUALTO):{
		//printf("varName = %s", varName);
		if(strcmp(varName, "$ShowTokens") ==0){
			showTokens = str;
		}
            addToEnvList(varName, str);
            break;
        }
        case(LISTJOBS):{
            listEnv();
            break;
        }
        case(BYE):{
	    exit(0);
            break;
        }
            
        
    }
}

void user_command(ARG_LIST * argList, char * inputRedirect, char * outputRedirect){
	int old_stdout = dup(1);
    int argListCount = 0;
    int envListCount = 0;
    ARG_LIST * myArglist = argList;
    char output[4096];
    while(myArglist != NULL)
    {
        argListCount++;
        myArglist = myArglist->next;
    }
    char ** argv = malloc((argListCount + 1) * sizeof(char[INPUT_LIMIT]));
    /* Copy word list to argv */
    myArglist = argList;
    int i = 0;
	//ENVIRON_LIST * varVal = environList;
	ENVIRON_LIST * environListIterator = environList;
    while(myArglist != NULL)
    {
	environListIterator = environList;
	argv[i] = myArglist->word;
	while(environListIterator != NULL && environListIterator->varName != myArglist->word){
		if(strcmp(environListIterator->varName, myArglist->word)==0){
             		argv[i] = environListIterator->varValue;
        	}

		environListIterator = environListIterator -> next;
	}

	myArglist = myArglist->next;
        i++;
    }
    argv[i] = NULL; // Last element has to be null for exec to work properly.
   
    environListIterator = environList;
    while(environListIterator != NULL)
    {
        envListCount++;
        environListIterator = environListIterator->next;
    }
   
    /*
     * NOTE: The environ array has to be able to hold a string of size "[256]=[256]"
     */
    char ** environ = (char **) malloc((envListCount + 1) * sizeof(char *));
    /*
     * Create environ strings in VAR=VAL format for each variable
     * in the environList.
     */
    environListIterator = environList;
    i = 0;
    while(environListIterator != NULL)
    {
        environ[i] = malloc((2*INPUT_LIMIT)+1);
        strncpy(environ[i], environListIterator->varName, (2*INPUT_LIMIT + 3));
        strcat(environ[i], "=");
        strcat(environ[i], environListIterator->varValue);
        environListIterator = environListIterator->next;
        i++;
   }
    environ[i] = NULL;

    
    pid_t pid;
    int status;
   

    if((pid = fork()) == 0)
    {
        if(inputRedirect != NULL){
            //int old_stdout = dup(1);
            freopen ("/dev/null", "w", stdout); // or "nul" instead of "/dev/null"
        }
        if(execve(argv[0], argv, environ) < 0)
        {
            printf("%s: Command not found. \n", argv[0]);	
            fclose(stdout);		    
            exit(0);
        }
    }
    
    if(waitpid(pid, &status, 0) < 0)
    {
        perror("WAITPID");
        kill(pid, SIGKILL);
        FILE *fp;
        char ch;
        int len = 0;
        stdout = fdopen(old_stdout, "w"); 
        fp=fopen(outputRedirect,"r");
        if(inputRedirect != NULL){
	char buffer[INPUT_LIMIT];
	fgets(buffer, sizeof(buffer), stdin);
	printf("name: %s, val: %s",inputRedirect, buffer);
	addToEnvList(inputRedirect, buffer);
	}

	if(!fp) {
            printf("Cannot open file!\n");
            return;
        }
        ch=fgetc(fp);
        while(ch != EOF && ch!= '\n' && len < 30)
        {
            output[len] = ch;
            len++;
            ch=fgetc(fp);
        }
        output[len] = 0;
        printf("%s", output);
    }

	if(inputRedirect != NULL){
    char buffer[INPUT_LIMIT];
	FILE *fp;
	fp = popen(*argv, "r");
	int i = 0;
	char temp[256];
        //fgets(buffer, sizeof(buffer), fp);
        while (fgets(buffer, sizeof(buffer), fp) != NULL){
		//size_t len = strlen(temp);
		  strcat(temp, buffer);
		//puts(temp);
		//printf("%s",temp);
		i++;}
	temp[strlen(temp)] = '\0';
        addToEnvList(inputRedirect, temp);  
        }
    /*
     * Free memory allocated for argv and environ arrays
     */
    free(argv);
    for(i = 0; i <= envListCount; i++)
        free(environ[i]);
    free(environ);
    printf("%s", output);
}

