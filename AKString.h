#ifndef AKString_FILE
#define AKString_FILE

#include <TXLib.h>
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wnonnull-compare"

#include <inttypes.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <exception>
#include <stdexcept>

#define throw(text) {$se printf(#text"\n");}

#include "AKDump.h"


int max(int a, int b);
int max(int a, int b){
    return a > b? a : b;
}

//{ PROTOTYPES

const char* find(const char* begin, const char* end, char c);

const char* find(const char* text, char letter);

const char* find(const char* text, size_t len, char letter);

const char* r_find(const char* begin, const char* end, char c);

const char* r_find(const char* text, char letter);

const char* r_find(const char* text, size_t len, char letter);

const char* stupid_search(const char* t_begin, const char* t_end, const char* w_begin, const char* w_end);

const char* boyer_moore_horspool_search(const char* t_begin, const char* t_end, const char* w_begin, const char* w_end);

const char* rabin_carp_search(const char* t_begin, const char* t_end, const char* w_begin, const char* w_end);

const char* knuth_morris_pratt_search(const char* text, const char* word);

const char* search(const char* text, const char* word, int type = 0);

size_t count(const char* text, char letter);

//}

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
$1    size_t prevSize = size();
$    resize(size() + len);
//$    dump();
//$    printf("ptr = \"%s\" [%p]; &ptr = \"%s\" [%p]; data() = \"%s\"; prevSize = %zu; ptr + prevSize = %s [%p]; str = %s [%p]; len + 1 = %zu\n",
//            ptr, ptr, (char*) &ptr, &ptr, data(), prevSize, ptr + prevSize, ptr + prevSize, str, str, len + 1);
$    strncpy(data() + prevSize, str, len + 1);
}

