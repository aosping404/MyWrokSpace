#include <iostream>
#include <vector>
#include <ctime>
#include <cstring>
#define LOG(x) std::cout << x << " "
#define INPUT(x) std::cin >> x

struct Student
{
    void getInfo(){
        //内部获取
        LOG("\n");
        LOG("内部获取"<<"\n");
        LOG("name: "<<name<<"\n");
        LOG("age: "<<age<<"\n");
        LOG("index: "<<index<<"\n");
    }
    int age;
    const char *name;
    const char *index;
    /* data */
}; Student stu3{18,"Rion","23022020821"};
void exGetInfo(Student *stu){
    LOG("\n");
    LOG("外部获取"<<"\n");
    LOG("name: "<<stu->name<<"\n");
    LOG("age: "<<stu->age<<"\n");
    LOG("index: "<<stu->index<<"\n");
}


//交换地址
void mySwap(int *pa, int *pb) {
    int temp = *pa;
    *pa = *pb;
    *pb = temp;
}
//数组顺序反转
void reverseArray(int arr[],int len) {
    for (int i = 0; i < len / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = temp;
    }
}
//优化反转
void reverseArrayp(int* a, int n) {          // 原地算法
    int l = 0, r = n - 1;
    while (l <  r) {
        std::swap(a[l++], a[r--]);
    }
}
void reverseArrayp(char* a, int n) {          // 原地算法
    int l = 0, r = n - 1;
    while (l < r) {
        std::swap(a[l++], a[r--]);
    }
}
//生成随机数组
void getrandArray(int* randArr, int len) {
    for (int i{}; i < len; i++) {
        *(randArr + i) = rand()%120+1;
    }
}
//打印数组
void showArray(int n[], int len) {
    for (int i{}; i < len; i++) {
        LOG(n[i]);
    }
}
void showArray(char *str,int len) {
    for(int i{};i<len;i++) {
        LOG(*(str+i));
    }
}
//冒泡排序
void bubbleSort(int arr[], int len) {
    for (int i{}; i < len; i++) {
        for (int j{}; j < (len - i - 1); j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
//选择排序
void selectionSort(int arr[], int len) {
    for (int i{}; i < len; i++) {
        int min{ i };
        for (int j{i+1}; j < len; j++) {
            if (arr[min] > arr[j]) {
                min = j;
            }
        }
        if (min != i) {
            int temp = arr[min];
            arr[min] = arr[i];
            arr[i] = temp;
        }
    }
}
//检查重复
bool checkRepeat(int arr[], int len) {
    for (int i = 0; i < len; ++i)
        for (int j = i + 1; j < len; ++j)
            if (arr[i] == arr[j]) return true;
    return false;
}
int mystrlen(const char* str) {
    const char* str_t = str;
    while (*str) {
        str++;
    }
    return str-str_t;
}


int main() {
    srand(static_cast<unsigned int>(time(0))); //初始化随机数种子
    int randArr[100];
    getrandArray(randArr,100); //随机数组
    int a[10]{ 100,21,84,7,32,12,1,125,22,11 }; //测试数组
#if 0
    showArray(a, 10);
    LOG("\n");
    bubbleSort(a, 10);
    LOG("\n");
    showArray(a, 10);
    LOG("\n");
#endif
#if 0
    showArray(a, 10);
    LOG("\n");
    selectionSort(a, 10);
    LOG("\n");
    showArray(a, 10);
#endif
#if 0
    showArray(a, 10);
    reverseArrayp(a, 10);
    LOG("\n");
    showArray(a, 10);
#endif
#if 0
    showArray(randArr, 100);
    LOG("\n");
    if (checkRepeat(randArr, 100)) {
        LOG("有相同数据");
    }
    else {
        LOG("没有相同数据");
    }
#endif
#if 0
    //1-45随机6位数，6位数不能相同，从小到大排序输出
    int q_59[6]{};
    while (checkRepeat(q_59, 6)) {
        getrandArray(q_59, 6);
    }
    selectionSort(q_59, 6);
    showArray(q_59, 6);
#endif
#if 0
    int box[45]{};
    for (int i = 0; i < 45; i++) box[i] = i + 1;
    int q_59[6];
    for (int i{}; i < 6; ++i) {
        int idx = rand() % (45 - i);
    }
    int idx = rand() % (44 - i);
#endif
#if 0
    int sa{ 4 };
    int sb{ 9 };
    mySwap(&sa, &sb);
    LOG(sa << "," << sb);
#endif
    //函数指针 int (*func)(int, int)
#if 0
    int arr1[30]{};
    int arr2[30]{};
    int put[30]{};
    int putCnt{};
    int flog = 1;
    getrandArray(arr1, 30);
    getrandArray(arr2, 30);
    LOG("第一个数组：");
    showArray(arr1, 30);
    LOG("\n");
    LOG("第二个数组：");
    showArray(arr2, 30);
    LOG("\n");
    for (int i{}; i < 30; i++) {
        bool foundInArr2 = false;
        for (int j{}; j < 30; j++) {
            if (arr1[i] == arr2[j])
                foundInArr2 = true;
        }
        if (!foundInArr2) continue;
        bool alreadyInPut = false;
        for (int k{}; k < putCnt; ++k) {
            if (put[k] == arr1[i])
            {
                alreadyInPut = true;
                break;
            }
        }
        if (!alreadyInPut) { 
            put[putCnt++] = arr1[i];
        }
    }
    LOG("\n");
    LOG("数组的重复数：");
    for (int i{}; i < putCnt; ++i) {
        LOG(put[i]);
    }
#endif
#if 0
    int sum,base;
    INPUT(sum);
    INPUT(base);
    char buf[100];
    int flog{};
    while(sum>0){
        buf[flog++] = sum % base < 10 ? '0' + (sum % base): 'A' + (sum % base) - 10;
        sum = sum / base;
    }
    for (int i = flog-1; i >= 0; --i) {
        LOG(buf[i]);
    }
#endif
#if 0
    char str[10]{ '1','a','b','c','t','u' };
    reverseArrayp(str, 9);
    showArray(str,10);
    LOG("\n");
#endif
#if 0
    const int len {5};
    char str[len][80];
    for(int i{};i<len;i++){
        LOG("输入第"<<i+1<<"“字符串\n");
        INPUT(str[i]);
    }
    for(int i{};i<len;i++){
        for(int j{};j<len-1-i;j++){
            if(std::strcmp(str[j],str[j+1])>0){
                char temp[80];
                memcpy(temp,str[j],80);
                memcpy(str[j],str[j+1],80);
                memcpy(str[j+1],temp,80);
            }
        }
    }
    LOG("结果： \n");
    for(int i{};i<len;i++){
        LOG(str[i]);
    }
#endif
#if 0
    const int len {5};
    const int MAX = 80; 
    char **str = new char*[len];
    for(int i{};i<len;++i){
        str[i] = new char[MAX];
    }
    for(int i{};i<len;i++){
        LOG("输入第"<<i+1<<"字符串\n");
        INPUT(str[i]);
    }
    for(int i{};i<len;i++){
        for(int j{};j<len-1-i;j++){
            if(std::strcmp(str[j],str[j+1])>0){
                char temp[80];
                memcpy(temp,str[j],80);
                memcpy(str[j],str[j+1],80);
                memcpy(str[j+1],temp,80);
            }
        }
    }
    LOG("结果： \n");
    for(int i{};i<len;i++){
        LOG(str[i]);
        delete(str[i]);
    }
    LOG("\n");
#endif
#if 0
    //引用：给变量起别名
    const char* idiom[] = {
        "跋山涉水",
        "百尺竿头",
        "百年树人",
        "报仇雪恨",
        "暴风骤雨",
        "杯弓蛇影",
        "背信弃义",
        "笨鸟先飞",
        "变幻无常",
        "博古通今",
        "不由分说"
    };
        int flog{1};
    while(flog){
        LOG("成语填空开始！（按q/Q退出）\n");
        int randnum = rand() % 11;
        const char *orig  = idiom[randnum];
        int charCnt = strlen(orig) / 3;
        int pick    = rand() % charCnt;
        int bytePos = pick * 3;

        //把那个汉字换成一个 '_'
        for (int i = 0; i < charCnt; ++i) {
            if (i == pick) {
                LOG('_');
            }
            else {
                std::cout.write(orig + i * 3, 3);
            }       
        }
        std::cout << '\n';

        //读3字节并拼回去
        LOG("你的答案：");
        char key[4]{0};
        std::cin >> std::ws;
        std::cin.getline(key, sizeof(key));   // 先整行读
        if (key[0] == 'q' || key[0] == 'Q') break;

        char buf[32];
        strcpy(buf, orig);
        std::memcpy(buf + bytePos, key, 3);   // 填 3 字节

        if (std::strcmp(buf, orig) == 0) LOG("正确！\n");
        else LOG("错误，正确答案是：" << orig << "\n");
    }
#endif
    //高效阶乘
#if 1
    int n[1000] = {1};
    int count = 1;
    for(int i{2};i<=100;i++){
        for (int j{};j<count;j++){
            n[j]*=i;
        }
        for(int j{};j<count;j++){
            n[j+1] += n[j]/10;
            n[j] %= 10;
        }
        while (n[count]>0){
            n[count+1] = n[count] / 10;
            n[count] %= 10;
            count++;
        }
    }
    LOG("100!=");
    for(int i=count-1;i>=0;i--){
        LOG(n[i]);
    }
#endif
    /**这段代码思路是对的：用「低位在前」的数组模拟手算乘法，逐级进位。
但有两处小 bug，导致最高位可能丢数字或者多输出 0：
进位循环里 n[j+1] += ... 可能把 count 位置外的数字也加进来，但下一轮外层的 count 并没有同步增加；
最后输出循环从 count-1 开始，而 count 在 while 循环结束后会指向第一个 0 位，所以最高位被跳过。**/

    /*(base) shigure@shigure:~/MyCppProject/ycx_02$ ./src/ycx01
*** stack smashing detected ***: terminated
已中止 (核心已转储)*/
    //优化阶乘
#if 0
    int n[1000] = {1};          // 低位在前，n[0] 是个位
    int count = 1;             // 已用位数

    for (int i = 2; i <= 100; ++i) {
    /* 1. 每位乘 i */
    for (int j = 0; j < count; ++j) n[j] *= i;

    /* 2. 统一进位，并扩张最高位 */
    for (int j = 0; j < count; ++j) {
        if (n[j] < 10) continue;
        n[j + 1] += n[j] / 10;
        n[j] %= 10;
        if (j + 1 == count) ++count;   // 需要新位时立刻扩张
        }
    }

    /* 3. 打印 */
    LOG("100! = ");
    for (int i = count - 1; i >= 0; --i) LOG(n[i]);
#endif
    Student stu1;
    stu1.age = 20;
    stu1.index = "22016021241";
    stu1.name = "Tom";
    stu1.getInfo();
    exGetInfo(&stu1);
    Student stu2{21,"Alan","22016023201"};
    exGetInfo(&stu2);
    exGetInfo(&stu3);
    return 0;  
}