
#ifndef _SOMEFILE_H_
#define _SOMEFILE_H_
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;
struct all_string
{
    int pc;
    string val;
};
union Val
{
    char ch[8];
    long long ll;
};
extern map<int, int> codes;
extern all_string datum[];
extern Val men[];
extern int totalnums;
extern unordered_map<char, char> mapping;
string processMapping(const string &value);
class memory
{
public:
    memory();
    ~memory();
    void read_file();          // 读文件
    void env_build();          // 生成内存
    long long getpc(string a); // 计算要存储的PC
    int getlength(string a);
};

#endif