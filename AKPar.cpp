 #include "../TXLib.h"

#include <stack>
#include <utility>
#include <exception>
#include <stdexcept>
#include <stdio.h>

using namespace std;

struct Triple
    {
    char first;
    size_t second;
    size_t third;

    Triple (char f, size_t s, size_t t) :
        first  (f),
        second (s),
        third  (t)
        {}

    };

int main()
    {
    txDisableAutoPause();
//    $g printf("If you ended the input press CTRL+Z\n\n"); $d

    stack<Triple> st;

    bool ignore = false;

    for (size_t line = 1, symbol = 1;; symbol++)
        {
        char c = getchar();
        if (c == EOF) break;

        if(c == '"') ignore = !ignore;

        if(ignore) continue;

        if (c == '\n')
            {
            line++;
            symbol = 1;
            }

//        printf("The char is %c\n", c);

        char prev = 0;

        switch (c)
            {
            case '{': case '(': case '[':
                {
//                printf ("Pushing c = '%c'; line = %zu\n", c, line);
                st.push (Triple (c, line, symbol));
                break;
                }

            case '}': prev = '{'; break;
            case ')': prev = '('; break;
            case ']': prev = '['; break;
            }

        if (prev != 0)
            {
            if (st.size() == 0)
                {
                printf ("There is wrong parenthesis '%c' [%zu] on line %zu\n",
                         c, symbol, line);
                return 1;
                }


            Triple t = st.top();
//            printf("Just topped pair. first = '%c', second = %zu\n", p.first, p.second);
            st.pop();

            if (t.first != prev)
                {
                printf ("There is wrong parenthesis '%c' [%zu] on line %zu\n"
                        "There must be a closing parenthesis for '%c'\n",
                         c, symbol, line, t.first);
                return 1;
                }
            }

        }

    if (st.size() != 0)
        {
        Triple t = st.top();
        st.pop();

        printf( "There is not closed parenthesis '%c' [%zu] on line %zu\n"
                "Fix it and try it out again\n\n",
                 t.first, t.third, t.second);
        return 1;
        }

    $g printf("Everything correct!"); $d
    }


