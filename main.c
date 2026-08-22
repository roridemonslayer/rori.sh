#include <stdio.h> //for printing hello
#include <unistd.h> //this pulls in fork which is the thing that makes a duplicate of ur shell while a process is happening 
#include <string.h>
#include <sys/wait.h>

int main() {
    //here we;re seeing upo our repl loop which is th e loop used for our shell 
    while(1){ //this loops forever and its a true statemtn 1 = true
        //reading part 
        char input[100]; //this says the char max inout is 100 characters
        printf("◆ "); //wehre users will type 
        fflush(stdout);
        fgets(input, 100, stdin); //this gets the info from the keyboard. stdnin means read from the keyboard

        char *command = strtok(input, " \n") ;// here we're storing the first word look isndie the inout adn give me the fiurst wordd u see b4 a new line 
        char *args = strtok(NULL, " \n") ; //for wtv additional info you're passing in aside form ur maid so liek ls - la

        pid_t pid = fork(); // makes the cline 

        if (pid == 0){
            char *argsv[] = {command, args, NULL};
            execvp(command, argsv); 
        } else{
            wait(NULL);

        }
    }



}