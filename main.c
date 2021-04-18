#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <kbdEvents.h>




/**
 * this program find each keyboard device connected to this machine. Then, for each
 * device it will create a log file that will record every input event.
 * 
 * future plan:
 * create a client/server and have clients send input_event to server
 */


void sig_interrupt_handler(int sig){
    fprintf(stdout, "process %d terminated\n", getpid());
    kill(-getpid(), SIGINT); //negative make it interrupt process group
    exit(0);
}


int main(int argc, char **argv){
    //set up termination. when this process ends, close all child processes
    struct sigaction action_interrupt;
    action_interrupt.sa_handler = sig_interrupt_handler;
    sigfillset(&action_interrupt.sa_mask);
    action_interrupt.sa_flags=0;
    sigaction(SIGINT, &action_interrupt, NULL);
   

    keys_init();
    find_keyboards();
    read_keyboards();

    //keep  this process alive until interrupt signal
    int count = 0;
    while(1){
        //printf("%d\n", count);
        sleep(5);
        count++;
    } 
    
}