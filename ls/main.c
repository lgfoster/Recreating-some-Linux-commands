/**
 * @file main.c
 * @author Luke Foster
 * @brief This progam will call the traverse function, which will display all files in the working
 *        directory and the files of any subsequent directories that are below.
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ls.h"

int main(void)
{   
    // Call the traverse function by starting in current dir
    traverse(".");

    return 0;
}