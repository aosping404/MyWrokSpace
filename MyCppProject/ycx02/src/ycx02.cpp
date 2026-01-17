#include <iostream>
#include <vector>
#include <ctime>
#include <cstring>
#define LOG(x) std::cout << x << " "
#define INPUT(x) std::cin >> x

struct country{
    char name[20];
    double population;
    double area;
} countrys[5] = {
    {"中国",14.25,936},
    {"美国",3.4,996},
    {"俄罗斯",1.4,1709},
    {"印度",14.26,298},
    {"印尼",2.8,191},
};
int countryCount = 5;
struct countLetter {
    char letter;
    int count;
}; 
countLetter str[1000];    
int letterCount{0};

int findletter(char c){
    for(int i{};i<letterCount;i++){
        if(str[i].letter==c) return i;
    }
    return -1;
}

int main() {
#if 0
    char source_str[1000];
    INPUT(source_str);
    int len = strlen(source_str);
    for(int i{};i<len;i++){
        int index = findletter(source_str[i]);
        if(index==-1) {
            str[letterCount].count=1;
            str[letterCount].letter=source_str[i];
            letterCount++;
        }
        else{
            str[index].count++;
        }
    }
    LOG("\n");
    for(int i{};i<letterCount;i++){
        if(str[i].count>=2) 
            LOG("字母"<<str[i].letter<<"有"<<str[i].count<<"个"<<"\n");
    }
#endif
#if 0
    LOG("----------------------------\n");
    LOG("国家\t\t人口\t面积\n");
    LOG("---------------------------\n");

    for(int i{};i<countryCount;i++){
        LOG(countrys[i].name<<"\t\t"<<countrys[i].population<<"\t"<<countrys[i].area);
        LOG("\n");
    }
#endif
    char str1[100];
    char str2[100];
    INPUT(str1);
    INPUT(str2);
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if(len2>len1){
        LOG("输入字符串格式有误");
        return 0;
    }
    for(int i{};i<=len1-len2;++i){
        int index = 0;
        while(index<len2 && str1[index+i]==str2[index]){
            ++index;
        }
        if(index==len2){
            LOG(i);
            return 0;
        }
    }
    std::string str3{"asdlakndlansd"};
    size_t post1 = str3.find("ansd");
    LOG("没有匹配字符串");
    return 0;
/*
    //优化算法
    //复杂度O（n+m）
    std::vector<int> buildNext(const std::string& p) {
    int m = p.size(), j = -1;
    std::vector<int> ne(m, -1);
    for (int i = 1; i < m; ++i) {
        while (j >= 0 && p[i] != p[j + 1]) j = ne[j];
        if (p[i] == p[j + 1]) ++j;
        ne[i] = j;
    }
    return ne;
}

int kmp(const std::string& s, const std::string& p) {
    int n = s.size(), m = p.size(), j = -1;
    auto ne = buildNext(p);
    for (int i = 0; i < n; ++i) {
        while (j >= 0 && s[i] != p[j + 1]) j = ne[j];
        if (s[i] == p[j + 1]) ++j;
        if (j == m - 1) return i - m + 1;   // 找到
    }
    return -1;
}
*/
}