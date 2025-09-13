#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>


char *home_dir;

static int cd(int argc, char **argv){
    const char *target = NULL;

    if(argc < 2 || strcmp(argv[1], "~") == 0){
        // home directory
        target = getenv("HOME"); 
    }
    else{
        // .. and . and all the stuff are consumed here.
        target = argv[1];
    }

    if(chdir(target) != 0){ 
        return 1; // actual CD + error processing
    }

    return 0;
}

int main(void){
    char buf[1024];
    char *argv[10];
    int argc; 
    
    while(1){
        printf("> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) break;

        // tokenize
        buf[strcspn(buf, "\n")] = '\0';

        argc = 0;
        char *token = strtok(buf, " ");
        while(token != NULL && argc < 10){
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        if (argc == 0) continue; // skip if meaningless

        // This is required for the last one.
        argv[argc] = NULL;

        // exit
        if (strcmp(argv[0], "exit") == 0){
            printf("exiting...\n");
            return 0;
        }
        
        if(strcmp(argv[0], "cd") == 0){
            if (cd(argc, argv)){
                printf("cd: invalid path");
            }
        }
        else { // execvp implementation
            pid_t pid;

            pid = fork();  
            if (pid == 0) { // child
                if (execvp(argv[0], argv) == -1){
                    printf("Parse Error\n");
                }
            }
            else { // parent
                int status;
                waitpid(pid, &status, 0);  
            }
            
        }
    }
}