#include <iostream>
#include <vector>
#include <string>
#include "main.h"

#define log(x) std::cout << x << " "

class Entity
{
public:
    float X, Y;

    Entity(){
        X = 0.0f;
        Y = 0.0f;
        log("Created Entity!"<<std::endl);
    }
    ~Entity(){
        log("Destroyed Entity!"<<std::endl);
    }
    void Print(){
        log(X<<","<<Y<<std::endl);
    }
};
void Function() {
    Entity e;
    e.Print();
}
int main()
{
    Function();
    return 0;
}
