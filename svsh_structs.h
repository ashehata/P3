#ifndef _SVSH_STRUCTS_H_
#define _SVSH_STRUCTS_H_

#define INPUT_LIMIT 40000

typedef struct token_list{
	struct token_list * next;
	char tokenType[INPUT_LIMIT];
	char token[INPUT_LIMIT];
	char usage[INPUT_LIMIT];
} TOKEN_LIST;

typedef struct arg_list {
	struct arg_list * next;
	char word[INPUT_LIMIT];
	char variable[INPUT_LIMIT];
} ARG_LIST;

typedef struct environ_list {
	struct environ_list * next;
	struct environ_list * prev;
	char varName[INPUT_LIMIT];
	char varValue[INPUT_LIMIT];
} ENVIRON_LIST;

#endif /* _SVSH_STRUCTS_H_DEFINED_  */
