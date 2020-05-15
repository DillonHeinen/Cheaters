#ifndef CHEATERS
#define CHEATERS

#include <vector>
#include <string>

using namespace std;

class cheaters{

private:

    static const int colSize = 250043;	// prime number hash table

    typedef struct ListNode {		// linked list struct
        int data;
        ListNode* next;
    } ListNode;

    ListNode* collisions[colSize];

    int **table;

    int chunkSize;	// stores chunk size
    int numShared;		// threshold
    int fileNum;	    // stores file numbers for
    vector<string> files;	// stores file names
    vector<string> chunks;	// stores chunks
    vector<int> indices;	// stores collisions idices
    string path;	//stores path
    // hash table
    int getDir(string dir, vector<string> &files);	// gets vector of paths
    int hash(string key);	// hash function
    void storeChunk(int hashIndex, int fileNum);	// stores in hash table
    void deallocate();


public:
    cheaters(int chunkSize, int numShared, string &path);
    ~cheaters();
    void printFiles();
    int getChunks();
    void getTable();
    void printResult();


};

#endif