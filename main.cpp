#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "cheaters.h"

using namespace std;

int main(int argc, char* argv[]) {

    string path = argv[1];
    int chunk = atoi(argv[2]);
    int numShared = atoi(argv[3]);
    cheaters a(chunk, numShared, path);
    a.getChunks();
    a.getTable();
    a.printResult();
    return 0;
}