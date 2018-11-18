#include <cstdio>
#include <stdexcept>
#include "akio.h"

#define DEBUG_LOG_MODE 0

template <class T>
struct listBundle{
    T value;

    listBundle<T>* prevPtr;
    listBundle<T>* nextPtr;
};

//TODO: optimize size()
template <class T>
class AKList{
private:
    listBundle<T>* first_;
    listBundle<T>*  last_;

    listBundle<T>& bundleAt(size_t pos);

    size_t sz_;

public:
    AKList();
    AKList(const AKList<T>&) = delete;

    operator = (const AKList<T>&) = delete;

    ~AKList();

    size_t size() const;
    bool  empty() const;

    void push_back  (const T& val);
    void push_front (const T& val);
    T pop_back   ();
    T pop_front  ();
    void insert(size_t pos, const T& val);
    T remove(size_t pos);

    T& elemAt(size_t pos);
    T& operator [] (size_t pos);

    void resize(size_t sz, const T& val = T());
    void clear();
};

template <class T>
listBundle<T>& AKList<T>::bundleAt(size_t pos){$1
    $p("size() == %u; pos = %u\n", size(), pos);

    size_t ctr = 0;
    for(listBundle<T>* ptr = first_; ptr != nullptr; ptr = ptr->nextPtr, ctr++) if(ctr == pos) return *ptr;
    throw std::runtime_error("Index out of range in list\n");
}

template <class T>
AKList<T>::AKList():
    first_ (nullptr),
    last_  (nullptr),
    sz_          (0)
{}

template <class T>
AKList<T>::~AKList(){$1
    clear();
}

template <class T>
size_t AKList<T>::size() const{$1
//    size_t ctr = 0;
//    for(listBundle<T>* ptr = first_; ptr != nullptr; ptr = ptr->nextPtr, ctr++);
//    return ctr;
    $p("sz_ = %u\n", sz_);
    return sz_;
}

template <class T>
bool AKList<T>::empty() const{$1
    return !size();
}

template <class T>
void AKList<T>::push_back(const T& val){$1
//New bundle
    listBundle<T>* newBundlePtr = new listBundle<T>;        $
    newBundlePtr->value   =     val;                        $
    newBundlePtr->prevPtr =   last_;                        $
    newBundlePtr->nextPtr = nullptr;                        $
//Previous last
    if(!empty()) last_->nextPtr = newBundlePtr;
    else         first_ = newBundlePtr;                     $
//
    last_ = newBundlePtr;                                   $
//
    sz_++;
}

template <class T>
void AKList<T>::push_front(const T& val){$1
//New bundle
    listBundle<T>* newBundlePtr = new listBundle<T>;        $
    newBundlePtr->value   =     val;                        $
    newBundlePtr->prevPtr = nullptr;                        $
    newBundlePtr->nextPtr =  first_;                        $
//Previous first
    if(!empty()) first_->prevPtr = newBundlePtr;
    else         last_ = newBundlePtr;                      $
//
    first_ = newBundlePtr;                                  $
//
    sz_++;
}

template <class T>
T AKList<T>::pop_back(){$1
    if(empty()) throw std::runtime_error("An attempt to pop() from empty AKList\n");

    sz_--;

    T retValue = last_->value;                 $
    last_  = last_->prevPtr;                   $
    first_ = last_? first_ : nullptr;          $
    if(empty()) return retValue;               $

    delete last_->nextPtr;                     $
    last_->nextPtr = nullptr;                  $

    return retValue;
}

template <class T>
T AKList<T>::pop_front(){$1
    if(empty()) throw std::runtime_error("An attempt to pop() from empty AKList\n");

    sz_--;

    T retValue = first_->value;                  $
    first_ = first_->nextPtr;                    $
    last_  = first_? last_ : nullptr;            $
    if(empty()) return retValue;                 $

    delete first_->prevPtr;                      $
    first_->prevPtr = nullptr;                   $

    return retValue;
}

template <class T>
void AKList<T>::insert(size_t pos, const T& val){$1
    if((!pos) || pos >= size()) throw std::runtime_error("Position given to AKList.insert(size_t) is out of range\n");

    listBundle<T>& followBundle = bundleAt(pos);                                                                              $
    listBundle<T>& previoBundle = *(followBundle.prevPtr);                                                                    $

    listBundle<T>* newBundlePtr = new listBundle<T>;                                                                          $
    newBundlePtr->value = val;                                                                                                $
    newBundlePtr->prevPtr = &previoBundle;                                                                                    $
    newBundlePtr->nextPtr = &followBundle;                                                                                    $

    followBundle.prevPtr = newBundlePtr;                                                                                      $
    previoBundle.nextPtr = newBundlePtr;                                                                                      $

    sz_++;
}

template <class T>
T AKList<T>::remove(size_t pos){
    listBundle<T>& removeBundle = bundleAt(pos);

    listBundle<T>* previoBundlePtr = removeBundle.prevPtr;
    listBundle<T>* followBundlePtr = removeBundle.nextPtr;

    T retValue = removeBundle.value;
    delete &removeBundle;

    previoBundlePtr->nextPtr = followBundlePtr;
    followBundlePtr->prevPtr = previoBundlePtr;

    sz_--;

    return retValue;
}

template <class T>
T& AKList<T>::elemAt(size_t pos){$1
    return bundleAt(pos).value;
}

template <class T>
T& AKList<T>::operator [] (size_t pos){$1
    return elemAt(pos);
}

template <class T>
void AKList<T>::resize(size_t sz, const T& val/*=T()*/){$1
    if(sz == size()) return;

    size_t prevSz = size();                                                                                     $
    if(sz > prevSz) for(size_t i = 0; i < sz - prevSz; i++) push_back(val);
    else{
        size_t ctr = 0;                                                                                         $
        listBundle<T>* nextLast;                                                                                $
        for(listBundle<T>* ptr = first_; ptr != nullptr; ptr = ptr->nextPtr, ctr++) if(ctr >= sz){              $
            if(ctr == sz){
                if(ptr->prevPtr != 0){
                    nextLast = ptr->prevPtr;
                    nextLast->nextPtr = 0;
                }
                else{
                    nextLast = 0;
                    first_ = 0;
                }                                                                                               $
            }
            delete ptr;                                                                                         $
        }
        last_ = nextLast;                                                                                       $
    }                                                                                                           $

    sz_ = sz;
}

template <class T>
void AKList<T>::clear(){$1
    resize(0);
}

