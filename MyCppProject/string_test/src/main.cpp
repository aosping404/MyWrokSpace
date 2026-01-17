#include <iostream>
#include <vector>
#include <string>
#include "main.h"

using namespace std;

bool is_prime(int n){
    int i{};
    for (i = 2; i <= n - 1; i++) {
        if (n % i == 0) {
        break;
    }
    }
    if (i >= n) {
        return true;
    } else {
        return false;
    }
}

int main()
{
    vector<int> prime; //向量容器，动态数组
    for(int i=2;i<10000;i++){
        if(is_prime(i)){
            prime.push_back(i);
        }
    }
    for(int &item:prime){
        cout<<item<<",";
    }
    return 0;
}
