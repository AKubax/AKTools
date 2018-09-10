

struct Token{
    union{

        const char* str = 0;
        double      dbl = 0;

    } value;


};
