#ifndef AKStack_FILE
#define AKStack_FILE

#include "..\TXLib.h"


#include <inttypes.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <exception>
#include <stdexcept>

#include "..\AKTools\AKString.h"
#include "..\AKTools\AKDump.h"

AKString getVarStr(int var);
AKString getVarStr(double var);

class StackUnderflow : public std::runtime_error
{
public:
    StackUnderflow(const char* str = "EVRISSING VERI BED") :
        std::runtime_error (str)
    {}
};

template <class T>
class AKVector{
private:

    T* buf_ = 0;
    size_t capacity_ = 0;
    size_t size_ = 0;

public:

    AKVector(size_t capacity = 0);
    AKVector(const AKVector& vec) = delete;

	~AKVector();

    void reserve(size_t capacity);
    void push_back(T var);
    T pop_back();
    T back();

    T* data();

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

template <typename T>
AKVector<T>::AKVector(size_t capacity):
    buf_ (capacity > 0? new T[capacity] : NULL),
    capacity_   (capacity)
{}

template <typename T>
AKVector<T>::~AKVector() {
	delete[] buf_;
}

//-----------------------------------------------------------------------------

template <typename T>
void AKVector<T>::reserve(size_t capacity){
    if     (capacity <= capacity_) return;

    else{
        T* prevBuf = buf_;
        buf_ = new T[capacity];
        std::copy(prevBuf, prevBuf + size_, buf_);
		delete[] prevBuf;
        capacity_ = capacity;
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

    if(!(0 <= size_ && size_ <= capacity_)) { isOK = false; PRINTD(4, "ok() failed on this: if(!(0 <= size_ && size_ <= capacity_))\n"); }
    if((buf_ == NULL) ^  (capacity_ == 0))     { isOK = false; PRINTD(4, "if(!(buf_ == NULL && capacity_ == 0))\n"); }

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
