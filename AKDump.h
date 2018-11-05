#ifndef AKDump_FILE
#define AKDump_FILE

#include <TXLib.h>

#define AKDUMP(classname, code)   void dump(const char* msg = "") const{\
                                    printf("%s [0x%p]{ //%s\n", #classname, this, msg);\
                                    bool isOK = ok();\
                                    if(!isOK){ $m } \
                                    printf("  ok() result is %s\n\n", isOK? "true" : "false");  $d\
                                    {code}\
                                    printf("}\n\n");\
                                }



//=============================================================================
//{  AKFuncTab
class AKFuncTab{
public:
    static int level;

    AKFuncTab(){
        level++;
    }

    ~AKFuncTab(){
        level--;
    }
};

int AKFuncTab::level = 0;

//}
//=============================================================================



#ifndef DEBUG_CHANNEL
    #define DEBUG_CHANNEL printf
#endif

#ifdef DEBUG_LVL
#define DEBUG_PREFIX()  DEBUG_CHANNEL("%-60s (%3d) |%*s%s: ",  __FILE__, __LINE__, AKFuncTab::level * 2, "", __TX_FUNCTION__)
#define DUMP(lvl, source, str)      {if(lvl <= GlobalDebugLevel) (source).dump(str); }
#define PRINTD(level, ...)  {if((level) <= GlobalDebugLevel){ $s $r DEBUG_PREFIX(); DEBUG_CHANNEL(__VA_ARGS__);}}
#ifdef $
    #undef $
#endif
#define $                   {PRINTD(10, "\n");}
#define $1                  AKFuncTab __________AKEntryObject__________; $

#else
#define DUMP(lvl, source, str)   {}
#define PRINTD(...) {}
#define $           {}
#define $1          {}

#define DEBUG_LVL 0

#endif

unsigned int GlobalDebugLevel = DEBUG_LVL;



#define PRINTVAR(varname)   {printvar(#varname, varname);}

void printvar(const char* varname, int var);
void printvar(const char* varname, int var){
    printf("%s : %d\n", varname, var);
}

void printvar(const char* varname, long var);
void printvar(const char* varname, long var){
    printf("%s : %ld\n", varname, var);
}

void printvar(const char* varname, unsigned int var);
void printvar(const char* varname, unsigned int var){
    printf("%s : %u\n", varname, var);
}

void printvar(const char* varname, unsigned long var);
void printvar(const char* varname, unsigned long var){
    printf("%s : %lu\n", varname, var);
}

void printvar(const char* varname, unsigned long long var);
void printvar(const char* varname, unsigned long long var){
    printf("%s : %zu\n", varname, var);
}

void printvar(const char* varname, double var);
void printvar(const char* varname, double var){
    printf("%s : %lf\n", varname, var);
}

void printvar(const char* varname, ptrdiff_t var);
void printvar(const char* varname, ptrdiff_t var){
    printf("%s : %lld\n", varname, (long long) var);
}

void printvar(const char* varname, const char var);
void printvar(const char* varname, const char var){
    printf("%s : %c[%d]\n", varname, var, var);
}

void printvar(const char* varname, char* var);
void printvar(const char* varname, char* var){
    printf("%s : \"%s\"\n", varname, var);
}

void printvar(const char* varname, const char* var);
void printvar(const char* varname, const char* var){
    printf("%s : \"%s\"\n", varname, var);
}

void printvar(const char* varname, const void* var);
void printvar(const char* varname, const void* var){
    printf("%s : 0x%p\n", varname, var);
}

void printvar(const char* varname, const bool var);
void printvar(const char* varname, const bool var){
    printf("%s : %s\n", varname, var? "True" : "False");
}

template <class T>
void printvar(const char* varname, const T& var){
    printf("%s : \"%s\"\n", varname, var.toString());
}


#endif
