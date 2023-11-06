/**
 * @file main.c
 * @author Luke Foster
 * @brief This program has the user input a file name and then either a new file name or a directory path.
 *        It will then move or rename that file accordingly.
 * @version 0.1
 * @date 2021-11-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check if a valid number of arguments were passed
    if(argc == 1)
    {
        fprintf(stderr, "Too few arguments! Canceling move...\n");
        exit(1);
    }
    else if(argc > 3)
    {
        fprintf(stderr, "Too many arguments! Canceling move...\n");
        exit(1);
    }

    int newLink, oldLink;

    // Generate new link of the file
    newLink = link(argv[1], argv[2]);

    // Determine if an error occurred on the move
    if(newLink == -1)
    {
        perror(argv[2]);
        exit(1);
    }

    // Remove old hard link
    newLink = unlink(argv[1]);

    // Determine if an error occurred on unlink
    if(oldLink == -1)
    {
        perror(argv[1]);
        exit(1);
    }

    return 0;
}