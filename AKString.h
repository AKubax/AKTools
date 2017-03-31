#ifndef AKString_FILE
#define AKString_FILE

#include "C:\cppProjects\TXLib.h"

#include <inttypes.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <exception>
#include <stdexcept>
#include "AKDump.h"

#define $ {PRINTD("Line: %d\n", __LINE__);}

int max(int a, int b);
int max(int a, int b){
    return a > b? a : b;
}





//=============================================================================
//{  Kernel for AKString

class AKStringKernel{
private:
    char* ptr;
    size_t nowLen;
    size_t maxSize;

    bool isDynamic(size_t len) const;
    bool isDynamic() const;


public:
    AKStringKernel(const char* str, size_t copyLen, size_t bufLen);
    ~AKStringKernel();

    AKStringKernel(const AKStringKernel&) = delete;
    AKStringKernel& operator = (const AKStringKernel&) = delete;

    const char* data() const;
    char* data();    //TODO make it private

    size_t size() const;

    void add(const char* str, size_t len);

    void resize(size_t len, bool smart = true);
    void update(const char* str, size_t copyLen, bool haveResize = true);

    bool ok() const;

    AKDUMP(AKStringKernel,{
        PRINTVAR(isDynamic());
        PRINTVAR((const void*) ptr);
        PRINTVAR(data());
        PRINTVAR(nowLen);
        PRINTVAR(strlen(data()))
        PRINTVAR(maxSize);
    })
};

//=============================================================================


AKStringKernel::AKStringKernel(const char* str, size_t copyLen, size_t bufLen):
    ptr (NULL),
    nowLen (0), //It will be defined in update
    maxSize (0)  //It will be defined in resize
{
    if(copyLen > bufLen) std::runtime_error("CopyLen can't be greater than bufLen");

    if(bufLen){
        resize(bufLen, false);
        update(str, copyLen, false);
    }
    else{
        update(str, copyLen);
    }
}

AKStringKernel::~AKStringKernel() {
    if(isDynamic()) free(ptr);

    ptr = NULL;
}



bool AKStringKernel::isDynamic(size_t sz) const{
    return (sz * sizeof(char)) > sizeof(ptr);
}
bool AKStringKernel::isDynamic() const{
    return isDynamic(maxSize);
}


const char* AKStringKernel::data() const{
    if(isDynamic()) return ptr;

    else   return (const char*) &ptr;
}

char* AKStringKernel::data(){
    return const_cast<char*>(((const AKStringKernel*) (this))->data());
}

size_t AKStringKernel::size() const{
    return nowLen;
}

void AKStringKernel::add(const char* str, size_t len){
    size_t prevSize = size();
    resize(size() + len);
    strncpy(ptr + prevSize, str, len + 1);
}

