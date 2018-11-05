#ifndef AKTOKENIZER_FILE
#define AKTOKENIZER_FILE

#include <utility>
#include <vector>
#include <string>

#include "akio.h"


struct Token{
    union{

        const char* str;
        double      dbl;

    } value;

    const char* c_str() const{
        if(isStr) return value.str;

        char* ret = (char*) calloc(20, 1);
        sprintf(ret, "%lf", value.dbl);
        return ret;
    }

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

    Token (const Token& t):
        value (t.value),
        isStr (t.isStr)
    {}

//
//    char* toString (){
//        if(isStr) return value.str;
//
//        char* res = (char*) calloc(20, 1);
//        snprintf(res, 20, "%lf", value.dbl);
//        return res;
//    }
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
    printd("Begin of tokenize func. inp = '%s'\n", inp);

    std::vector<char*> splitInp = split(inp, skipComments);
    std::vector<Token> out;

    printd("split() worked successfully\nIts value = {");
    for(auto it : splitInp) printd("\"%s\" ,", it);
    printd("}\n");

    for(auto& elem : splitInp){
        printd("\telem = '%s'\n", elem);

        char* endPtr = 0;
        double dblValue = 0;

        dblValue = std::strtod(elem, &endPtr);

        if(endPtr == elem)  out.push_back(      Token(elem));
        else                out.push_back(  Token(dblValue));
    }

    return out;
}

std::vector<Token> tokenize(const char* inp, bool skipComments = 1);
std::vector<Token> tokenize(const char* inp, bool skipComments /* = 1*/){
    char* copyInp = (char*) calloc(strlen(inp) + 1, 1);
    strcpy(copyInp, inp);

    return tokenize(copyInp, skipComments);
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
