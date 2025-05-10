#ifndef NDDT_H
#define NDDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * nddt.h - NDS+ Developer Terminal Header
 *
 * This header provides a simple implementation for the interactive NDS+ Terminal,
 * which is invoked (for example) via the command "ndsp term". The terminal allows
 * the user to execute commands in an interactive loop (using bash).
 *
 * Functions provided:
 *   - nddt_init()      : Initialize any terminal state (if needed)
 *   - nddt_run()       : Run the interactive terminal loop
 *   - nddt_shutdown()  : Shutdown the terminal cleanly
 *
 * Usage Example (in your ndspterm.c file):
 * 
 *      #include "nddt.h"
 *
 *      int main(void) {
 *          nddt_init();
 *          nddt_run();
 *          nddt_shutdown();
 *          return 0;
 *      }
 */

/*
 * nddt_init
 *
 * Initializes the NDS+ Terminal. In a real implementation, this might configure terminal
 * modes (echo, raw mode, etc.) or set up environment variables. For this simple example,
 * it just prints an initialization message.
 */
static inline void nddt_init(void) {
    printf("NDS+ Terminal initialized.\n");
}

/*
 * nddt_run
 *
 * Runs the main loop of the NDS+ Terminal. This function repeatedly prints a prompt,
 * reads a command from standard input, and then uses system() to execute the command.
 * The loop exits if the user types "exit".
 */
static inline void nddt_run(void) {
    char input_buffer[1024];

    printf("Welcome to NDS+ Terminal (ndspterm.exe)\n");
    printf("Type commands for bash (or 'exit' to quit).\n");

    while (1) {
        printf("ndsterm> ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break; // End if input fails
        }
        /* Remove trailing newline, if any */
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        if (strcmp(input_buffer, "exit") == 0) {
            break;
        }

        /* Execute the command using the default system shell (bash).
         * In a more advanced implementation, you might want to parse the command
         * or handle errors more gracefully.
         */
        if(system(input_buffer) == -1) {
            perror("Error executing command");
        }
    }
}

/*
 * nddt_shutdown
 *
 * Performs any necessary shutdown/cleanup for the terminal.
 * For this simple example, it simply prints a shutdown message.
 */
static inline void nddt_shutdown(void) {
    printf("NDS+ Terminal shutting down.\n");
}

#ifdef __cplusplus
}
#endif

#endif /* NDDT_H */
