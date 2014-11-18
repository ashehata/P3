%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "svsh_structs.h" 
#include "svsh.h"

extern void builtIn(int cmd, char * str, char * varName);
extern void user_command(ARG_LIST * argList, char * inputRedirect, char * outputRedirect);
int yylex(void);
void yyerror(char * s);
void printArgList(ARG_LIST * argList);
void printTokenList(TOKEN_LIST * tokenList);
ARG_LIST * makeArgList(char * arg, ARG_LIST * wordList);
extern void makeTokenList(char * tokenType, char * tokenList, char * usage, ARG_LIST * wordList);
#define __NR_SaveVariable 314

int yydebug = 1;

%}

/* Define the union for yylval */
%union {
	char * str;	/* string literals */
	char * word;	/* words */
	char * variable;
	char * metachar;
	ARG_LIST * argList;
	TOKEN_LIST * tokenList;
};

%token NEWLINE DEFPROMPT CD  ASSIGNTO LISTJOBS BYE BG RUN PATH
%token <str> STRING
%token <word> WORD
%token <metachar> METACHAR EQUALTO
%token <variable> VARIABLE SYS_VARIABLE
%type <argList> arg_list user_command
/*%type <tokenList> user_command*/
%type <str> command

%%

session:
	   	lines			{ /* Do nothing */ }	
	      | /* NULL */		{ /* Do nothing */ }
	      ;

lines:
		line			{ /* Do nothing */ }
	      |	lines line		{ /* Do nothing */ }
	      ;

line:
		command NEWLINE		{ printCmdPrompt(); }
	      | NEWLINE			{ printCmdPrompt(); }
	      | error NEWLINE		{ 
						yyerrok;
						yyclearin;
						printCmdPrompt();
					}
	      ;

command:
		DEFPROMPT STRING	{		builtIn(DEFPROMPT, $2, NULL); }
	      | PATH STRING		{ 		makeTokenList("word", "PATH", "variable_name", NULL);
							builtIn(PATH, $2, NULL); }
	      | VARIABLE EQUALTO STRING	{ 		builtIn(EQUALTO, $3, $1);
							makeTokenList("variable", $1,"variable_name", NULL);
							makeTokenList("metachar", "=", "assignment",NULL);
							makeTokenList("string", $3, "variable_def",NULL); }
	     /* | ASSIGNTO WORD STRING	{	makeTokenList("keyword", "assignto", "assignto");
						makeTokenList("word", $2, "variable_name");
						makeTokenList("",""
						 builtIn(ASSIGNTO, $3, $2); }*/
	      |	LISTJOBS			{ builtIn(LISTJOBS, NULL, NULL); }
	      |	CD WORD				{	makeTokenList("keyword", "cd", "cd", NULL);
							makeTokenList("word", $2, "arg", NULL);
							builtIn(CD, $2, NULL); }
	      |	BYE					{ builtIn(BYE, NULL, NULL); }
          | ASSIGNTO VARIABLE arg_list      { 		makeTokenList("keyword", "assignto", "assignto", NULL);
							makeTokenList("word", $2, "variable_name", NULL);
							makeTokenList("word", NULL, "arg", $3);
						//	makeTokenList("cmd", $3, "cmd", NULL);
						//	builtIn(EQUALTO, NULL, $2);
						//	$$ = makeArgList(NULL, $3);
							user_command($3, $2, NULL);
					}
	      | user_command		{
						user_command($1, NULL, NULL);
						free($1);
					}


                  | SYS_VARIABLE EQUALTO STRING {
                                                //saveGlobalVar System call                           
                                                syscall(__NR_SaveVariable,$1, $3);                   
                                                makeTokenList("global_variable", $1,"global_variable_name", NULL);
                                                makeTokenList("metachar", "=", "assignment",NULL);    
                                                makeTokenList("string", $3, "global_variable_def",NULL);       
                  }                                                                                   
              ; 
	

user_command:
	      /* WORD arg_list		{
						$$ = makeArgList($1, $2);
					}*/
	      RUN WORD			{	 makeTokenList("keyword", "run", "run", NULL);
                        		 	 makeTokenList("word", $2, "cmd", NULL);

						$$ = makeArgList($2, NULL);
					}
              |RUN WORD arg_list   {
						makeTokenList("keyword", "run", "run", NULL);
                                                makeTokenList("word", $2, "cmd", NULL);
                                                makeTokenList("word", NULL, "arg", $3);
                       				$$ = makeArgList($2,$3);
						//makeTokenList("word", NULL, "arg",$3);
          }
	     |RUN VARIABLE              {       makeTokenList("keyword", "run", "run", NULL);
                                                makeTokenList("variable", $2, "variable_name", NULL);
						$$ = makeArgList($2, NULL); } 

	     |RUN VARIABLE arg_list	{	makeTokenList("keyword", "run", "run", NULL);
                                                makeTokenList("variable", $2, "variable_name", NULL);
						makeTokenList("word", NULL, "arg", $3);
						$$ = makeArgList($2, $3); } 
        ;

arg_list:
		WORD			{ //makeTokenList("word", $1, "arg", NULL);
					  $$ = makeArgList($1, NULL); }
	      |	STRING			{ $$ = makeArgList($1, NULL); }
	      | VARIABLE		{ $$ = makeArgList($1, NULL); }
	/*      | VARIABLE arg_list	{ $$ = makeArgList($1, $2); }*/
	      | WORD arg_list		{ $$ = makeArgList($1, $2); }	
	      | STRING arg_list		{ $$ = makeArgList($1, $2); } 
	      ;

%%

ARG_LIST * makeArgList(char * arg, ARG_LIST * argList)
{
	/* Add the current word or string to the front of the arg_list. */
	/* The parser actually reads the last argument first. */
	ARG_LIST * newEntry = malloc(sizeof(ARG_LIST));
	strncpy(newEntry->word, arg, sizeof(newEntry->word));
	newEntry->next = argList;
	argList = newEntry;
	//printArgList(argList);
	return argList;
}
/*
TOKEN_LIST * makeTokenList(char * tokenType, char * token, char * usage,TOKEN_LIST * tokenList )
{
	TOKEN_LIST * newToken = malloc(sizeof(TOKEN_LIST));
	strncpy(newToken -> tokenType, tokenType, sizeof(newToken->tokenType));
	strncpy(newToken -> token, token, sizeof(newToken->token));
	strncpy(newToken -> usage, usage, sizeof(newToken->usage));
	newToken->next = tokenList;
	tokenList = newToken;
	//printTokenList(tokenList);
	return tokenList;
}
*/	 
void printTokenList(TOKEN_LIST * tokenList)
{
	TOKEN_LIST * currentToken = tokenList;
	//printf("Tokens:\n");
	while(currentToken != NULL)
	{
		printf("Token Type: %-20s Token: %-20s Usage:  %-20s\n", currentToken->tokenType, currentToken->token, currentToken->usage);
		currentToken = currentToken->next;
	}
}
void printArgList(ARG_LIST * argList)
{
	ARG_LIST * currentArg = argList;
	printf("ARG_LIST:\n");
	while(currentArg != NULL)
	{	
		printf("%s\n", currentArg->word);	
		currentArg = currentArg->next;
	}
}

void yyerror(char * s)
{
	printf("%s\n", s);
}
