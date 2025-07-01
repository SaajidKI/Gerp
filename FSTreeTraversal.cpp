/*
 *  FSTreeTraversal.cpp
 *  Saajid Islam and Alexander Fomin
 *  12/4/2022
 *
 *  Project 4 - Gerp
 *
 *  Purpose: File housing implementation of the FSTreeTraversal function.
 */

#include "FSTree.h"
#include "DirNode.h"
#include <iostream>

using namespace std;

void FSTreeTraversal(DirNode* dir, string s);

int main(int argc, char* argv[])
{
    FSTree mainDir = FSTree(argv[1]);

    if (argc == 2) {
        FSTreeTraversal(mainDir.getRoot(), "");
    } else {
        cerr << "Usage: ./treeTraversal Directory" << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}

/* parameters:  pointer to a directory, and a string which recursively
 *              stores the filepath
 * purpose:     print the full paths of all the files accessible from root
 *              directory
 * returns:     none
 */
void FSTreeTraversal(DirNode* dir, string s)
{
    s += dir->getName();

    // checks if the current directory has subdirectories
    if (dir->hasSubDir()) {
        string s2 = s + "/";
        // traverses the subdirectories contained in DirNodes
        for (int i = 0; i < dir->numSubDirs(); i++) {
            FSTreeTraversal(dir->getSubDir(i), s2);
        }
    }

    // checks if the current directory has files
    if (dir->hasFiles()) {
        // traverses the files contained in the directory
        for (int i = 0; i < dir->numFiles(); i++) {
            string s3 = s + "/" + dir->getFile(i);
            cout << s3 << endl;
        }
    }
}
