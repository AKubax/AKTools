#ifndef AKIO_FILE
#define AKIO_FILE

#include <io.h>
#include <stdio.h>
#include <cstdlib>
#include <utility>

#define DEBUG_LOG_MODE 0
#define DEBUG_LOG_PATH stdout

#define printd(...) { if(DEBUG_LOG_MODE) fprintf(DEBUG_LOG_PATH, __VA_ARGS__); else fprintf(DEBUG_LOG_PATH, "DEBUG OFF\n");}

std::pair<const char*, size_t> writeFileToBuf(FILE* file, char* buf = 0, unsigned int size = 0);
size_t writeBufToFile(const char* buf, unsigned int size, FILE* file);

std::pair<const char*, size_t> writeFileToBuf(FILE* file, char* buf, unsigned int size){
    unsigned int charToRead = size? size : filelength(fileno(file));
    char* bufToWrite = buf? buf : ((char*) calloc(charToRead, 1));

    _read(fileno(file), bufToWrite, charToRead);
    return std::pair<const char*, size_t>(bufToWrite, charToRead);
}


size_t writeBufToFile(const char* buf, unsigned int size, FILE* file){
    return fwrite(buf, 1, size, file);
}

#endif

