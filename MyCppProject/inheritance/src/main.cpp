#include <iostream>
#include <vector>
#include <string>
#include "main.h"

class Entity
{
public:
    float X,Y;
    void Move(float xa,float ya){
        X += xa;
        Y += ya;
    }
    Entity(){
        std::cout<<"Entity构造函数执行"<<std::endl;
    }
    ~Entity(){
        std::cout<<"Entity析构函数执行"<<std::endl;
    }
};

class Player : public Entity   
//protected 保护继承 在保护继承中，
//!在保护继承中，基类的public和protected成员在派生类中仍然是protected，而private成员依然是private。
//private 私有继承
//!在私有继承中，基类的public和protected成员在派生类中变为private。
//一般使用 public 公有继承
{
public:
    const char* Name;
    void PrintName(){
        std::cout<<Name<<std::endl;
    }
    Player(){
        std::cout<<"Player构造函数执行"<<std::endl;
    }
    ~Player(){
        std::cout<<"Player析构函数执行"<<std::endl;
    }
};
int main()
{
    Player player;
    player.Move(5,5);
    player.X = 2;
    player.Name = "Tom";
    player.PrintName();
    return 0;
}
