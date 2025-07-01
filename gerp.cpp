/*
 *  gerp.h
 *  Alexander Fomin and Saajid Islam
 *  12/9/2022
 *
 *  Project 4 - Gerp
 *
 *  Purpose: Function definitions for Gerp which creates the simulation by 
 *           making a interface that will continuously take in and execute
 *           commands based on input.
 */

#include "gerp.h"

/* parameters:  directory to traverse through, file to output text to
 * purpose:     initializes a Gerp class object; constructor
 * returns:     none
 */
Gerp::Gerp(FSTree dir, string outputfile)
{
    open_or_die(output, outputfile);
    numItems = 0;
    
    // initializes the two hash tables used in Gerp
    hashTable = vector<list<wordID*>>(1 << 20);
    lcHashTable = vector<list<wordID*>>(1 << 18);

    // reads through every file and hashes/stores all the words and lines

    FSTreeTraversal(dir.getRoot(), "");

    // runs the command loop
    commandLoop(cin);
}

/* parameters:  none
 * purpose:     destructor; deletes WordIDs from the heap
 * returns:     none
 */
Gerp::~Gerp()
{
    // iterates through case sensitive hash table
    for (auto x : hashTable) {
        // checks if hash table is empty
        while (not x.empty()) {
            delete x.front();
            x.pop_front();
        }
    }

    // iterates through lowercase hash table 
    for (auto x : lcHashTable) {
        // checks if lowercase hash table is empty
        while (not x.empty()) {
            delete x.front();
            x.pop_front();
        }
    }

    // iterates through file line structs in vector
    for (auto x : allFileLines) {
        delete x;
    }
}

/* parameters:  takes in a queried string to search for
 * purpose:     searches tables for every line the query is in; sensitive
 * returns:     none
 */
void Gerp::query(const string &query)
{
    // initializes size_t num to hash to the appropriate bucket of hash table
    size_t index = getIndex(query, hashTable);
    // creates a bool to track if the queried word was found in the files
    bool found = false;

    // iterates through list of word variations at the hash index
    for (auto x : hashTable[index]) {
        // checks if query matches a particular variation stored at index
        if (x->word == query) {
            found = true;
            // iterates through the vector of line nums if query found
            for (auto y : x->indices) {
                output << allFileLines[y]->filepath << ":" 
                     << allFileLines[y]->lineNumber << ": "
                     << allFileLines[y]->line << endl;
            }
        }
    }

    // checks found bool to see if the word has not been found
    if (not found)
        output << query << " Not Found. Try with @insensitive or @i." << endl;
}

/* parameters:  takes in a queried string to search for
 * purpose:     conducts an insensitive search of a string in the files
 * returns:     none
 */
void Gerp::insensitiveQuery(const string &query)
{
    // initializes size_t num to hash to the appropriate bucket of hash table
    size_t index = getIndex(query, lcHashTable);
    // creates a bool to track if the queried word was found in the files
    bool found = false;

    // iterates through list of word variations at the hash index
    for (auto x : lcHashTable[index]) {
        // checks if query matches a particular variation stored at index
        if (query == x->word) { 
            found = true;
            // iterates through the vector of line nums if query found
            for (auto y : x->indices) {
                output << allFileLines[y]->filepath << ":" 
                     << allFileLines[y]->lineNumber << ": "
                     << allFileLines[y]->line << endl;
            }
        }
    }

    // checks found bool to see if the word has not been found
    if (not found)
        output << query << " Not Found." << endl;
}

/* parameters:  user inputted command
 * purpose:     runs the command loop until told to quit
 * returns:     none
 */
void Gerp::commandLoop(istream &command)
{
    // keeps command loop running while gerp has not been quit
    while (not command.eof() and not end_sim)
        readCommand(command);
    cout << "Goodbye! Thank you and have a nice day." << endl;
    input.close();
    output.close();
}

/* parameters:  user or file inputted command
 * purpose:     read in command and execute it
 * returns:     none
 */
void Gerp::readCommand(istream &command)
{
    cout << "Query? ";
    string word; // initializes a new string for commands passed in
    command >> word;

    // checks if the command indicates a insensitive search
    if (word == "@i" or word == "@insensitive") {
        string word2; // initializes a string to hold queried word
        cin >> word2;
        insensitiveQuery(stripNonAlphaNum(word2));
    } else if (word == "@f") { // checks if output file is to be changed
        string file; // initializes string to hold new output filename
        cin >> file;
        output.close();
        open_or_die(output, file);
    } else if (word == "@q" or word == "@quit") { // checks for quitting
        end_sim = true;
    } else {
        query(stripNonAlphaNum(word));
    }
}

/* parameters:  pointer to a directory, and a string which recursively
 *              stores the filepath
 * purpose:     print the full paths of all the files accessible from root
 *              directory
 * returns:     none
 */
void Gerp::FSTreeTraversal(DirNode* dir, string s)
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
            open_or_die(input, s3);
            readFile(s3);
        }
    }
}

