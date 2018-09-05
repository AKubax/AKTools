#include <stdio.h>
#include <cstdlib>
#include <utility>

std::pair<const char*, size_t> loadFileToBuf(FILE* file, char* buf = 0, unsigned int size = 0);


std::pair<const char*, size_t> loadFileToBuf(FILE* file, char* buf, unsigned int size){
    unsigned int charToRead = size? size : filelength(fileno(file));
    char* bufToWrite = buf? buf : ((char*) calloc(charToRead, 1));

    _read(fileno(file), bufToWrite, charToRead);
    return std::pair<const char*, size_t>(bufToWrite, charToRead);
}

