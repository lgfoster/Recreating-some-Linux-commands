/**
 * @file ls.c
 * @author Luke Foster
 * @brief Function definitions for the ls.h header. These functions mimic the ls command and will traverse
 *        through the working directory and all subsequent ones below it and display various pieces of information
 *        pertaining to those files.
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ls.h"

/**
 * @brief This function traverses directories by starting in the current working directory and displays various information about
 *        the files in the directory. Furthermore, it will also traverse through all directories that are below the current working
 *        directory and display all their contents as well.
 * 
 * @param currentDir Current directory being scanned
 */
void traverse(char currentDir[])
{
    struct dirent *filePtr; 
    struct stat infoBuff;
    
    int totalDir = 0;
    int blockSize = 0;
    char **dirNames;
    char *pathToFile;

    // Open directory
    DIR *dirPtr = opendir(currentDir); 

    // Print current directory in tree
    printf("\n%s:\n", currentDir);

    // Initialize directory names array of size 5
    dirNames = (char **) malloc(sizeof(char *) * 5);
    
    // Read directory entries
    while((filePtr = readdir(dirPtr)))
    {
        pathToFile = generatePath(currentDir, filePtr->d_name);
        
        // Open stat struct of file and display contents
        if(lstat(pathToFile, &infoBuff) == 0)
        {
            // Display file info and increase blocksize of directory
            displayFile(filePtr, &infoBuff);

            blockSize += infoBuff.st_blksize;

            // Determine if mode is directory and not . or .. directories
            if(S_ISDIR(infoBuff.st_mode) && strcmp(".", filePtr->d_name) != 0 && strcmp("..", filePtr->d_name) != 0)
            {
                // Save pathway to found directory
                dirNames[totalDir] = pathToFile;
                totalDir++;

                // Increase directory array size by 5 if not enough space
                if(totalDir % 5 == 0 && totalDir != 0)
                {
                    dirNames = (char **) realloc(dirNames, totalDir + 5);
                }
            }
            else
            {
                free(pathToFile); // Free file pathway
            }  
        }
    }

    // Display blocksize of the directory
    printf("Total: %d\n", blockSize);

    // Close current working dir
    closedir(dirPtr); 

    // Process found directories, if any
    if(totalDir > 0)
    {
        int index;

        for(index = 0; index < totalDir; index++)
        {
            traverse(dirNames[index]);
           
            free(dirNames[index]); // Release directory name
        }
    }

    free(dirNames); // Release directory names array
}

/**
 * @brief This function handles the creation of file pathway names so that the lstat() and opendir() functions can
 *        properly open directories and structures as the program traverses the directory tree.
 * 
 * @param currentDir Current directory that is open
 * @param fileName Name of file found
 * @return char* Pathway to the file
 */
char * generatePath(const char * currentDir, const char *fileName)
{
    char *pathway;

    // Allocate memory for new pathway
    pathway = (char *) malloc(strlen(currentDir) + strlen(fileName) + 2);

    if(pathway == NULL)
    {
        printf("Error allocating memory for path to file!\n");
        exit(1);
    }
    
    // Copy over file pathway
    strcpy(pathway, currentDir);
    strcat(pathway, "/");
    strcat(pathway, fileName);

    return pathway;
}


/**
 * @brief Display information about the current file. The information displayed is similar to the command ls -ail,
 *        showing all files, including hidden ones, with additional information in regards to permissions, size, ect.
 * 
 * @param currentFile Current file to display
 * @param infoBuff Stat buffer containing current file contents
 */
void displayFile(const struct dirent *currentFile, const struct stat *infoBuff)
{
    struct group *groupInfo;
    struct passwd *userInfo;

    char modes[] = "----------";

    modeToLetters(infoBuff->st_mode, modes);

    // Retrieve actual names of user and group
    userInfo = getpwuid(infoBuff->st_uid);
    groupInfo = getgrgid(infoBuff->st_gid);
    
    // Display all information of current file
    printf("%ld ", infoBuff->st_ino);
    printf("%s  ", modes);
    printf("%ld ", infoBuff->st_nlink);
    printf("%s ", userInfo->pw_name);
    printf("%s ", groupInfo->gr_name);
    printf("%7ld ", infoBuff->st_size);

    // Call time conversion function
    showTime(infoBuff->st_mtime);

    printf("%s", currentFile->d_name);
    
    // Print a ~ if the file is a symbolic link
    if(S_ISLNK(infoBuff->st_mode))
    {
        printf("~\n");
    }
    else
    {
        printf("\n");
    }
}




/**
 * @brief This function translates the mode bits of a file into something readable
 * 
 * @param mode File mode
 * @param str Mode string to be written to
 */
void modeToLetters(int mode, char *str)
{ 
    // Directory
    if(S_ISDIR(mode)) 
        str[0] = 'd';  
    
    // Char device
    if(S_ISCHR(mode)) 
        str[0] = 'c';  
    
    // Block device
    if(S_ISBLK(mode)) 
        str[0] = 'b';  

    if(S_ISLNK(mode))
        str[0] = 'l';

    // Modes for user
    if(mode & 0400) 
        str[1] = 'r';
    if(mode & 0200) 
        str[2] = 'w';
    if(mode & 0100) 
        str[3] = 'x';

    // Modes for group
    if(mode & 0040) 
        str[4] = 'r';
    if(mode & 0020) 
        str[5] = 'w';
    if(mode & 0010) 
        str[6] = 'x';

    // Modes for other
    if(mode & 0004) 
        str[7] = 'r';
    if(mode & 0002) 
        str[8] = 'w';
    if(mode & 0001) 
        str[9] = 'x';
}


/**
 * @brief Displays time since last modification in an appropriate time format
 *        
 * 
 * @param timeval Time value to be translated
 */
void showTime(long timeval)
{
    // C-String to hold full date info
    char formTime [17];

    // Time structure that will hold the number in timeval
    struct tm * timeStruct;

    timeStruct = localtime(&timeval);

    // Access the timeStruct, format it and store in formTime
    strftime (formTime, 17, "%F %R", timeStruct);

    // Print the formated time
    printf("%-5s ", formTime);
}