void AKStringKernel::resize(size_t len, bool smart /*= true*/){        //It cares about zero symbol
$    bool isNeeded = false;
$    len++;
$
$    bool isPrevDynamic = isDynamic(maxSize);
$    bool isNextDynamic = isDynamic(len);
$
$    size_t nextSize = -1u;
$
$    if(smart){
$        if(isNextDynamic){
$            if(len < maxSize * 0.7){
$                nextSize = static_cast<size_t>(len * 1.3);
$                isNeeded = true;
$            }

            else if(len > maxSize){
$                nextSize = static_cast<size_t>(len * 1.3);
$                isNeeded = true;
$            }
$        }

        else{
$            nextSize = sizeof(ptr);
$            isNeeded = isPrevDynamic;
$        }
$    }

    else{
$        nextSize = nextSize < 4? 4 : len;
$        isNeeded = true;
$    }

/*
    if(smart){
        if(!isDynamic()){

        }

        if(len < sizeof(ptr) - 1){
            maxSize = sizeof(ptr);
            isNeeded = true;
        }
        else if(len < maxSize * 0.5){
            maxSize = (int) (len * 0.7 + 1);
            isNeeded = true;
        }

        else if(len > maxSize){
            maxSize = (int) (len * 1.2 + 1);                                 //TODO  Print what case there is and info about it
            isNeeded = true;
        }

        else if(!isDynamic() && len >= sizeof(ptr)){
            maxSize = (int) (len * 1.2 + 1);
            isNeeded = true;
        }
    }

    else{
        maxSize = len;
        isNeeded = true;
    }
*/

$    if(isNeeded){
//        PRINTD("isDynamic() returns %d, isDynamic(len) returns %d, len is %lu, nextSize is %lu \n\n", isDynamic(), isDynamic(len), (unsigned long) len, (unsigned long) nextSize);
$
$        if(isPrevDynamic && isNextDynamic){
//          PRINTD("Case: Dynamic -> Dynamic");
$
//$            PRINTD("nowLen: %d, nextSize: %d\n", nowLen, nextSize);
$
$            char* res = (char*) ( realloc(ptr, nextSize * sizeof(char)) );
$            memset(res + len, '\0', (nextSize - len) * sizeof(char));
$
$            if(res == NULL){
$                throw std::runtime_error("realloc() doesn't have enough memory");
$            }
$
$            ptr = res;
$            ptr[nextSize - 1] = '\0';
$
$            maxSize = nextSize;
$        }
$
$        if(isPrevDynamic && !isNextDynamic){
//            PRINTD("Case: Dynamic -> Static");
$
$            char* str = ptr;
$
$            ptr = 0;
$            strncpy((char*) &ptr, str, sizeof(ptr) - 1);
$
$            free(str);
$
$            maxSize = sizeof(ptr);
$        }
$
$        if(!isPrevDynamic && isNextDynamic){
$//            PRINTD("Case: Static -> Dynamic\n");
$//            PRINTD("nextSize: %u \n", nextSize);
$//            DUMP("Before copy");
$
$            uintptr_t str = (uintptr_t) ptr;
$//            PRINTD("str is 0x%p \"%s\" \n", str, (const char*) &str);
$
$            ptr = (char*) calloc(nextSize, sizeof(char));        //TODO Use = (int) instead of strcpy
$            strncpy(ptr, (const char*) &str, len);
$
$            maxSize = nextSize;
$//
$//            DUMP("After copy");
$        }
$
$        if(!isPrevDynamic && !isNextDynamic){
$//            PRINTD("Case: Static -> Static");
$
$            maxSize = sizeof(ptr);
$        }
$    }

     nowLen = --len;
}

void AKStringKernel::update(const char* str, size_t copyLen, bool haveResize /*= true*/){
//    PRINTD("PARAMETERS| str: \"%s\"    copyLen: %lu", str, copyLen);
$    size_t strLen = strlen(str);
$
     PRINTD("str: \"%s\"; copyLen: %d\n", str, copyLen);

$    if(copyLen > strLen)  copyLen = strLen;
$
$    if(haveResize){
$        resize(copyLen);
$    }

     DUMP("AFTER RESIZE");
$
$    nowLen = copyLen;
     DUMP("AFTER NOWLEN = COPYLEN");
$    strncpy(data(), str, copyLen);
     *(data() + copyLen) = '\0';

     DUMP("AFTER STRNCPY");
}

bool AKStringKernel::ok() const{
    bool isOK = true;

    if(this == 0) return false;

    if(nowLen != strlen(data())) isOK = false;

    return isOK;
}

//}
//=============================================================================


//=============================================================================
//{  Shell for AKString

class AKString{
private:
    AKStringKernel krn;

public:
    AKString(const char* str = "", size_t copyLen = UINT_MAX - 1, size_t bufLen = 0);
    explicit AKString(size_t len);
    AKString(const AKString& astr = "", size_t copyLen = UINT_MAX, size_t bufLen = 0);

    ~AKString();
    //-----------------------------------------------------------------------------
    const char* c_str() const;

    const char* data() const;

    AKString substr(size_t begin, size_t end) const;

    void dump(const char* str) const;

    size_t size() const;

    bool isEqual(const AKString& astr) const;
    bool isEqual(const char* str) const;

    bool ok() const;
    //-----------------------------------------------------------------------------
    void add(const char* str);
    void add(const AKString& astr);

    const char& charAt(size_t index) const;

    void clear();

