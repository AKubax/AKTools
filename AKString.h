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
    AKStringKernel(const char* str);
    AKStringKernel(const char* str, size_t maxLen);
    explicit AKStringKernel(size_t len);
    AKStringKernel(const char* str, size_t copyLen, size_t maxLen);

    ~AKStringKernel();

    const char* data() const;
    char* data();

    size_t size() const;

    void resize(size_t len);
    void update(const char* str, bool haveResize = true);
};

//=============================================================================

    AKStringKernel::AKStringKernel(const char* str):
        ptr (strdup(str)),
        nowSize (strlen(ptr)),
        maxSize (nowSize)
    {}

    AKStringKernel::AKStringKernel(const char* str, size_t maxLen):
        ptr (NULL),
        nowSize (strlen(str)),
        maxSize (0)  //It will be defined
    {
        resize( max(maxLen, nowSize) );
        strncpy(ptr, str, maxSize);
    }

    AKStringKernel::AKStringKernel(const char* str, size_t copyLen, size_t maxLen):
        ptr (NULL),
        nowSize (strlen(str)),
        maxSize (0)  //It will be defined
    {
        resize( max(maxLen, nowSize) );
        strncpy(ptr, str, copyLen);
        ptr[copyLen] = '\0';
    }

    AKStringKernel::AKStringKernel(size_t maxLen):
        ptr (NULL),
        nowSize (0), //It's right
        maxSize (0)  //It will be defined in resize()
    {
        resize(maxLen);
    }

    AKStringKernel::~AKStringKernel() {
        free(ptr);
        ptr = NULL;
    }



    bool AKStringKernel::isDynamic(size_t len) const{
        return true; //len * sizeof(char) > sizeof(ptr);                     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    bool AKStringKernel::isDynamic() const{
        return isDynamic(maxSize);
    }


    const char* AKStringKernel::data() const{
        if(isDynamic()) return ptr;

        else   return (char*) &ptr;
    }

    char* AKStringKernel::data(){
        return (char*) ((const AKStringKernel*) (this))->data();
    }

    size_t AKStringKernel::size() const{
        return nowSize;
    }


    void AKStringKernel::resize(size_t len){
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
            if(isDynamic() && isDynamic(len)){
                char* res = (char*) ( realloc(ptr, maxSize * sizeof(char)) );

                if(res == NULL){
                    throw std::runtime_error("realloc() doesn't have enough memory");                 //TODO 4 cases
                }

                ptr = res;
                ptr[maxSize - 1] = '\0';
            }
            //else if(true);                       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
    }

    void AKStringKernel::update(const char* str, bool haveResize /*= true*/){
        int len = strlen(str);

        if(haveResize || len >= maxSize)
            resize(len);

        strncpy(ptr, str, maxSize);
        nowSize = len;
    }

//}
//=============================================================================


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
    krn (AKStringKernel(str))
{}

AKString::AKString(const char* str, size_t maxLen):
    krn (AKStringKernel(str, maxLen))
{}

AKString::AKString(size_t len):
    krn (AKStringKernel(len))
{}

AKString::AKString(const char* str, size_t copyLen, size_t maxLen):
    krn (AKStringKernel(str, copyLen, maxLen))
{}







//{
//void AKString::dump() const{
//    printf("\n AKString{ \n");
//    printf("\t ptr : \"%s\"  (ptr pointer is 0x%p) \n", ptr, &ptr);
//    printf("\t nowSize : %lu \n", (unsigned long) nowSize);
//    printf("\t maxSize : %lu \n", (unsigned long) maxSize);                  //TODO Come up how to do it with kernel
//    printf("} \n\n");
//}


const char* AKString::data() const{
    return krn.data();
}

char* AKString::data(){
    krn.data();
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

char& AKString::charAt(size_t index){
    if(index >= size())
        throw std::runtime_error("Invalid index for this AKString");

    return krn.data()[index];
}

const char& AKString::charAt(size_t index) const{
    if(index >= size())
        throw std::runtime_error("Invalid index for this AKString");

    return krn.data()[index];
}
//}
//=============================================================================


#endif
