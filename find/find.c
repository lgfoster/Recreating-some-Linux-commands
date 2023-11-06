/**
 * @file find.c
 * @author Luke Foster
 * @brief All function definitions for the find.h header file. Functions are designed to search directories for 
 *        a specific query if needed, or it will gather all files in all directories in and below the current working
 *        directory.
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "find.h"

static const char dot[] = ".";
static const char dot2[] = "..";

#define S_IFMT  0170000 /*    Mask    */
#define S_IFDIR 0040000 /* Directory. */

/**
 * @brief Wrapper for the argument search function. Designed to pre allocate memory for array before search
 * 
 * @param currentDir Current directory to start in
 * @param query String to be searched for
 * @param stringArray Array of found files
 * @param total Total number of found files
 */
void wrapperArg(char currentDir[], char query[], char ***stringArray, int *total)
{
    *stringArray = (char **) malloc(sizeof(char *) * 50);

    searchArg(currentDir, query, stringArray, total);
}

/**
 * @brief This function accepts the user's query and searches directories for file names that match it
 * 
 * @param currentDir Current directory in the search
 * @param query File to be searched for
 * @param foundFiles Filenames that are found
 * @param total Total filenames that are found
 */
void searchArg(char currentDir[], char query[] , char ***foundFiles, int *total)
{
    struct dirent *filePtr; 
    struct stat infoBuff;

    // Open directory
    DIR *dirPtr = opendir(currentDir); 
    
    // Read directory entries
    while((filePtr = readdir(dirPtr)))
    {
        // Skip over curDir . and prevDir ..
        if(strcmp(dot, filePtr->d_name) != 0 && strcmp(dot2, filePtr->d_name) != 0)
        {
            // If query found, store information
            if(strcmp(filePtr->d_name, query) == 0)
            {
                (*foundFiles)[*total] = malloc(strlen(currentDir) + strlen(filePtr->d_name) + 3);

                strcpy((*foundFiles)[*total], currentDir);
                strcat((*foundFiles)[*total], "/");
                strcat((*foundFiles)[*total], filePtr->d_name);

                *(total) += 1;
            }

            // Generate stat struct to determine type
            stat(filePtr->d_name, &infoBuff);

            // Determine if mode is directory
            if(S_ISDIR(infoBuff.st_mode))
            {
                // Generate new file path string
                char *newDirPath;

                newDirPath = malloc(strlen(currentDir) + strlen(filePtr->d_name) + 2);

                strcpy(newDirPath, currentDir);
                strcat(newDirPath, "/");
                strcat(newDirPath, filePtr->d_name);

                // Recursively call
                searchArg(newDirPath, query, foundFiles, total);

                free(newDirPath);
            }
        }
    }
}

/**
 * @brief Wrapper function for no argument that handles pre allocation of filename array
 * 
 * @param currentDir Directory to start search from
 * @param stringArray Array of filenames
 * @param total Total number of files found
 */
void wrapperNoArg(char currentDir[], char ***stringArray, int *total)
{
    *stringArray = (char **) malloc(sizeof(char *) * 50);

    noArg(currentDir, stringArray, total);
}

/**
 * @brief This function handles the case when the user doesn't enter in a query. It will traverse the working directory
 *        and recursively open any directories that are below this directory, storing all files that it has found along the way/
 * 
 * @param currentDir Current directory being scanned
 * @param foundFiles Names of files that were found
 * @param total Total number of files recorded
 */
void noArg(char currentDir[], char ***foundFiles, int *total)
{
    struct dirent *filePtr; 
    struct stat infoBuff;

    // Open directory
    DIR *dirPtr = opendir(currentDir); 
    
    // Read directory entries
    while((filePtr = readdir(dirPtr)))
    {
        // Skip over curDir . and prevDir ..
        if(strcmp(dot, filePtr->d_name) != 0 && strcmp(dot2, filePtr->d_name) != 0)
        {
            (*foundFiles)[*total] = malloc(strlen(currentDir) + strlen(filePtr->d_name) + 3);

            strcpy((*foundFiles)[*total], currentDir);
            strcat((*foundFiles)[*total], "/");
            strcat((*foundFiles)[*total], filePtr->d_name);

            *(total) += 1;

            // Read filename into 
            stat(filePtr->d_name, &infoBuff);


            // Determine if mode is directory
            if(S_ISDIR(infoBuff.st_mode))
            {
                // Generate new file path string
                char *newDirPath;

                newDirPath = malloc(strlen(currentDir) + strlen(filePtr->d_name) + 2);

                strcpy(newDirPath, currentDir);
                strcat(newDirPath, "/");
                strcat(newDirPath, filePtr->d_name);

                // Recursively call
                noArg(newDirPath, foundFiles, total);

                free(newDirPath);
            }
        }
    }
}

/**
 * @brief Comparison function for quicksort 
 * 
 * @param p1 First file to be compared
 * @param p2 Second file to be compared
 * @return int Boolean for lower to uppercase comparison
 */
int compare(const void *p1, const void *p2)
{
    const char *first = *(char **)p1;
    const char *second = *(char **)p2;

    return strcmp(first, second);
}

/**
 * @brief Function that calls quicksort
 * 
 * @param files Files to be sorted alphabetically
 * @param totalFiles Total number of files to be sorted
 */
void sortContents(char **files, const int totalFiles)
{
    qsort(files, totalFiles, sizeof(char *), compare);
}

