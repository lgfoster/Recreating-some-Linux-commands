/**
 * @file find.h
 * @author Luke Foster
 * @brief Header file that contains all the functions to be declared in its respective c file
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void noArg(char currentDir[], char ***foundFiles, int *total);

void wrapperNoArg(char currentDir[], char ***stringArray, int *total);

void wrapperArg(char currentDir[], char query[],char ***stringArray, int *total);

void searchArg(char currentDir[], char query[] , char ***foundFiles, int *total);

void sortContents(char **files, const int totalFiles);
