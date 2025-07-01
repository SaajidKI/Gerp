/*
 *  main.cpp
 *  Alexander Fomin and Saajid Islam
 *  12/4/2022
 *
 *  Project 4 - Gerp
 *
 *  Purpose: Gerp is a program that behaves similarly to the unix grep program,
 *           and can search through all the files in a provided directory and
 *           look for a sequence of characters.
 */

#include "gerp.h"

int main(int argc, char *argv[])
{
    if (argc == 3) {
        FSTree directory(argv[1]);
        Gerp gerp(directory, argv[2]);
    } else {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
