#define AKDUMP(classname, code)   void dump(const char* msg = "") const{\
                                    printf("%s [0x%p]{ //%s\n", #classname, this, msg);\
                                    bool isOK = ok();\
                                    if(!isOK){ $m } \
                                    printf("    ok() result is %s\n\n", isOK? "true" : "false");  $d\
                                    {code}\
                                    printf("}\n");\
                                }

#ifdef DEBUG_VER
#define DUMP(str) { dump(str) }
#define PRINTD(...) {$s $r printf("%s (%d) %s: ", __FILE__, __LINE__, __TX_FUNCTION__); printf(__VA_ARGS__);}

#else
#define DUMP(str) {}
#define PRINTD(...) {}

#endif

#define PRINTVAR(varname)   {printvar(#varname, varname);}

void printvar(const char* varname, long var);
void printvar(const char* varname, long var){
    printf("    -%s: %ld\n", varname, var);
}

void printvar(const char* varname, unsigned int var);
void printvar(const char* varname, unsigned int var){
    printf("    -%s: %u\n", varname, var);
}

void printvar(const char* varname, unsigned long var);
void printvar(const char* varname, unsigned long var){
    printf("    -%s: %lu\n", varname, var);
}

void printvar(const char* varname, double var);
void printvar(const char* varname, double var){
    printf("    -%s: %lf\n", varname, var);
}

void printvar(const char* varname, char* var);
void printvar(const char* varname, char* var){
    printf("    -%s: %s\n", varname, var);
}

void printvar(const char* varname, const char* var);
void printvar(const char* varname, const char* var){
    printf("    -%s: \"%s\"\n", varname, var);
}

void printvar(const char* varname, const void* var);
void printvar(const char* varname, const void* var){
    printf("    -%s: 0x%p\n", varname, var);
}

void printvar(const char* varname, const bool var);
void printvar(const char* varname, const bool var){
    printf("    -%s: %s\n", varname, var? "True" : "False");
}

template <class T>
void printvar(const char* varname, const T& var){
    printf("    -Dump for %s: ", varname); var.dump();
}


