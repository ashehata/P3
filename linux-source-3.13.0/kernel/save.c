#nclude <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <ctype.h>
     
#define MAX_BUF_SIZE 1000
/* Our linked list */
struct var{
	char def;
	struct node *next;
};
/* Initialize the root node*/
struct node *root;
root = (struct node *)malloc(sizeof(struct node));
root->next=0;
root->def=0; 

asmlinkage int SaveVariable(char *varname, char *vardef) {
	printk(KERN_EMERG "Entering SaveVariable()");
	/* Error if the varname is not a valid string  */
	if (isdigit(*varname)){
		printk(KERN_EMERG "SaveVariable() failed: varname error");
		return(-1); //return -1 for error
	}
	
	root->next = *varname;	//assign the node's name to varname
	root->def = *vardef;	//assign the struct's def to vardef 
			
    	return(0); //return 0 for successful run
}

asmlinkage int GetVariable(char *varname, char *vardef, int deflen){
/*	int index = root;
	printk(KERN_EMERG "Entering GetVariable()");	
	
	while(index != NULL){
		if (){
			
		}
		index = index->next;
	}
*/
	return(0);
}

asmlinkage int NextVariable(char *prevname, char *varname, int namelen, char *vardef, int deflen){
	printk(KERN_EMERG "Entering NextVariable()");
	
	return(0);
}
