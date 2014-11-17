#include <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/string.h>

#define ROWS 20
#define COLUMNS 2
#define MAX_BUF_SIZE 128

char* Variables[ROWS][COLUMNS];

//SaveVariable
asmlinkage int SaveVariable(char __user *varname, char __user *vardef) {

        char* name;			//Buffer for the variable's name
        char* def;			//Buffer for the variable's definition
        int i = 0;			//initialized i for the for loop
	
	//name = *varname;
	//def = *vardef;
	
//Error handling & copying from user space to kernel
        if(copy_from_user(name, varname, MAX_BUF_SIZE)){	//Copy varname from the user space
                printk(KERN_EMERG "SaveVariable failed: copy_from_user() error");
                return (-1);
        }
        if(copy_from_user(def, vardef, MAX_BUF_SIZE)){		//Copy vardef from the user space
                printk(KERN_EMERG "SaveVariable failed: copy_from_user() error");
                return (-1);
        }

//Saving the variable
        for(i = 0; i < ROWS; i++){
                if (i == (ROWS-1)){							//If out of memory 
						printk(KERN_EMERG "SaveVariable failed: Out of memory!\n");
                        return (-1);
                }
                else if (Variables[i][0] == name){      	//If the variable does already exist
                        printk(KERN_EMERG "Variable %s already exists, %s is now assigned to %s!\n", name, name, def);
                        Variables[i][1] = def;
                        return (0);
                }
                else{										//If the variable doesn't already exists
                        printk(KERN_EMERG "Added to memory, variable %s = %s added!\n", name, def);
                        Variables[i][0] = name;
                        Variables[i][1] = def;
                        return (0);
				}
        }
}

asmlinkage int GetVariable(char __user *varname, char __user *vardef, int deflen) {

        char* name; 	//Buffer for the variable's name
        int i = 0;		//initialized i for the for loop

//Error handling
        if(copy_from_user(name, varname, MAX_BUF_SIZE)){
                printk(KERN_EMERG "GetVariable failed: copy_from_user() error");
                return (-1);
        }

//Finding the variable
        for(i = 0; i < ROWS; i++){
                if(Variables[i][0] == name){           //Successfully found the variable
                        copy_to_user(Variables[i][1],vardef,MAX_BUF_SIZE);
                        printk(KERN_EMERG "Successfully retrieved the definition of %s = %s!\n", Variables[i][0], Variables[i][1]);
                        return (0);
                }
                else if (i == (ROWS-1)){               //If not found, return error
                        printk(KERN_EMERG "GetVariable failed: Variable %s not found!\n", name);
                        return (-1);
                }
        }
}

//NextVariable
asmlinkage int NextVariable(char *prevname, char *varname, int namelen, char *vardef, int deflen) {

        char* prev; //Buffer for the previous variable in the array
        int i = 0;

//Error handling
        if(copy_from_user(prev, prevname, MAX_BUF_SIZE)){       //Copy the argument from the user
                printk(KERN_EMERG "NextVariable failed: copy_from_user error!");
        }

//Finding next variable based on previous variable
        for(i = 0; i < ROWS; i++){
                if(prev == ""){        //starting at root or original value in array
                        copy_to_user(varname, Variables[0][0], namelen);
                        copy_to_user(vardef, Variables[0][1], deflen);
                        printf("%s = %s\n", Variables[0][0], Variables[0][1]); //prints the first variable
                        return (0);
                }
                else if(prev == Variables[i][0]){       //not at the beginning of the array of variables
                        if(Variables[i+1][0] != NULL){
                                copy_to_user(varname, Variables[i+1][0], namelen);
                                copy_to_user(vardef, Variables[i+1][1], deflen);
                                printf("%s = %s\n", Variables[i+1][0], Variables[i+1][1]); //prints the next variables
                                return (0);
                        }
                        else if (Variables[i+1][0] == NULL){    //When finished with the array of variables
                                printf("NextVariable failed: End of array!\n");
                                return (-1);
                        }
                }
                else if(i == (ROWS-1)){          //if the variable was not found
                        printf("NextVariable failed: The previous variable was not found!\n");
                        return (-1);
                }
        }
}
