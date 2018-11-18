#ifndef AKIO_FILE
#define AKIO_FILE

#include <io.h>
#include <stdio.h>
#include <cstdlib>
#include <utility>

#define DEBUG_LOG_MODE 0
#define DEBUG_LOG_PATH stdout

#define printd(...) ( (DEBUG_LOG_MODE)? fprintf(DEBUG_LOG_PATH, "%*s", 4 * PRINTD_TABULATION_STRUCT::level, "") + fprintf(DEBUG_LOG_PATH, "# " __VA_ARGS__) : 0 )

#define $       ( printd("/-/-/-/\"%s\" [%d] (%s)\n", __FILE__, __LINE__, __FUNCTION__) );

#define $1      printd("-----%s-Entry----->\n", __FUNCTION__); PRINTD_TABULATION_STRUCT ______printd_tabulation_struct______; $

#define $p(...)  { $ printd(">>>>>>" __VA_ARGS__); }

struct PRINTD_TABULATION_STRUCT{
    static int level;

    PRINTD_TABULATION_STRUCT(){
        printd("up ^^^\n");
        level++;
    }

    ~PRINTD_TABULATION_STRUCT(){
        printd("down vvv\n");
        level--;
    }
};

int PRINTD_TABULATION_STRUCT::level = 0;

std::pair<const char*, size_t> writeFileToBuf(FILE* file, char* buf = 0, unsigned int size = 0);
size_t writeBufToFile(const char* buf, size_t size, FILE* file);

std::pair<const char*, size_t> writeFileToBuf(FILE* file, char* buf, unsigned int size){
    unsigned int charToRead = size? size : filelength(fileno(file));
    char* bufToWrite = buf? buf : ((char*) calloc(charToRead + 1, 1));

    _read(fileno(file), bufToWrite, charToRead);
//    printf("_read has read %d bytes\n", _read(fileno(file), bufToWrite, charToRead));
//    printf("It has returned\n'%s'\nIts length is %d\n", bufToWrite, strlen(bufToWrite));
    return std::pair<const char*, size_t>(bufToWrite, charToRead);
}


size_t writeBufToFile(const char* buf, size_t size, FILE* file){
    return fwrite(buf, 1, size, file);
}

#endif

