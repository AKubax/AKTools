#ifndef AKTOKENIZER_FILE
#define AKTOKENIZER_FILE

#include <utility>
#include <vector>
#include <string>

#include "akio.h"

#undef DEBUG_LOG_MODE
#define DEBUG_LOG_MODE 1

struct Token{
    union{

        const char* str;
        double      dbl;

    } value;

    bool isStr;

    Token ():
        value (),
        isStr (0)
    {}

    Token (const char* s):
        value (),
        isStr (1)
    { value.str = s; }

    Token (double d):
        value (),
        isStr (0)
    { value.dbl = d; }
};

std::vector<char*> split(char* inp, bool skipComments = 1);
std::vector<char*> split(char* inp, bool skipComments/*= 1*/){
    std::vector<char*> out;

    for(char* ptr = inp; *ptr != '\0'; ){
        while( (ptr[0] == '/' && ptr[1] == '*') || isspace(ptr[0]) ){
            if(ptr[0] == '\0') return out;


            if(skipComments) if(ptr[0] == '/' && ptr[1] == '*'){
                while( !(ptr[0] == '*' && ptr[1] == '/') ){
                    ptr++;

                    if(ptr[0] == '\0')  return out;
                }
                ptr += 2;
            }

            while( isspace(ptr[0]) ) ptr++;
        }

        if(ptr[0] == '\0') return out;

        out.push_back(ptr);
        while( (!isspace(ptr[0])) && ptr[0] != '\0') ptr++;
        ptr[0] = '\0';
        ptr++;
    }

    return out;
}

std::vector<Token> tokenize(char* inp, bool skipComments = 1);
std::vector<Token> tokenize(char* inp, bool skipComments/*= 1*/){
    std::vector<char*> splitInp = split(inp, skipComments);
    std::vector<Token> out;

    printd("split() worked successfully\n");

    for(auto& elem : splitInp){
        char* endPtr = 0;
        double dblValue = 0;

        dblValue = std::strtod(elem, &endPtr);

        if(endPtr == elem)  out.push_back(      Token(elem));
        else                out.push_back(  Token(dblValue));
    }

    return out;
}

//Token* tokenize(char* inp){
//    AKVector<Token> out;
//    bool isComment = 0;
//
//    for(char* ptr = inp; *ptr != '\0'; ptr++){
//        if(
//    }
//}


#endif
