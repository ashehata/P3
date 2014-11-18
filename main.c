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




























//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <errno.h>
//
//#ifdef _WIN32
//#include <windows.h>
//#define chdir _chdir
//
//#else
//#include <unistd.h>
//#endif
//
//#define MAX_LENGTH 1024
//#define DELIMS " \t\r\n"
//pid_t pid;
//int child_status, retval;
//int countArguments(char *str){
//    const char *sep = " \t\n\r\v\f";
//    int count = 0;
//    size_t len;
//    
//    str += strspn(str, sep);
//    
//    while ((len = strcspn(str, sep)) > 0) {
//        ++count;
//        str += len;
//        str += strspn(str, sep);
//    }
//    return count;
//}
//
//void parse(char* str, int numArgs,char* args[]){
//    char* arg;
//    for (int i=0; i<numArgs; i++){
//        arg = strtok(str, DELIMS);
//        args[i] = arg;
//    }
//    
//}
//
//int main(int argc, char *argv[]) {
//    char *prompt = "svsh > ";
//    char *cmd;
//    char line[MAX_LENGTH];
//    
//    while (1) {
//        printf("%s", prompt);
//        if (!fgets(line, MAX_LENGTH, stdin)) break;
//        
//        // Parse and execute command
//        if ((cmd = strtok(line, DELIMS))) {
//            // Clear errors
//            errno = 0;
//            ////////////////
//            
//            //# command
//            if (strcmp(cmd, "#") == 0){
//                char *arg = strtok(0, "\n");
//                printf("%s\n", arg);
//            }
//            
//            
//            
//            //defpromt
//            else if (strcmp(cmd, "defprompt") == 0){
//                char *arg = strtok(0, "\n");
//                prompt = arg;
//            }
//            else if (strcmp(cmd, "cd") == 0) {
//                char *arg = strtok(0, DELIMS);
//                if (!arg) fprintf(stderr, "Missing argument\n");
//                else chdir(arg);
//            }
//            else if (strcmp(cmd, "bye") == 0) {
//                break;
//            }
//            
//            //User commands:
//            
//            //run
//            else if (strcmp(cmd, "run")==0){
//                {
//                    char* path = "/bin/";
//                    char *arguments = strtok(0, "<");
//                    //if pid = 0 then we're at the child
//                    //Count the number of arguments
//                    int num_of_args = countArguments(arguments);
//                    //create an array of pointers for the arguments to be passed to execcv.
//                    char *tokenizedArguments[num_of_args+1];
//                    //parse the input and arguments will have all the arguments to be passed to the program
//                    parse(arguments, num_of_args, tokenizedArguments);
//                    
//                    //set the last pointer in the array to NULL. Requirement of execv
//                    tokenizedArguments[num_of_args] = NULL;
//                    //This will be the final path to the program that we will pass to execv
//                    char prog[512];
//                    //First we copy a /bin/ to prog
//                    strcpy(prog, path);
//                    //Then we concancate the program name to /bin/
//                    //If the program name is ls, then it'll be /bin/ls
//                    strcat(prog, tokenizedArguments[0]);
//                    //pass the prepared arguments to execv and we're done!
//                    char* test[2] = {"/bin/ls", NULL};
//                    int pid = fork();
//                    //Error checking to see if fork works
//                    //If pid !=0 then it's the parent
//                    pid = fork();
//                    if (pid ==0) {
//                        printf("The child is running. The child is about to run %s\n",
//                               cmd);
//                        int result =execv("/bin/ls", test);
//                        if (result == -1)
//                            printf("Error calling exec!");
//                        printf("If you see this, then the child failed to exec the program\n");
//                        exit(0);
//                    }
//                    else {
//                        
//                        printf("The parent started a child to run %s.\n", cmd);
//                        retval = wait(&child_status); /* block until child terminates */
//                        
//                    }
//                }
//            }
//            
//            
//            // assignto
//            else if (strcmp(cmd, "assignto")){
//                
//                
//            }
//            
//            else system(line);
//            
//            if (errno) perror("Command failed");
//        }
//    }
//    
//    return 0;
//}
