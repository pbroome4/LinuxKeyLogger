#ifndef KBDEVENTS_HEADER
#define KBDEVENTS_HEADER

    /**
     * Maps linux input.h keys to character data
     */
    void keys_init();

    /**
     * finds all the keyboard devices on this machine and stores their device files
     */
    #define MAX_INPUT_DEVICES 20
    #define MAX_FILE_PATH 255
    void find_keyboards();

    /**
     * forks the main process so each so each child is assigned to listen to a keyboard file
     */
    void read_keyboards();

    /**
     * will continuosly read from this device file.
     */
    void listen_to_device(const char *device);
#endif