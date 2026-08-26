#include <stdio.h> //for printing hello
#include <unistd.h> //this pulls in fork which is the thing that makes a duplicate of ur shell while a process is happening 
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    //here we;re seeing upo our repl loop which is th e loop used for our shell 
    while(1){ //this loops forever and its a true statemtn 1 = true
        //reading part 
        char input[100]; //this says the char max inout is 100 characters
        printf("◆ "); //wehre users will type 
        fflush(stdout);
        fgets(input, 100, stdin); //this gets the info from the keyboard. stdnin means read from the keyboard

        char *first_cmd = strtok(input, "|");
        char *second_cmd = strtok(NULL, "|"); //this allows the prep 

        char *command = strtok(first_cmd, " \n") ;// here we're storing the first word look isndie the inout adn give me the fiurst wordd u see b4 a new line 
        char *argsv[10] ; //for wtv additional info you're passing in aside form ur maid so liek ls - la
        int i = 0;
        char *redirect = NULL;
        char *gt = strchr(first_cmd, '>');
        if(gt != NULL){
            *gt = '\0';
            
        }

        while(command != NULL){ 
            argsv[i] = command; 
            i ++ ;
            command = strtok(NULL, " \n" );
        }
        argsv[i] = NULL; //this is the end of the argsv array
        char *redirect_file = strtok(NULL, " >");
        if (strcmp(argsv[0], "cd") == 0){ 
            chdir(argsv[1]); //this is the command to change directory
            continue; //this is the command to continue the loop and not run the rest of the code below it

        }
        if( strcmp(argsv[0], "exit") == 0){
            exit(0);
        }

        if (second_cmd != NULL){
            // PIPED CASE: two commands, needs the pipe, two forks, close both ends in parent, wait twice
            int fd[2];
            pipe(fd);

            pid_t pid = fork();
            if(pid == 0){
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
                execvp(argsv[0], argsv);
            }

            pid_t pid2 = fork();
            if(pid2 == 0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                close(fd[1]);
                char *argsv2[10];
                int j = 0;
                char *tok = strtok(second_cmd, " \n");
                while(tok != NULL){
                    argsv2[j] = tok;
                    j++;
                    tok = strtok(NULL, " \n");
                }
                argsv2[j] = NULL;
                execvp(argsv2[0], argsv2);
            }

            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);

        } else {
            // NON-PIPED CASE: just one command, no pipe at all, single fork, single wait
            pid_t pid = fork();
            if (pid == 0){
                execvp(argsv[0], argsv);
            } else {
                wait(NULL);
            }
        }

    }
}