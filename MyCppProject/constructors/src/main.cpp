#include <iostream>
#include <vector>
#include <string>
#include "main.h"

class Entity{
public:    
    float X,Y;
    void Print(){
        std::cout<<X<<","<<Y<<std::endl;
    }
};

int main()
{
    Entity e{};
    e.Print();
    return 0;
}
