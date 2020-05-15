#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "cheaters.h"

using namespace std;

cheaters::cheaters(int chunkSpec, int numShared, string &path) {
    this->chunkSize = chunkSpec;
    this->numShared = numShared;
    this->path = path;
    for(int i = 0; i < colSize; i++) {
        collisions[i] = NULL;
    }

    getDir(path, this->files);
}

int cheaters::getDir(string dir, vector<string> &files) {	// linux code for retrieving directories
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    files.erase(files.begin(), files.begin() + 2);
//	for(vector<string>::iterator iter = files.begin(); iter < files.end(); iter++) {
//		cout << *iter << endl;
//	}
    return 0;
}

int cheaters::getChunks() {
    ifstream inFile;
    string newPath;
    int fileNum = 0;

    for(vector<string>::iterator iter = files.begin(); iter < files.end(); iter++) {     // iterates through files vector
        string s, chunkString;
        int hashIndex;
        newPath = path + *iter;
        inFile.open(newPath.c_str());
        for(int i = 0; i < chunkSize; i++) {	// fill vector with chunks
            inFile >> s;
            chunks.push_back(s);
        }
        for(vector<string>::iterator iter = chunks.begin(); iter < chunks.end(); iter++) {	// concatinates chunks together
            chunkString = chunkString + *iter;
//            cout << *iter << endl;
        }
//        cout << chunkString << endl;
        hashIndex = hash(chunkString);
        storeChunk(hashIndex, fileNum);
        chunkString = "";
        while(inFile >> s) {		// fills chunks vector
            chunks.push_back(s);
            chunks.erase(chunks.begin());
            for(vector<string>::iterator iter = chunks.begin(); iter < chunks.end(); iter++) {
                chunkString = chunkString + *iter;
            }
            hashIndex = hash(chunkString);
            storeChunk(hashIndex, fileNum);
            chunkString = "";
        }
        chunks.clear();		// close and reset file pointer
        inFile.close();
        inFile.clear();
        fileNum++;
    }
    this->fileNum = fileNum;
    for(vector<string>::iterator iter = chunks.begin(); iter < chunks.end(); iter++) {
		cout << *iter << endl;
		cout << "made it" << endl;
    }

    return 0;
}

int cheaters::hash(string key) {		// hash function
    int hashVal = 0;
    for(int i = 0; i < key.length();  i++) {
        hashVal = 37 * hashVal + key[i];

    }

    hashVal = hashVal % colSize;

    if(hashVal < 0)
        hashVal = hashVal % colSize;

    hashVal = abs(hashVal);	// removes any negative results
    return hashVal;
}

void cheaters::storeChunk(int hashIndex, int fileNum) {
    if(collisions[hashIndex] == NULL) {		// stores item in new collision index
        indices.push_back(hashIndex);
        collisions[hashIndex] = new ListNode;
        collisions[hashIndex]->data = fileNum;
        collisions[hashIndex]->next = NULL;
    }
    else {		// stores item in chain
        ListNode *current = collisions[hashIndex]->next;
        ListNode *previous = collisions[hashIndex];
        if(previous->data == fileNum) {
            return;
        }
        while (current != NULL) {
            current = current->next;
            previous = previous->next;
            if(previous->data == fileNum) {
                return;
            }
        }
        current = new ListNode;
        current->data = fileNum;
        current->next = NULL;
        previous->next = current;
    }
}

void cheaters::getTable() {
    table = new int *[fileNum];		// allocates space for 2-d array
    for(int i = 0; i < fileNum; i++) {
        table[i] = new int [fileNum];
    }
    for(int i = 0; i < fileNum; i++) {
        for(int j = 0; j < fileNum; j++) {
            table[i][j] = 0;
        }
    }
    for(vector<int>::iterator iter = indices.begin(); iter < indices.end(); iter++) {	   // searches through collisions to fill 2-d array
        if((collisions[*iter] != NULL) && (collisions[*iter]->next != NULL)) {
            ListNode *ptr1 = collisions[*iter];
            ListNode *ptr2 = collisions[*iter]->next;
            while(ptr1 != NULL) {
                while(ptr2 != NULL) {
                    if(ptr1->data != ptr2->data) {
                        table[ptr2->data][ptr1->data] += 1;
                    }
                    ptr2 = ptr2->next;
                }
                ptr1 = ptr1->next;
            }
        }
    }    
}

void cheaters::printResult() {		// outputs final result of comparisons
    int maxRow = 0;
    int maxCol = 0;
    int maxValue = numShared + 1;
    int i = 0, j = 0, k = 0, m = 0;
      while(maxValue > numShared) {
        maxValue = 0;
        for (i = 0; i < fileNum; i++) {
            for (j = 0; j < fileNum; j++) {
                if (table[i][j] > maxValue) {
                    maxRow = i;
                    maxCol = j;
                    maxValue = table[i][j];
                }
            }
        }
        if(maxValue > numShared) {	// threshold for similarities
            cout << maxValue << ": " << files.at(maxRow) << ", " << files.at(maxCol) << " " << endl;
        }
        table[maxRow][maxCol] = 0;
        m++;
        k++;
     }
};

void cheaters::deallocate() {		// deallocates hash table and 2-d array
    int i = 0;
    for(i = 0; i < fileNum; i++) {
        delete table[i];
    }
    delete table;

    for(int i = 0; i < colSize; i++) {
        ListNode* previous;
        ListNode* current;
        while(collisions[i] != NULL) {
            previous = collisions[i];
            current = previous->next;
            while (current != NULL) {
                previous = previous->next;
                current = previous->next;
            }
            delete previous;
            collisions[i] = NULL;
        }
    }
}

cheaters::~cheaters() {
    deallocate();
}