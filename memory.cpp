#include "memory.h"

using namespace std;
all_string datum[100];
map<int, int> codes;
int totalnums = 0;
Val men[64];
unordered_map<char, char> mapping = {
    {'a', '5'}, {'b', '4'}, {'c', '3'}, {'d', '2'}, {'e', '1'}, {'f', '0'}, {'0', 'f'}, {'1', 'e'}, {'2', 'd'}, {'3', 'c'}, {'4', 'b'}, {'5', 'a'}, {'6', '9'}, {'7', '8'}, {'8', '7'}, {'9', '6'}};
memory::memory() {}
memory::~memory() {}
string processMapping(const string &value)
{
    string temp = value;
    for (char &c : temp)
    {
        c = mapping[c];
    }
    return temp;
}
int memory::getlength(string val)
{
    return val.size() / 2;
}
long long memory::getpc(string num)
{
    string b = num;
    long long ret_ = 0;
    if (b[1] == 'x')
    {
        b.erase(0, 2);
    }
    for (int i = 0; i < 3; i++)
    {
        if (std::isxdigit(b[i]))
        { // 检查是否为十六进制数字
            if (std::isdigit(b[i]))
            {
                ret_ = ret_ * 16 + (b[i] - '0'); // '0' 到 '9'
            }
            else
            {
                ret_ = ret_ * 16 + (std::tolower(b[i]) - 'a' + 10); // 'a' 到 'f' 或 'A' 到 'F'
            }
        }
        else
        {
            throw std::invalid_argument("Invalid hex character");
        }
    }
    return ret_;
}

void memory::env_build()
{
    string aggregatedValues = "";
    string lines[100];
    for (int i = 0; i < totalnums; i++)
    {
        int size = datum[i].val.size();
        int all_size = aggregatedValues.size();
        string temp = datum[i].val;
        if (i < totalnums - 1)
            if (datum[i + 1].pc - datum[i].pc > size / 2)
            {
                int x = datum[i + 1].pc - datum[i].pc - size / 2;
                x *= 2;
                for (int j = 0; j < x; j++)
                    temp.push_back('0');
                aggregatedValues += temp;
            }
            else
            {
                aggregatedValues += temp;
            }
        else
            aggregatedValues += temp;
    }
    int alls = aggregatedValues.size() % 16 == 0 ? aggregatedValues.size() / 16 : aggregatedValues.size() / 16 + 1;
    int index = 0;
    for (int i = 0; i < alls; i++)
    {
        lines[i] = aggregatedValues.substr(index, 16);
        index += 16;
        int size = lines[i].size();
        string b = "";
        for (int j = size - 1; j > 0; j -= 2)
        {
            b.push_back(lines[i][--j]);
            b.push_back(lines[i][++j]);
        }
        lines[i] = b;
    }
    for (int i = 0; i < alls; i++)
    {
        stringstream iss;
        long long int d1;
        if (lines[i][0] <= '7' || lines[i].size() < 16)
        {

            iss << hex << lines[i];
            iss >> d1;
            men[i].ll = d1;
        }
        else
        {
            string processedValue = processMapping(lines[i]);

            iss << hex << processedValue;
            iss >> d1;
            men[i].ll = -(d1 + 1);
        }
    }
}
void memory::read_file()
{
    string line;
    while (cin >> line)
    {
        string sTmp;
        if (line == "|")
        {
            getline(cin, sTmp);
            continue;
        }
        cin >> datum[totalnums].val;

        if (datum[totalnums].val == "|")
        {
            getline(cin, sTmp);
            continue;
        }
        datum[totalnums].pc = getpc(line);
        codes[datum[totalnums].pc] = totalnums;
        totalnums++;
    }
    env_build();
}