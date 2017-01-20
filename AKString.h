#ifndef AKString_FILE
#define AKString_FILE

#include <TXLib.h>

#include <inttypes.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <exception>
#include <stdexcept>

int max(int a, int b){
    return a > b? a : b;
}





//=============================================================================
//{  Kernel for AKString

class AKStringKernel{
private:
    char* ptr;
    size_t nowSize;
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

    void dump() const;

    size_t size() const;

    void resize(size_t len, bool smart = true);
    void update(const char* str, size_t copyLen, bool haveResize = true);
};

//=============================================================================

AKStringKernel::AKStringKernel(const char* str, size_t copyLen, size_t bufLen):
    ptr (NULL),
    nowSize (copyLen), //It will be defined in update
    maxSize (bufLen)  //It will be defined in resize
{
    if(copyLen > bufLen) std::runtime_error("CopyLen can't be greater than bufLen");

    resize(bufLen, false);
    update(str, copyLen, false);
}

AKStringKernel::~AKStringKernel() {
    free(ptr);
    ptr = NULL;
}



bool AKStringKernel::isDynamic(size_t len) const{
    return len * sizeof(char) > sizeof(ptr);
}
bool AKStringKernel::isDynamic() const{
    return isDynamic(maxSize);
}


const char* AKStringKernel::data() const{
    if(isDynamic()) return ptr;

    else   return (const char*) &ptr;
}

char* AKStringKernel::data(){
    return (char*) ((const AKStringKernel*) (this))->data();
}

size_t AKStringKernel::size() const{
    return nowSize;
}




void AKStringKernel::dump() const{
    printf("\n AKString{ \n");
    if(isDynamic()) printf("\t ptr pointer : 0x%p  |  ptr : \"%s\" (dynamic) \n", ptr, ptr);
    else            printf("\t ptr pointer : 0x%p  |  ptr : \"%s\" (static) \n", &ptr, &ptr);
    printf("\t nowSize     : %lu \n", (unsigned long) nowSize);
    printf("\t maxSize     : %lu \n", (unsigned long) maxSize);
    printf("} \n\n");
}


void AKStringKernel::resize(size_t len, bool smart /*= true*/){        //It cares about zero symbol
    bool isNeeded = false;

    if(smart){
        if(len < sizeof(ptr)){
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

    if(isNeeded){
        if(isDynamic() && isDynamic(len)){
            char* res = (char*) ( realloc(ptr, maxSize * sizeof(char)) );
            memset(res + nowSize, '\0', (maxSize - nowSize) * sizeof(char));

            if(res == NULL){
                throw std::runtime_error("realloc() doesn't have enough memory");
            }

            ptr = res;
            ptr[maxSize - 1] = '\0';
        }

        if(isDynamic() && !isDynamic(len)){
            char* str = ptr;

            ptr = 0;
            strncpy((char*) &ptr, str, sizeof(ptr) - 1);

            free(str);
        }

        if(!isDynamic() && isDynamic(len)){
            char* str = ptr;

            ptr = (char*) calloc(len + 1, sizeof(char));          //TODO Use = (int) instead of strcpy
            strncpy(ptr, str, len);
        }
    }

}

void AKStringKernel::update(const char* str, size_t copyLen, bool haveResize /*= true*/){
    if(haveResize && copyLen >= maxSize)
        resize(copyLen);

    nowSize = copyLen;
    strncpy(data(), str, copyLen);
}

//}
//=============================================================================

#if 0
//=============================================================================
//{  Shell for AKString

class AKString{
private:
    AKStringKernel krn;

public:
    AKString(const char* str);
    AKString(const char* str, size_t maxLen);
    explicit AKString(size_t len);
    AKString(const AKString& astr);
    AKString(const char* str, size_t copyLen, size_t maxLen);

    ~AKString();

    const char* c_str() const;

    const char* data() const;
    char* data();

    void add(const char* str);

    AKString substr(size_t begin, size_t end);

    size_t size() const;

    void clear();

    bool isEqual(const AKString& astr) const;
    bool isEqual(const char* str) const;

    void dump() const;

    char& charAt(size_t index);
    const char& charAt(size_t index) const;





    AKString& operator = (const char* str){
        krn.update(str);
        return *this;
    }
    AKString& operator = (const AKString& astr){
        krn.update(astr.c_str());
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


    char& operator [] (size_t index){
        return charAt(index);
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


AKString::AKString(const char* str):
    krn (str, strlen(str))
{}

AKString::AKString(const char* str, size_t bufLen):
    krn (str, bufLen)
{}

AKString::AKString(size_t bufLen):
    krn ("", bufLen)
{}

AKString::AKString(const AKString& astr):
    krn (astr.c_str(), astr.size())
{}


AKString::~AKString()
{}




//{
//void AKString::dump() const{
//    printf("\n AKString{ \n");
//    printf("\t ptr : \"%s\"  (ptr pointer is 0x%p) \n", ptr, &ptr);
//    printf("\t nowSize : %lu \n", (unsigned long) nowSize);
//    printf("\t maxSize : %lu \n", (unsigned long) maxSize);
//    printf("} \n\n");
//}


const char* AKString::data() const{
    return krn.data();
}

const char* AKString::c_str() const{
    return data();
}


void AKString::add(const char* str){
    krn.resize(size() + strlen(str));
    strcpy(data() + size(), str);
}

AKString AKString::substr(size_t begin, size_t end){
    std::string substr = c_str();
    return substr.substr(begin, end - begin).c_str();
}

size_t AKString::size() const{
    return krn.size();
}

void AKString::clear(){
    krn.update("", false);
}



bool AKString::isEqual(const AKString& astr) const{
    if(size() != astr.size()) return false;

    return (strcmp(c_str(), astr.c_str()) == 0);
}

bool AKString::isEqual(const char* str) const{
    if(size() != strlen(str)) return false;

    return (strcmp(c_str(), str) == 0);
}

//-----------------------------------------------------------------------------

const char& AKString::charAt(size_t index) const{
    if(index >= size())
        throw std::runtime_error("Invalid index for this AKString");

    return krn.data()[index];
}

//}
//=============================================================================
#endif

#endif
