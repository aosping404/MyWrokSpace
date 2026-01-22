#include <iostream>
#include <vector>
#include <string>
#include "main.h"

using namespace std;



int main()
{
    double a = 1;
    double b = 2;
    double x=(a+b)/2;
    double y=x*x-2;
    int count = 0;
    while(y>0.0000001||y<-0.0000001){
        count ++;
        if(y>0){
            b=x;
        }
        else {
            a=x;
        }
        x=(a+b)/2;
        y=x*x-2;
        cout<<x<<endl;
    }
    return 0;
}
