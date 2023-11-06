/**
 * @file ls.h
 * @author Luke Foster
 * @brief Function declarations and includes needed for the ls program
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

void traverse(char currentDir[]);

char * generatePath(const char * currentDir, const char *fileName);

void displayFile(const struct dirent *currentFile, const struct stat *infoBuff);

void modeToLetters(int mode, char *str);

void showTime(long timeval);

