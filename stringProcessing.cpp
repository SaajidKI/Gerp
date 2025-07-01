/*
 *  stringProcessing.cpp
 *  Saajid Islam and Alexander Fomin
 *  12/4/2022
 *
 *  Project 4 - Gerp
 *
 *  Purpose: File housing implementation of the stripNonAlphaNum function.
 */

#include "stringProcessing.h"

/* parameters:  takes in a string to be modified     
 * purpose:     strips the input string of all leading and trailing
 *              non-alphanumeric characters 
 * returns:     a stripped string without leading or trailing non-alphanumeric
 *              characters
 */
string stripNonAlphaNum(string input)
{    
    // stripping leading non-alphanumeric characters
    while (not iswalnum(input[0])) // i.e. while not alphanumeric
        input.erase(0, 1);

    // stripping trailing non-alphanumeric characters
    int i = input.length() - 1;
    while (not iswalnum(input[i])) {
        input.erase(i);
        i--;
    }
    
    return input;
}
