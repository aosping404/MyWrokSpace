#include "myCout.h"

myCout& myCout::operator<<(const char* str){
    printf("%s", str);
    return *this;
}



