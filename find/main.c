/**
 * @file main.c
 * @author Luke Foster
 * @brief This program has the user either input a search query or none, and it will then
 *        display all of the files found with that query if possible. If not, it will display
 *        all content in working directory and all directories contained within.
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include "find.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char **allFiles;
    int totalFiles = 0;
    int *ptr = &totalFiles;
    
    // Determine if user input a query
    if(argc == 1)
    {
        wrapperNoArg(".", &allFiles, ptr);
    }
    else
    {
        printf("Searching for: %s\n", argv[1]);
        wrapperArg(".", argv[1], &allFiles, ptr);
    }

    // Sort the results found
    if(totalFiles == 0)
    {
        printf("No matches found.\n");
    }
    else if(totalFiles == 1) // Skip sort if only one result
    {
        puts(allFiles[index]);
        free(allFiles[index]);
    }
    else // Sort results and then display them
    {
        sortContents(allFiles, totalFiles);

        int index;

        for(index = 0; index < totalFiles; index++)
        {
            puts(allFiles[index]);
            free(allFiles[index]);
        }
    }

    free(allFiles); // Free last pointer

    return 0;
}