#ifndef AKStack_FILE
#define AKStack_FILE

#include "..\TXLib.h"


#include <inttypes.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <exception>
#include <stdexcept>
#include <functional>

#include "..\AKTools\AKString.h"
#include "..\AKTools\AKDump.h"


const size_t SomeConst = 0xBEDABEDA;

AKString getVarStr(int var);
AKString getVarStr(double var);

class StackUnderflow : public std::runtime_error
{
public:
    StackUnderflow(const char* str = "EVRISSING VERI BED") :
        std::runtime_error (str)
    {}
};

size_t hash(const void* buf, size_t sz);
size_t hash(const void* buf, size_t sz){
    size_t sum = 0;
    for(size_t i = 0; i < sz; i++) sum ^= ((const char*) buf)[i] * i;
    return sum;
}

template <class T>
class AKVector{
public:
    size_t CANARY_begin = ((size_t) this) ^ SomeConst;

    T* buf_ = 0;

    size_t capacity_ = 0;
    size_t size_ = 0;

    size_t bufCanary = 0;
    size_t checkSumBuf = 0;
    const size_t checkSum;

    size_t CANARY_end = ((size_t) this) ^ SomeConst;

//public:

    AKVector(size_t capacity = 0);
    AKVector(const AKVector& vec) = delete;

	~AKVector();

    void bufLock();
    bool bufCheck() const;

    void reserve(size_t capacity);
    void push_back(T var);
    T pop_back();
    T back();

    T* data();
    T* begin();
    T* end();

    void clear();
    bool isEmpty();

    const T& at(long pos) const;
    T& at(long pos);
    bool ok() const;
    AKString toString() const;
    size_t size() const;

    AKDUMP(AKVector,{
            PRINTVAR(toString());
            PRINTVAR(capacity_);
            PRINTVAR(size_);
            }
        )

    //-----------------------------------------------------------------------------

    const T& operator [](long pos) const;
    T& operator [](long pos);

    AKVector& operator = (const AKVector& vec) = delete;
};

//{----------------------------------------------------------------------------
//!
//}----------------------------------------------------------------------------

//=============================================================================

template <typename T>
AKVector<T>::AKVector(size_t capacity):
    buf_ (capacity > 0? new T[capacity + 2] + 1 : NULL),
    capacity_   (capacity),
    checkSum (hash(this, sizeof(AKVector<T>)))
{}

template <typename T>
AKVector<T>::~AKVector() {
	delete[] (buf_ - 1);
}

//-----------------------------------------------------------------------------

template <class T>
void AKVector<T>::bufLock(){
    checkSumBuf = hash(buf_, capacity_);
}

template <class T>
bool AKVector<T>::bufCheck() const{
    return checkSumBuf == hash(buf_, capacity_);
}

template <typename T>
void AKVector<T>::reserve(size_t capacity){
    if     (capacity <= capacity_) return;

    else{
        T* prevBuf = buf_ - 1;
        buf_ = (new T[capacity + 2]) + 1;
        std::copy(prevBuf + 1, prevBuf + 1 + size_, buf_);
		delete[] prevBuf;
        capacity_ = capacity;

        bufCanary = (size_t) buf_ ^ SomeConst;

        (buf_ - 1)[0]         = bufCanary;
        (buf_ + capacity_)[0] = bufCanary;

        bufLock();
    }
}

template <typename T>
void AKVector<T>::push_back(T var){
    if(size_ == capacity_) reserve((size_t) ceil(capacity_ * 1.25 + 1));

    buf_[size_++] = var;
}

template <typename T>
T AKVector<T>::pop_back(){
    T r_val = back();
    T* p = &(buf_[size() - 1]);
    p->~T();
    size_--;
    return r_val;
}

template <typename T>
T AKVector<T>::back(){
    if(size() == 0) throw StackUnderflow("Don't try to underflow stack. It's pointless (and Santa Claus won't give you a present)");

    return buf_[size() - 1];
}

template <typename T>
void AKVector<T>::clear(){
    for(auto& elem : buf_){
        ~elem();
    }
    size_ = 0;
}

template <typename T>
T* AKVector<T>::data(){
    return buf_;
}

template <typename T>
bool AKVector<T>::isEmpty(){
    return (size_ == 0);
}

template <typename T>
T* AKVector<T>::begin(){
    return data();
}

template <typename T>
T* AKVector<T>::end(){
    return data() + size();
}

template <typename T>
T& AKVector<T>::at(long pos){
    if(!(0 <= pos && (size_t) pos < capacity_)) throw std::runtime_error("Wrong argument in func at()");

    return buf_[(size_t) pos];
}

template <typename T>
const T& AKVector<T>::at(long pos) const{
    return (const_cast<AKVector<T>>(*this).at(pos));
}

template <typename T>
AKString AKVector<T>::toString() const{
    AKString str("");
    for(size_t i = 0; i < size_; i++){
        str += "[";
        str += getVarStr(buf_[i]);
        str += "] ";
    }
    return str;
}

template <typename T>
bool AKVector<T>::ok() const{
    bool isOK = true;
    int errNum = 0;

    if(!(0 <= size_ && size_ <= capacity_)) { isOK = false; errNum = 1; PRINTD(4, "ok() failed on this: if(!(0 <= size_ && size_ <= capacity_))\n"); }
    if((buf_ == NULL) ^  (capacity_ == 0))  { isOK = false; errNum = 2;PRINTD(4, "if(!(buf_ == NULL && capacity_ == 0))\n"); }

    if(!(CANARY_begin == CANARY_end && CANARY_begin == ( ((size_t) this) ^ SomeConst ) ))      { isOK = 0; errNum = 3; }
    if(checkSum != hash(this, sizeof(AKVector<T>)))                                           { isOK = 0; errNum = 4; }

    isOK &= bufCheck();

    if(!isOK) printf("ok() returns false. errNum = %d\n", errNum? errNum : 5);
    return isOK;
}

template <typename T>
size_t AKVector<T>::size() const{
    return size_;
}

//-----------------------------------------------------------------------------
template <typename T>
const T& AKVector<T>::operator [](long pos) const{
    (const_cast<AKVector<T>>(*this).at(pos));
}

template <typename T>
T& AKVector<T>::operator [](long pos){
    return at(pos);
}

//{----------------------------------------------------------------------------
//!
//}----------------------------------------------------------------------------

AKString getVarStr(int var){
    AKString str(8);
    _itoa(var, const_cast<char*>(str.data()), 10);
    return str;
}

AKString getVarStr(double var){
    AKString str(8);
    _itoa((int) var, const_cast<char*>(str.data()), 10);
    return str;
}


#endif
