#include <stdio.h> //for printing hello
#include <unistd.h> //this pulls in fork which is the thing that makes a duplicate of ur shell while a process is happening 
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

int main() {
    signal(SIGINT, SIG_IGN);
    while(1){
        char input[100];
        printf("◆ ");
        fflush(stdout);
        fgets(input, 100, stdin);

        char *first_cmd = strtok(input, "|");
        char *second_cmd = strtok(NULL, "|");

        char *argsv[10];
        int i = 0;

        char *gt = strchr(first_cmd, '>');
        if (gt != NULL){
            *gt = '\0';
        }

        char *command = strtok(first_cmd, " \n");
        while(command != NULL){
            argsv[i] = command;
            i++;
            command = strtok(NULL, " \n");
        }
        argsv[i] = NULL;
        if(argsv[0] == NULL){
            continue;
        }

        char *redirect_file = NULL;
        if (gt != NULL){
            redirect_file = gt + 1;
            redirect_file = strtok(redirect_file, " \n");
        }

        if (strcmp(argsv[0], "cd") == 0){
            chdir(argsv[1]);
            continue;
        }
        if(strcmp(argsv[0], "exit") == 0){
            exit(0);
        }

        if (second_cmd != NULL){
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
            pid_t pid = fork();
            if (pid == 0){
                if (redirect_file != NULL){
                    int fd_out = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                }
                execvp(argsv[0], argsv);
            } else {
                wait(NULL);
            }
        }
    }
}