    void replace(size_t first, size_t len, const char* str);
    //=============================================================================
    AKString& operator = (const char* str){
        krn.update(str, strlen(str));
        return *this;
    }
    AKString& operator = (const AKString& astr){
        PRINTD("astr.c_str(): \"%s\"\n", astr.c_str());
        krn.update(astr.c_str(), astr.size());
        return *this;
    }

    AKString& operator += (const AKString& astr){
        add(astr.c_str());
        return *this;
    }
    AKString& operator += (const char* str){
        add(str);
        return *this;
    }


    AKString operator + (const char* str) const{
        return AKString(*this) += str;
    }
    AKString operator + (const AKString& astr) const{
        return AKString(*this) += astr;
    }


    const char& operator [] (size_t index) const{
        return charAt(index);
    }


    bool operator == (const AKString& astr) const{
        return isEqual(astr);
    }
    bool operator == (const char* str) const{
        return isEqual(str);
    }

    bool operator > (const AKString& astr) const{
        return (strcmp(c_str(), astr.c_str()) > 0);
    }
    bool operator > (const char* str) const{
        return (strcmp(c_str(), str) > 0);
    }

    bool operator < (const AKString& astr) const{
        return (strcmp(c_str(), astr.c_str()) < 0);
    }
    bool operator < (const char* str) const{
        return (strcmp(c_str(), str) < 0);
    }

    bool operator >= (const AKString& astr) const{
        return (strcmp(c_str(), astr.c_str()) >= 0);
    }
    bool operator >= (const char* str) const{
        return (strcmp(c_str(), str) >= 0);
    }

    bool operator <= (const AKString& astr) const{
        return (strcmp(c_str(), astr.c_str()) <= 0);
    }
    bool operator <= (const char* str) const{
        return (strcmp(c_str(), str) <= 0);
    }
};
//{----------------------------------------------------------------------------
//!
//}----------------------------------------------------------------------------
AKString::AKString(const char* str/*= ""*/, size_t copyLen/*= UINT_MAX*/, size_t bufLen/*= 0*/):
    krn (str, copyLen, bufLen)
{}

AKString::AKString(size_t bufLen):
    krn ("", 0, bufLen)
{}

AKString::AKString(const AKString& astr/*= ""*/, size_t copyLen/*= UINT_MAX*/, size_t bufLen/*= 0*/):
    krn (astr.c_str(), copyLen, bufLen)
{}


AKString::~AKString()
{}
//-----------------------------------------------------------------------------
const char* AKString::c_str() const{
    return data();
}

const char* AKString::data() const{
    return krn.data();
}

AKString AKString::substr(size_t begin, size_t end) const{
    PRINTD("Beginning of substr() func\n");

    if(begin > end) throw std::runtime_error("First position can't be further then last");
    if(end >= size()) throw std::runtime_error("Out of string's bounds");

    PRINTD("begin: %d; end: %d\n", begin, end);

    AKString as(c_str() + begin, end - begin);

    PRINTD("Ending of substr() func\n");

    return as;
}

void AKString::dump(const char* str) const{
    $y printf("Dump is called from AKString (Shell) \n"
            "Dump for AKStringKernel: \n"); $d
    krn.dump(str);
}

size_t AKString::size() const{
    return krn.size();
}

bool AKString::isEqual(const AKString& astr) const{
    if(size() != astr.size()) return false;

    return (strcmp(c_str(), astr.c_str()) == 0);
}
bool AKString::isEqual(const char* str) const{
    if(size() != strlen(str)) return false;

    return (strcmp(c_str(), str) == 0);
}

bool AKString::ok() const{
    return krn.ok();
}
//-----------------------------------------------------------------------------
void AKString::add(const char* str){
    krn.add(str, strlen(str));
}
void AKString::add(const AKString& astr){
    krn.add(astr.c_str(), astr.size());
}

const char& AKString::charAt(size_t index) const{
    if(index >= size())
        throw std::runtime_error("Invalid index for this AKString");

    return krn.data()[index];
}

void AKString::clear(){
    krn.update("", false);
}

void AKString::replace(size_t first, size_t len, const char* str){
    if(strlen(str) != len) throw std::runtime_error("String to be placed instead of another doesn't has the right length");

    strncpy((char*) (data() + first), str, len);
}

//}
//=============================================================================

#endif
