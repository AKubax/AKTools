#ifndef AKTOKENIZER_FILE
#define AKTOKENIZER_FILE


#include "AKVector.h"

struct Token{
    union{

        const char* str;
        double      dbl;

    } value;

    bool isStr = 0;

    Token (const char* s):
        isStr (1)
    { value.str = s; }

    Token (double d):
        isStr(0)
    { value.dbl = d; }
};

const char** divide(char* inp, bool skipComments = 1){
    AKVector<const char*> out;

    for(char* ptr = inp; *ptr != '\0'; ){
        if(skipComments) if(ptr[0] == '/' && ptr[1] == '*'){ while( !(ptr[0] == '*' && ptr[1] == '/') ) ptr++; ptr += 2; }
        while(ptr[0] == ' ' || ptr[0] == '\n') ptr++;

        out.push_back(ptr);
        while(ptr[0] != ' ' && ptr[0] != '\n') ptr++;
        ptr[0] = '\0';

        while(ptr[0] == ' ' || ptr[0] == '\n') ptr++;
    }

    return out.data();
}

//Token* tokenize(const char* inp){
//    AKVector<Token> out;
//    bool isComment = 0;
//
//    for(char* ptr = inp; *ptr != '\0'; ptr++){
//        if(
//    }
//}


#endif
