#include <iostream>
#include <vector>
#include <string>
#include "main.h"

using namespace std;
class Point
{
public:
    void InitP(float xx=0,float yy = 0){
        X=xx;
        Y=yy;
        cout<<"X:"<<X<<"Y:"<<Y<<endl;
    }
    void Move(float xOff,float yOff) {
        X += xOff;
        Y += yOff;
    }
    float GetX(){
        return X;
    }
    float GetY(){
        return Y;
    }
private:
    float X,Y;
};
class Rectangle : public Point //派生声明
{
public:
    void InitR(float x,float y, float w,float h){
        InitP(x,y);
        W = w; H = h;
    }
    float GetH(){
        return H;
    }
    float GetW(){
        return W;
    }
private:
    float W,H;
};
class Parent
{
public:
    Parent(int i = 0)
    {
        member = i;
    }
    void method(){
        cout << "member = "<<member << endl;
    }
private:
    int member;
};
class Child : public Parent{
public:
    Child(int i = 0,int j=0) :Parent(i)
    {
        childMember = j;
    }
    void childMethod(){
        method();
        cout<<"childMember = "<<childMember<<endl; 
    }
private:
    int childMember;
};
int main()
{
    Child child(1,2);
    child.method();
    child.childMethod();
    return 0;
}