/* parameters:  takes in the filepath
 * purpose:     reads the file to hash the words away and store its lines
 * returns:     none
 */
void Gerp::readFile(const string &filepath)
{
    int lineNum = 1; // initializes the line number to read from

    // keeps getting the line while the end of the file is not reached
    while (not input.eof()) {
        string line; // creates a string to hold file lines
        getline(input, line);
        lineID *id = new lineID; // makes struct on the heap to hold info
        id->filepath = filepath;
        id->lineNumber = lineNum;
        id->line = line;
        lineNum++;

        allFileLines.push_back(id);

        stringstream ss(line); // makes new stringstream from line
        
        // moves through words in a line to hash them away
        while (not ss.eof()) {
            string word; // initializes a string to hold words from the line
            ss >> word;
            word = stripNonAlphaNum(word);
            if (word != "") {
                if ((numItems >> 6) >= hashTable.size()) {
                    expandHash();
                }

                initializeTable(word, hashTable, allFileLines.size() - 1);
                initializeTable(toLower(word), lcHashTable, 
                                allFileLines.size() - 1);
                numItems++;
            }
        }
    }

    input.close();
}

/* parameters:  a string (word), and the hash table to store it in
 * purpose:     appends the current index to the wordID's indices, creating a
 *              new wordID node first if necessary
 * returns:     none
 */
void Gerp::initializeTable(const string &word, vector<list<wordID*>> &table,
                           size_t lineIndex)
{
    size_t index = getIndex(word, table);
    wordID* node = chainSearch(word, index, table);
    
    if (node != nullptr) { 
        // checks if there are duplicates in same line
        if (node->indices.back() != lineIndex)
            node->indices.push_back(lineIndex);
    } else {
        // creates new wordID 
        wordID* key = new wordID;
        key->word = word;
        key->indices.push_back(lineIndex);
        table[index].push_back(key);
    }
}

/* parameters:  a word, its hash index, and the hash table to search in
 * purpose:     checks if the given word already exists in the hash table
 * returns:     a pointer to the word's wordID, nullptr if word is not found
 */
wordID* Gerp::chainSearch(const string &word, size_t index, 
                          const vector<list<wordID*>> &table)
{
    for (auto x : table[index]) {
        if (x->word == word)
            return x;
    }

    return nullptr;
}

/* parameters:  a string
 * purpose:     turns all letters in the string into lowercase versions
 * returns:     the lowercase version of the string
 */
string Gerp::toLower(string line)
{
    // iterates through the chars of the given line
    for (size_t i = 0; i < line.length(); i++)
        line[i] = tolower(line[i]);

    return line;
}

/* parameters:  takes in a string to be modified     
 * purpose:     strips the input string of all leading and trailing
 *              non-alphanumeric characters 
 * returns:     a stripped string without leading or trailing non-alphanumeric
 *              characters
 */
string Gerp::stripNonAlphaNum(const string &word)
{
    // checks if the string is a new line
    if (word.size() < 1)  
        return "";

    int left = 0; 
    int right = word.size() - 1; 

    // "strips" leading non-alphanumeric characters
    while (left < (int)(word.size()) and not iswalnum(word.at(left)))
        left++;

    // "strips" trailing non-alphanumeric characters
    while (right >= 0 and not iswalnum(word.at(right)))
        right--;
    
    // checks that it wasn't all non-alphanumeric characters
    if (left <= right)
        return word.substr(left, right - left + 1);
    
    // should only get here if word was all non-alphanumeric
    return "";
}

/* parameters:  word to convert to index, hash table to base function off of
 * purpose:     converts the word into a hash number (i.e. a index)
 * returns:     index of the word
 */
size_t Gerp::getIndex(const string &word, vector<list<wordID*>> &table)
{
    return hash<string>{}(word) & (table.size() - 1);
}

/* parameters:  none
 * purpose:     expands the hash in case the load factor of the current hash
 *              gets too high
 * returns:     none
 */
void Gerp::expandHash()
{
    // initializes two new vectors to become the new expanded hash tables
    vector<list<wordID*>> newHashTable(hashTable.size() * 2);
    vector<list<wordID*>> newLcHashTable(hashTable.size() * 2);
    
    // iterates through the existing hash table to access all of its line nums
    for (auto x : hashTable) {
        for (auto y : x) {
            for (auto z : y->indices)
                initializeTable(y->word, newHashTable, z);
        }
    }

    // iterates through the existing lc hash table to access all of its lines
    for (auto x : lcHashTable) {
        for (auto y : x) {
            for (auto z : y->indices)
                initializeTable(y->word, newLcHashTable, z);
        }
    }

    hashTable = newHashTable;
    lcHashTable = newLcHashTable;
}

/* parameters:  attempts to open files, aborts if unable to open
 * purpose:     ifstream or ofstream and the associated file
 * returns:     none
 */
template<typename streamtype>
void Gerp::open_or_die(streamtype &stream, const string &filename)
{
    stream.open(filename);
    
    // checks if the filestream did not successfully open
    if (not stream.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
}
