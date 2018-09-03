#ifndef AKSTACK_FILE
#define AKSTACK_FILE

#include "../TXLib.h"

#include "AKVector.h"

#define $mytest(code, ethalon)  { auto code_res = (code);\
                                  if((code_res) == (ethalon)) { $so printf("[SUCCESS] %s", #code); }\
                                  else { $se printf("[FAILED] %s == ", #code); printvar("", code_res); printf("Should be "); printvar("", ethalon);}  $n; }

template <class T>
class AKStack
{
private:
    AKVector<T> vec;
public:
    AKStack (size_t sz = 0);

    void push (T arg);

    void pop ();

    T top ();

    void dump();
    bool ok();
};

template <class T>
AKStack<T>::AKStack (size_t sz/*= 0*/):
    vec (sz)
{}

template <class T>
void AKStack<T>::push (T arg)
{
    vec.push_back (arg);
}

template <class T>
void AKStack<T>::pop()
{
    vec.pop_back();
}

template <class T>
T AKStack<T>::top()
{
    return vec.back();
}

template <class T>
void AKStack<T>::dump(){
    printf("AKStack dump\n");

    vec.dump();
}

template <class T>
bool AKStack<T>::ok(){
    return vec.ok();
}


#endif
