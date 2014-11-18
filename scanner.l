%{
#define YY_NO_INPUT
#include <stdlib.h>
#include <string.h>
#include "svsh_structs.h"
#include "y.tab.h"
void yyerror(char * s);
%}



alpha 		[A-Za-z]

alphaAug	(_|/|-|.|{alpha})

digit		[0-9]

%%

[ \t]+		; /* ignore blank spaces*/
"\n"		return NEWLINE; 


"#".*	  /*Ignore comments */
"defprompt"	{ return DEFPROMPT; }
"cd"		{ return CD; }
"="		{ return EQUALTO; }
"assignto"	{ return ASSIGNTO; }
"listjobs"	{ return LISTJOBS; }
"bye"		{ return BYE; }
"run"		{ return RUN; }


[$][a-zA-Z0-9]+	{
				yylval.variable = strdup(yytext);
				return VARIABLE;
			}

[@][a-zA-Z0-9]+	{
				yylval.variable = strdup(yytext);
				return SYS_VARIABLE;
			}

\"(\\.|[^\\"])*\" 	 {	
				yylval.str = strdup(yytext);
				/* Strip the quotes from the string literal */
				/* Start the string beyond the first quote */
				yylval.str++;
				/* Replace the ending quote with null terminator */
				yylval.str[strlen(yylval.str)-1] = '\0';
				return STRING; 
		  	 }

[^% \t\n\"]+		{
				yylval.word = strdup(yytext);
				return WORD;
			}

[#|=] 			{
				yylval.metachar = strdup(yytext);
				return METACHAR;
			}


%%
