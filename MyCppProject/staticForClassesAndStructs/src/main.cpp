#include <iostream>
#include <vector>
#include <string>
#include "main.h"

#define LOG(x) std::cout<<x<<std::endl;
struct Entity{
    static int x,y;
};
int Entity::x;
int Entity::y;  //做一个链接
int main()
{
    Entity e;
    e.x = 2;
    e.y = 3;
    Entity e1;
    e1.x = 5;
    e1.y = 8;
    //上面实际上修改的是位于统一命名空间的变量
    //你应该使用Entity::x或者Entity::y
    LOG(e.x);
    LOG(e.y);
    LOG(e1.x);
    LOG(e1.y);
    return 0;
}
