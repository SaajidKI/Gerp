/*
 *  gerp.h
 *  Alexander Fomin and Saajid Islam
 *  12/9/2022
 *
 *  Project 4 - Gerp
 *
 *  Purpose: Class declaration for Gerp which creates the search functionality
 *           by making a interface that will continuously take in and execute
 *           commands based on input.
 */

#ifndef GERP_H
#define GERP_H

#include "DirNode.h"
#include "FSTree.h"
#include <cwctype>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct lineID {
    string filepath;
    int lineNumber;
    string line;
};

struct wordID {
    string word;
    vector<size_t> indices;
};

class Gerp {
public:
    Gerp(FSTree dir, string outputfile);
    ~Gerp();

private:
    void query(const string &query);
    void insensitiveQuery(const string &query);
    void commandLoop(istream &command);
    void readCommand(istream &command);
    
    void FSTreeTraversal(DirNode* dir, string s);
    void readFile(const string &filepath);
    void initializeTable(const string &word, vector<list<wordID*>> &table, 
                         size_t lineIndex);
    wordID* chainSearch(const string &word, size_t index, 
                        const vector<list<wordID*>> &table);
    string toLower(string line);
    string stripNonAlphaNum(const string &word);
    size_t getIndex(const string &word, vector<list<wordID*>> &table);
    void expandHash();

    vector<lineID*> allFileLines;
    vector<list<wordID*>> hashTable;
    vector<list<wordID*>> lcHashTable;

    ofstream output;
    ifstream input;
    bool end_sim = false;
    size_t numItems;
    
    template<typename streamtype>
    void open_or_die(streamtype &stream, const string &filename);
};

#endif