void AKStringKernel::resize(size_t len, bool smart /*= true*/){        //It cares about zero symbol
$1    bool isNeeded = false;
$    len++;
$
$    bool isPrevDynamic = isDynamic(maxSize);
$    bool isNextDynamic = isDynamic(len);
$
$    size_t nextSize = -1u;
$
$    if(smart){
$        if(isNextDynamic){
$            if(len < (size_t) (maxSize * 0.7)){
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

$    if(isNeeded){
$        PRINTD(5, "isDynamic() returns %d, isDynamic(len) returns %d, len is %lu, nextSize is %lu \n\n", isDynamic(), isDynamic(len), (unsigned long) len, (unsigned long) nextSize);
$
$        if(isPrevDynamic && isNextDynamic){
$            PRINTD(5, "Case: Dynamic -> Dynamic");
$
$            PRINTD(5, "nowLen: %zu, nextSize: %zu\n", nowLen, nextSize);
$
$            char* res = (char*) ( realloc(ptr, nextSize * sizeof(char)) );
$            memset(res + len, '\0', (nextSize - len) * sizeof(char));
$
$            if(res == NULL){
$                throw(std::runtime_error("realloc() doesn't have enough memory"));
$            }
$
$            ptr = res;
$            ptr[nextSize - 1] = '\0';
$
$            maxSize = nextSize;
$        }
$
$        if(isPrevDynamic && !isNextDynamic){
$            PRINTD(5, "Case: Dynamic -> Static");
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
$            PRINTD(5, "Case: Static -> Dynamic\n");
$            PRINTD(5, "nextSize: %zu \n", nextSize);
$            DUMP(7, *this, "Before copy");
$
$            uintptr_t str = (uintptr_t) ptr;
$            PRINTD(5, "str is 0x%p \"%s\" \n", (const char*) str, (const char*) &str);
$
$            ptr = (char*) calloc(nextSize, sizeof(char));        //TODO Use = (int) instead of strcpy
$            strncpy(ptr, (const char*) &str, len);
$
$            maxSize = nextSize;
$
$            DUMP(7, *this, "After copy");
$        }
$
$        if(!isPrevDynamic && !isNextDynamic){
$            PRINTD(5, "Case: Static -> Static");
$
$            maxSize = sizeof(ptr);
$        }
$    }

     nowLen = --len;
}

void AKStringKernel::update(const char* str, size_t copyLen, bool haveResize /*= true*/){
$1   PRINTD(5, "PARAMETERS| str: \"%s\"    copyLen: %lu\n", str, (long unsigned int) copyLen);
$    size_t strLen = strlen(str);
$
     PRINTD(5, "str: \"%s\"; copyLen: %zu\n", str, copyLen);

$    if(copyLen > strLen)  copyLen = strLen;
$
$    if(haveResize){
$        resize(copyLen);
$    }

     DUMP(7, *this, "AFTER RESIZE");
$
$    nowLen = copyLen;
     DUMP(7, *this, "AFTER NOWLEN = COPYLEN");
$    strncpy(data(), str, copyLen);
$    *(data() + copyLen) = '\0';

     DUMP(7, *this, "AFTER STRNCPY");
}

bool AKStringKernel::ok() const{
$1    bool isOK = true;

$    if(!this) return false;

$    if(nowLen != strlen(data())) isOK = false;

$    return isOK;
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
    const char* toString() const;

    AKString substr(size_t begin, size_t end) const;

    void dump(const char* str = "") const;

    size_t size() const;

    bool isEqual(const AKString& astr) const;
    bool isEqual(const char* str) const;

    bool ok() const;

    const char* search(const char* word, int type = 0) const;
    const char* find(char letter) const;

    //-----------------------------------------------------------------------------
    void add(const char* str);
    void add(const AKString& astr);

    const char& charAt(size_t index) const;

    void clear();

    void replace(int first_, int second_, const char* str);

    void change(int first_, int second, const char* str);

    ptrdiff_t count(char c);
    //=============================================================================
    AKString& operator = (const char* str){
    $1    krn.update(str, strlen(str));
    $    return *this;
    }
    AKString& operator = (const AKString& astr){
    $1    PRINTD(5, "astr.c_str(): \"%s\"\n", astr.c_str());
    $    krn.update(astr.c_str(), astr.size());
    $    return *this;
    }

    AKString& operator += (const AKString& astr){
    $1    add(astr.c_str());
    $    return *this;
    }
    AKString& operator += (const char* str){
    $1    add(str);
    $    return *this;
    }


    AKString operator + (const char* str) const{
    $1    return AKString(*this) += str;
    }
    AKString operator + (const AKString& astr) const{
    $1    return AKString(*this) += astr;
    }


    const char& operator [] (size_t index) const{
    $1    return charAt(index);
    }


    bool operator == (const AKString& astr) const{
    $1    return isEqual(astr);
    }
    bool operator == (const char* str) const{
    $1    return isEqual(str);
    }

    bool operator > (const AKString& astr) const{
    $1    return (strcmp(c_str(), astr.c_str()) > 0);
    }
    bool operator > (const char* str) const{
    $1    return (strcmp(c_str(), str) > 0);
    }

    bool operator < (const AKString& astr) const{
    $1    return (strcmp(c_str(), astr.c_str()) < 0);
    }
    bool operator < (const char* str) const{
    $1    return (strcmp(c_str(), str) < 0);
    }

    bool operator >= (const AKString& astr) const{
    $1    return (strcmp(c_str(), astr.c_str()) >= 0);
    }
    bool operator >= (const char* str) const{
    $1    return (strcmp(c_str(), str) >= 0);
    }

    bool operator <= (const AKString& astr) const{
    $1    return (strcmp(c_str(), astr.c_str()) <= 0);
    }
    bool operator <= (const char* str) const{
    $1    return (strcmp(c_str(), str) <= 0);
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

$1    return krn.data();

}

const char* AKString::toString() const{
    $1 return data();
}

AKString AKString::substr(size_t begin, size_t end) const{

$1    PRINTD(5, "Beginning of substr() func\n");

$    if(begin > end) throw(std::runtime_error("First position can't be further then last"));
$    if(end >= size()) throw(std::runtime_error("Out of string's bounds"));

$    PRINTD(5, "begin: %zu; end: %zu\n", begin, end);

$    AKString as(c_str() + begin, end - begin);

$    PRINTD(5, "Ending of substr() func\n");

$    return as;

}

void AKString::dump(const char* str) const{

$1    $h printf("Dump is called from AKString (Shell) \n"
                "Dump for AKStringKernel: \n"); $d

$    krn.dump(str);
}

size_t AKString::size() const{

$1    return krn.size();

}

bool AKString::isEqual(const AKString& astr) const{

$1    if(size() != astr.size()) return false;

$    return (strcmp(c_str(), astr.c_str()) == 0);
}

bool AKString::isEqual(const char* str) const{

$1    if(size() != strlen(str)) return false;

$    return (strcmp(c_str(), str) == 0);
}

bool AKString::ok() const{

$1    return krn.ok();

}

const char* AKString::search(const char* word, int type/* = 0*/) const{
    return ::search(data(), word, type);
}

const char* AKString::find(char letter) const{
    return ::find(data(), letter);
}
//-----------------------------------------------------------------------------
void AKString::add(const char* str){

$1    krn.add(str, strlen(str));

}
void AKString::add(const AKString& astr){

$1    krn.add(astr.c_str(), astr.size());

}

const char& AKString::charAt(size_t index) const{

$1    if(index >= size()){
$        throw(std::runtime_error("Invalid index for this AKString"));
$    }

$    return krn.data()[index];
}

void AKString::clear(){

$1    krn.update("", false);

}

void AKString::replace(int first_, int second_, const char* str){

$1   if(!ok())                              throw(std::runtime_error("Something is wrong with object"));

$    if( !(               0 <= first_)  )   throw(std::runtime_error("Out of string bounds (first arg)"));
$    if( !(          first_ <= second_) )   throw(std::runtime_error("Second arg can't be less than first"));
$    if( !((size_t) second_ <= size())  )   throw(std::runtime_error("Out of string bounds (second arg)"));

$    size_t first =  (size_t) first_;
$    size_t second = (size_t) second_;

$    if(str == NULL)                        throw(std::runtime_error("String argument can't be NULL"));

$    if(strlen(str) != second - first)      throw(std::runtime_error("String to be placed instead of another doesn't has the right length"));

$    strncpy(const_cast<char*>(data() + first), str, second - first);
}

void AKString::change(int first_, int second_, const char* str){

$1   if(!ok())                              throw(std::runtime_error("Something is wrong with object"));

$    if( !(               0 <= first_)  )   throw(std::runtime_error("Out of string bounds (first arg)"));
$    if( !(          first_ <= second_) )   throw(std::runtime_error("Second arg can't be less than first"));
$    if( !((size_t) second_ <= size())  )   throw(std::runtime_error("Out of string bounds (second arg)"));

$    size_t first =  (size_t) first_;
$    size_t second = (size_t) second_;

$    if(str == NULL)                        throw(std::runtime_error("String argument can't be NULL"));

$    AKString as(c_str(), first);
$    as += str;
$    as += (c_str() + second);

$    krn.update(as.c_str(), as.size());
}

//}
//=============================================================================

const char* find(const char* begin, const char* end, char c){

$1   if(begin == NULL) throw(std::runtime_error("No argument can be NULL (-_-)"));
$    if(end   == NULL) throw(std::runtime_error("No argument can be NULL (-_-)"));

$    if(begin > end) throw(std::runtime_error("Incorrect begin"));

$    while(begin != end){
$        if(*begin == c) return begin;
$        begin++;
$    }

$    return NULL;
}

const char* find(const char* text, char letter){

$1   return find(text, text + strlen(text), letter);

}

const char* find(const char* text, size_t len, char letter){

$1   return find(text, text + len, letter);
}

const char* r_find(const char* begin, const char* end, char c){

$1   if(begin == NULL) throw(std::runtime_error("No argument can be NULL (-_-)"));
$    if(end   == NULL) throw(std::runtime_error("No argument can be NULL (-_-)"));

$    if(begin > end) throw(std::runtime_error("Incorrect begin"));

$    while(end != begin){
$        if(*end == c) return end;
$        end--;
$    }

$    return NULL;

}

const char* r_find(const char* text, char letter){

$1   return r_find(text, text + strlen(text), letter);

}

const char* r_find(const char* text, size_t len, char letter){

$1   return r_find(text, text + len, letter);
}

//-----------------------------------------------------------------------------

const char* stupid_search(const char* t_begin, const char* t_end, const char* w_begin, const char* w_end){

$1   if(t_begin == NULL) throw(std::runtime_error("Argument 1 can't be NULL (-_-)"));
$    if(t_end   == NULL) throw(std::runtime_error("Argument 2 can't be NULL (-_-)"));
$    if(w_begin == NULL) throw(std::runtime_error("Argument 3 can't be NULL (-_-)"));
$    if(w_end   == NULL) throw(std::runtime_error("Argument 4 can't be NULL (-_-)"));

$    if(t_begin > t_end) throw(std::runtime_error("Incorrect argument 1"));
$    if(w_begin > w_end) throw(std::runtime_error("Incorrect argument 3"));

$    size_t w_len = (size_t) (w_end - w_begin);

$    if(w_len > (size_t) (t_end - t_begin)) throw(std::runtime_error("The length of substring oughtn't be more than string's length"));

$    while(t_begin != t_end - w_len + 1){
$        if(strncmp(t_begin, w_begin, w_len) == 0) return t_begin;
$        t_begin++;
     }

$    return NULL;
}

const char* boyer_moore_horspool_search(const char* t_begin, const char* t_end, const char* w_begin, const char* w_end){
$1   if(t_begin == NULL) throw(std::runtime_error("Argument 1 can't be NULL (-_-)"));
$    if(t_end   == NULL) throw(std::runtime_error("Argument 2 can't be NULL (-_-)"));
$    if(w_begin == NULL) throw(std::runtime_error("Argument 3 can't be NULL (-_-)"));
$    if(w_end   == NULL) throw(std::runtime_error("Argument 4 can't be NULL (-_-)"));

$    if(t_begin > t_end) throw(std::runtime_error("Incorrect argument 1"));
$    if(w_begin > w_end) throw(std::runtime_error("Incorrect argument 3"));

    const size_t w_len = w_end - w_begin;

    //{ Extraordinary cases
    PRINTD(5, "BEFORE IF #1. t_begin == \"%s\" ; w_begin == \"%s\"\n", t_begin, w_begin);
    if(strcmp(t_begin, "") == 0){
        PRINTD(5, "IF #1 WORKED\n");
        if(strcmp(w_begin, "") == 0){
            PRINTD(5, "IF #2 WORKED\n");
            return t_begin;
        }
        return NULL;
    }
    //}

    size_t arr[256];

    for(int i = 0; i < 256; i++){
        arr[i] = w_len;
    }

    for(const char* w = w_begin; w < w_end; w++){
//        assert(0 <= *w && *w < 256);
        arr[(int) *w] = w_end - w - 1;
    }

//    $g
//    for(char c = 'a'; c <= 'z'; c++){
//        PRINTD("%c", c);
//    }
//    PRINTD("\n");
//
//    for(int i = 'a'; i <= 'z'; i++){
//        PRINTD("%d", arr[i]);
//    }
//    PRINTD("\n");

//    $b PRINTD("\n\n01234567890123456789012345678901234567890123456789\n"); $d
//    PRINTD("%s\n", t_begin);

    for(const char* cur = t_begin; cur <= t_end - w_len + 1; cur++){
//        PRINTD("%*s%s {cur = \"%s\" ; cur - t_begin = %2d ; \n", cur - t_begin, "", w_begin, cur, cur - t_begin);

        size_t i = 0;
        for(i = 1; i <= w_len; i++){
            if(*(cur + w_len - i) != *(w_end - i)) break;
            if(i == w_len) return cur;
        }
        cur += arr[(int) (*(cur + w_len - i))] - 1;

//        PRINTD("c = \"%7s\" ; w_len = %2d ; w_end - c = %2d}\n", c, w_len, w_end - c);
    }

    return NULL;
}

const char* rabin_carp_search(const char* t_begin, const char* t_end, const char* w_begin, const char* w_end){
$1   if(t_begin == NULL) throw(std::runtime_error("Argument 1 can't be NULL (-_-)"));
$    if(t_end   == NULL) throw(std::runtime_error("Argument 2 can't be NULL (-_-)"));
$    if(w_begin == NULL) throw(std::runtime_error("Argument 3 can't be NULL (-_-)"));
$    if(w_end   == NULL) throw(std::runtime_error("Argument 4 can't be NULL (-_-)"));

$    if(t_begin > t_end) throw(std::runtime_error("Incorrect argument 1"));
$    if(w_begin > w_end) throw(std::runtime_error("Incorrect argument 3"));

    size_t w_sum = 0;
    const size_t w_len = w_end - w_begin;



    //{ Extraordinary cases
    PRINTD(5, "BEFORE IF #1\n");
    if(strcmp(t_begin, "") == 0){
        PRINTD(5, "IF #1 WORKED\n");
        if(strcmp(w_begin, "") == 0){
            PRINTD(5, "IF #2 WORKED\n");
            return t_begin;
        }
        return NULL;
    }
    //}

    for(size_t i = 0; i < w_len; i++){
        w_sum += w_begin[i];
    }

    size_t st_sum = 0;
    for(size_t i = 0; i < w_len; i++){
        st_sum += t_begin[i];
    }

    for(const char* cur = t_begin; cur <= (t_end - w_len); cur++){
        if(st_sum == w_sum){
            if(strncmp(cur, w_begin, w_len) == 0) return cur;
        }

        st_sum += -(*cur) + *(cur + w_len);
    }

    return NULL;
}

const char* knuth_morris_pratt_search(const char* text, const char* word){
    const ptrdiff_t t_len = strlen(text);
    const ptrdiff_t w_len = strlen(word);

    if(w_len > t_len)   return NULL;
    if(w_len == t_len)  return strcmp(text, word) == 0? text : NULL;
    if(w_len == 1)      return find(text, *word);
    if(!word[0])        return text;

    ptrdiff_t* z = new ptrdiff_t[w_len];
    memset(z, -1, w_len * sizeof(z[0]));
    z[0] = 0;

    for(ptrdiff_t i = 1; i < w_len; i++){
        assert(0 <= i     &&   i     < w_len);
        assert(0 <= i - 1 &&   i - 1 < w_len);
        z[i] = z[i - 1];
        PRINTD(5, "Initializing new z() -   z[i] = z[i - 1];  z[i - 1] = %zd;  z[i] = %zd\n\n\n", z[i - 1], z[i]);


        PRINTD(5, "Checking if letters are the same - (word[z[i] (%zd)] == word[i (%zd)]) == %s\n\n\n", z[i], i, word[z[i]] == word[i]? "true" : "false");
        assert(0 <= z[i] && z[i] < w_len);
        if(word[z[i]] == word[i]){
            z[i]++;
            PRINTD(5, "Letters are the same -   z[i]++; z[i (%zd)] = %zd\n\n\n", i, z[i]);
         }

        else{
            PRINTD(5, "Letters are not the same\n\n\n");
            while(true){
                PRINTD(5, "i = %zd; z[i] = %zd\n", i, z[i]);
                assert(0 <= z[i] && z[i] < w_len);
                ptrdiff_t zi1 = z[i] - 1 >= 0? z[i] - 1 : 0;
                PRINTD(5, "zi1 == %zd ; w_len == %zd\n", zi1, w_len);
                assert(0 <= zi1 && zi1 < w_len);
                z[i] = z[zi1];
                PRINTD(5, "z[i] = z[z[i] (%zd)] (%zd)\n\n\n", z[i], z[z[i]]);

                PRINTD(5, "Checking if letters are the same - (word[z[i] (%zd)] == word[i (%zd)]) == %s\n\n\n", z[i], i, word[z[i]] == word[i]? "true" : "false");
                if(word[z[i]] == word[i]){
                    z[i]++;
                    PRINTD(5, "Letters are the same -   z[i]++; z[i] = %zd\n\n\n", z[i]);
                    break;
                }
                PRINTD(5, "NOPE ^/\\^  no matching letters\n\n\n");
                if(z[i] == 0) break;
            }
        }

    }

    PRINTD(5, "%s\n", word);
    for(ptrdiff_t i = 0; i < w_len; i++){
        PRINTD(5, "%zd", z[i]);
    }
    PRINTD(5, "\n");


    ptrdiff_t cur = 0;

    for(ptrdiff_t i = 0; i < t_len; i++){
        PRINTD(5, "Checking if letters are the same - (word[cur (%zd)] == text[i (%zd)]) == %s\n\n\n", cur, i, word[cur] == text[i]? "true" : "false");
        if(word[cur] == text[i]){
            cur++;
            PRINTD(5, "Letters are the same -   cur++; z[i (%zd)] = %zd\n\n\n", i, cur);
        }
        else{
            PRINTD(5, "Letters are not the same\n\n\n");
            while(true){
                ptrdiff_t cur_m = (cur - 1 >= 0)? cur - 1 : 0;
                PRINTD(5, "cur = %zd; cur_m = %zd\n", cur, cur_m);
                assert(0 <= (cur - 1 >= 0? cur - 1 : 0) && (cur - 1 >= 0? cur - 1 : 0) < t_len);
                cur = z[cur - 1 >= 0? cur - 1 : 0];
//                PRINTD(5, "cur = z[cur (%d)] (%d)\n\n\n", cur, z[cur]);

                PRINTD(5, "Checking if letters are the same - (word[cur (%zd)] == text[i (%zd)]) == %s\n\n\n", cur, i, word[cur] == text[i]? "true" : "false");
                if(word[cur] == text[i]){
                    cur++;
                    PRINTD(5, "Letters are the same -   cur++; cur = %zd\n\n\n", cur);
                    break;
                }
                PRINTD(5, "NOPE ^/\\^  no matching letters\n\n\n");
                if(cur == 0) break;
            }
        }



        PRINTD(5, "cur (%zd) == w_len (%zd)\n\n\n", cur, w_len);
        if(cur == w_len){
            PRINTD(5, "FOUND!11!! %zd\n", i - w_len + 1);
            return &(text[i]) - w_len + 1;
    }
}
    PRINTD(5, "NUUUUUUUULL\n");
    delete [] z;
    return NULL;
}


const char* search(const char* text, const char* word, int type/* = 0*/){
    switch(type){
        case 0: return boyer_moore_horspool_search(text, text + strlen(text), word, word + strlen(word));
        case 1: return rabin_carp_search(text, text + strlen(text), word, word + strlen(word));
        case 2: return knuth_morris_pratt_search(text, word);
        case 3: return stupid_search(text, text + strlen(text), word, word + strlen(word));

        default: throw(std::runtime_error("Wrong arg 3 (int type)"));
    }
}

size_t count(const char* text, char letter){
$1
$    size_t counter = 0;

$    text = find(text, letter);

$    while(text != NULL){
$        counter++;
$        text = find(text + 1, letter);
$        PRINTD(5,"TEXT IN LOOP IS \"%s\" \n", text);
$    }

$    return counter;
}


#endif
