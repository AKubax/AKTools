#ifndef AKString_FILE
#define AKString_FILE

//#include <TXLib.h>

#include <inttypes.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <exception>
#include <stdexcept>

int max(int a, int b){
    return a > b? a : b;
}


class AKStringKernel{
private:
    char* ptr;
    size_t nowSize;
    size_t maxSize;
public:
    AKStringKernel(const char* str);
    AKStringKernel(const char* str, size_t maxLen);
    explicit AKStringKernel(size_t len);
    AKStringKernel(const AKString& astr);
    AKStringKernel(const char* str, size_t copyLen, size_t maxLen);

    ~AKStringKernel();



    const char* c_str() const;

    void resize(size_t len);
    void update(const char* str, bool haveResize = true);
};

//-----------------------------------------------------------------------------

AKString::AKString(const char* str):
    ptr (strdup(str)),
    nowSize (strlen(ptr)),
    maxSize (nowSize)
{}

AKString::AKString(const char* str, size_t maxLen):
    ptr (NULL),
    nowSize (strlen(str)),
    maxSize (0)  //It will be defined
{
    resize( max(maxLen, nowSize) );
    strncpy(ptr, str, maxSize);
}

AKString::AKString(const char* str, size_t copyLen, size_t maxLen):
    ptr (NULL),
    nowSize (strlen(str)),
    maxSize (0)  //It will be defined
{
    resize( max(maxLen, nowSize) );
    strncpy(ptr, str, copyLen);
    ptr[copyLen] = '\0';
}

AKString::AKString(size_t maxLen):
    ptr (NULL),
    nowSize (0), //It's right
    maxSize (0)  //It will be defined in resize()
{
    resize(maxLen);
}

AKString::AKString(const AKString& astr):
    ptr (NULL),
    nowSize (0),    //It will be defined
    maxSize (0)     //It will be defined
{
    update(astr.ptr);
}

AKString::~AKString() {
    free(ptr);
    ptr = NULL;
}

const char* AKString::c_str() const{
    return ptr;
}

void AKString::resize(size_t len){
    bool isNeeded = false;

    if(len < maxSize * 0.5){
        maxSize = (int) (maxSize * 0.7 + 1);
        isNeeded = true;
    }

    if(len > maxSize){
        maxSize = (int) (len * 1.2 + 1);
        isNeeded = true;
    }


    if(isNeeded){
        char* res = (char*) ( realloc(ptr, maxSize * sizeof(char)) );

        if(res == NULL){
            throw std::runtime_error("realloc() doesn't have enough memory");
        }

        ptr = res;
        ptr[maxSize - 1] = '\0';
    }
}

void AKString::update(const char* str, bool haveResize /*= true*/){
    int len = strlen(str);

    if(haveResize || len >= maxSize)
        resize(len);

    strncpy(ptr, str, maxSize);
    nowSize = len;
}

//{============================================================================
//!
//}============================================================================

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
        update(str);
        return *this;
    }
    AKString& operator = (const AKString& astr){
        update(astr.ptr);
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
        return (strcmp(ptr, astr.ptr) > 0);
    }
    bool operator > (const char* str) const{
        return (strcmp(ptr, str) > 0);
    }

    bool operator < (const AKString& astr) const{
        return (strcmp(ptr, astr.ptr) < 0);
    }
    bool operator < (const char* str) const{
        return (strcmp(ptr, str) < 0);
    }

    bool operator >= (const AKString& astr) const{
        return (strcmp(ptr, astr.ptr) >= 0);
    }
    bool operator >= (const char* str) const{
        return (strcmp(ptr, str) >= 0);
    }

    bool operator <= (const AKString& astr) const{
        return (strcmp(ptr, astr.ptr) <= 0);
    }
    bool operator <= (const char* str) const{
        return (strcmp(ptr, str) <= 0);
    }
};









//-----------------------------------------------------------------------------

void AKString::dump() const{
    printf("\n AKString{ \n");
    printf("\t ptr : \"%s\"  (ptr pointer is 0x%p) \n", ptr, &ptr);
    printf("\t nowSize : %lu \n", (unsigned long) nowSize);
    printf("\t maxSize : %lu \n", (unsigned long) maxSize);
    printf("} \n\n");
}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

void AKString::add(const char* str){
    resize(nowSize + strlen(str));
    strncat(ptr, str, maxSize);
}

AKString AKString::substr(size_t begin, size_t end){
    return AKString(ptr + begin, end - begin + 1, 0);
}

size_t AKString::size() const{
    return nowSize;
}

void AKString::clear(){
    update("", false);
}



bool AKString::isEqual(const AKString& astr) const{
    if(astr.nowSize != nowSize){
        return false;
    }
    return (strcmp(astr.ptr, ptr) == 0);
}

bool AKString::isEqual(const char* str) const{
    return (strcmp(str, ptr) == 0);
}

//-----------------------------------------------------------------------------

char& AKString::charAt(size_t index){
    if(index >= nowSize)
        throw std::runtime_error("Invalid index for this AKString");

    return ptr[index];
}

const char& AKString::charAt(size_t index) const{
    if(index >= nowSize)
        throw std::runtime_error("Invalid index for this AKString");

    return ptr[index];
}



#endif