#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <kbdEvents.h>

static char *keys[KEY_MAX];
void keys_init(){
    keys[KEY_1] = "1";
    keys[KEY_2] = "2";
    keys[KEY_3] = "3";
    keys[KEY_4] = "4";
    keys[KEY_5] = "5";
    keys[KEY_6] = "6";
    keys[KEY_7] = "7";
    keys[KEY_8] = "8";
    keys[KEY_9] = "9";
    keys[KEY_0] = "0";
    keys[KEY_MINUS] = "-";
    keys[KEY_EQUAL] = "=";
    keys[KEY_BACKSPACE] = malloc(1);
    *keys[KEY_BACKSPACE] = (char)8;
    keys[KEY_TAB] = "\t";
    keys[KEY_Q] = "q";
    keys[KEY_W] = "w";
    keys[KEY_E] = "e";
    keys[KEY_R] = "r";
    keys[KEY_T] = "t";
    keys[KEY_Y] = "y";
    keys[KEY_U] = "u";
    keys[KEY_I] = "i";
    keys[KEY_O] = "o";
    keys[KEY_P] = "p";
    keys[KEY_LEFTBRACE] = "[";
    keys[KEY_RIGHTBRACE] = "]";
    keys[KEY_ENTER] = "\n";
    keys[KEY_A] = "a";
    keys[KEY_S] = "s";
    keys[KEY_D] = "d";
    keys[KEY_F] = "f";
    keys[KEY_G] = "g";
    keys[KEY_H] = "h";
    keys[KEY_J] = "j";
    keys[KEY_K] = "k";
    keys[KEY_L] = "l";
    keys[KEY_SEMICOLON] = ";";
    keys[KEY_APOSTROPHE] = "'";
    keys[KEY_GRAVE] = "`";
    keys[KEY_BACKSLASH] = "\\";
    keys[KEY_Z] = "z";
    keys[KEY_X] = "x";
    keys[KEY_C] = "c";
    keys[KEY_V] = "v";
    keys[KEY_B] = "b";
    keys[KEY_N] = "n";
    keys[KEY_M] = "m";
    keys[KEY_COMMA] = ",";
    keys[KEY_DOT] = ".";
    keys[KEY_SLASH] = "/";
    keys[KEY_SPACE] = " ";
}




static char input_devices[MAX_INPUT_DEVICES][MAX_FILE_PATH];       //holds keyboard devices on this system
static int dev_n = 0;      //represents number of keyboard devices found

/**
 * will continuously read events from this device until user closes program.
 */
void listen_to_device(const char *device){
    int dev_fd = open(device, O_RDONLY);
    if(dev_fd == -1){
        fprintf(stderr, "error opening device file: %s | %s\n", device, strerror(errno));
        exit(1);
    }
    struct input_event event;
    ssize_t n = 0;
    while(1){
        n = read(dev_fd, &event, sizeof(event));
        if (n == -1){
            fprintf(stderr, "error while reading from device: %s | %s\n", device, strerror(errno));
            exit(1);
        }else if(n != sizeof(event)){ //EOF
            continue;
        }else{
            if(event.type == EV_KEY && event.value > 0){
                if(keys[event.code]){
                    fprintf(stdout, "%s", keys[event.code]);
                    fflush(stdout);
                }
            }
        }
    }
    
}

void read_keyboards(){
    for (int i = 0; i < dev_n; i++){
        pid_t pid = fork();
        if(pid == -1){
            fprintf(stderr, "error forking: %s", strerror(errno));
        }
        else if(pid == 0){     //dedicate this process to listening to specific device
            printf("child created: process %d: parent process %d: process group %d\n", getpid(), getppid(), getpgid(getpid()));
            listen_to_device(input_devices[i]);    
        }
    }
}


/**
 * finds all the keyboard devices on this machine and stores their device
 * files in input_devices
 */
void find_keyboards(){

   FILE *dev_pipe = popen("find \"/dev/input/by-id\" -iname \"*-kbd\"", "r");
   if(dev_pipe == NULL){
       fprintf(stderr, "failed to read input devices %s", strerror(errno));
       exit(1);
   }
   while(dev_n < MAX_INPUT_DEVICES
   && fgets(input_devices[dev_n], sizeof input_devices[dev_n], dev_pipe)){
       //replace terminating char with \0 instead of \n
       int last_char = strcspn(input_devices[dev_n], "\n");
       input_devices[dev_n][last_char] = '\0';
       dev_n++;
   }   
}

