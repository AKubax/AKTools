#include <TXLib.h>
#include <cstdio>
#include <stdexcept>
#include <windows.h>


CRITICAL_SECTION CriticalSection = {};

class Lock{
private:
    CRITICAL_SECTION* cs_;

public:
    Lock(CRITICAL_SECTION* cs):
        cs_ (cs)
    {
        EnterCriticalSection(cs_);
    }

    ~Lock(){
        LeaveCriticalSection(cs_);
    }
};

template <class T>
T getPoison();

template <>
int getPoison<int>(){
    return 0xABCDEF;
}

template <>
char getPoison<char>(){
    return '\0';
}

template <>
const char* getPoison<const char*>(){
    return (const char*) 0xABCDEF;
}

template <class T>
class AKQueue{
private:
    T* buf;
    size_t sz;

    bool isEmpty;

    T* begin;
    T* end;

    T* ptrInc(T* ptr);

public:
    AKQueue(size_t sz);
    ~AKQueue();

    AKQueue(const AKQueue<T>&) = delete;
    AKQueue<T>& operator = (const AKQueue<T>&) = delete;

    bool empty();

    void push(T arg);
    T pop();

    T* getBegin();
    T* getEnd();

    T* getBuf();
    size_t getSz();

    void dump();
};

template <class T>
AKQueue<T>::AKQueue(size_t sz_arg):
    buf     (new T[sz_arg]{}),
    sz      (sz_arg),
    isEmpty (1),
    begin   (buf),
    end     (buf)
{
    for(T* ptr = buf; ptr < buf + sz; ptr++){
        *ptr = getPoison<T>();
    }
}

template <class T>
AKQueue<T>::~AKQueue(){
    delete [] buf;
}

template <class T>
T* AKQueue<T>::ptrInc(T* ptr){
    if(ptr >= buf + sz - 1) return buf;
    else                    return ++ptr;
}


template <class T>
void AKQueue<T>::push(T arg){
    if(begin == end && !empty()) throw std::runtime_error("Queue overflow\n");

//    EnterCriticalSection(&CriticalSection);
    {
        Lock lck(&CriticalSection);

        *end = arg;
        end  = ptrInc(end);

        isEmpty = 0;
    }
}

template <class T>
T AKQueue<T>::pop(){
    {
        Lock lck(&CriticalSection);

        T retVal = *begin;

        begin = ptrInc(begin);

        isEmpty = begin == end;

        return retVal;
    }
}

template <class T>
T* AKQueue<T>::getBegin(){
    return begin;
}

template <class T>
T* AKQueue<T>::getEnd(){
    return end;
}

template <class T>
T* AKQueue<T>::getBuf(){
    return buf;
}

template <class T>
size_t AKQueue<T>::getSz(){
    return sz;
}

template <class T>
bool AKQueue<T>::empty(){
    return isEmpty;
}



