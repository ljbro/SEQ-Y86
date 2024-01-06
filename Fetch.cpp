#include "Fetch.h"

int geticode(string num)
{
    int numf = 100;
    if (isdigit(num[0]))
    {
        numf = num[0] - '0';
    }
    else if (isalpha(num[0]))
    {
        numf = tolower(num[0]) - 'a' + 10;
    }
    return numf;
}

int getifun(string num)
{
    int numf = 10;

    if (isdigit(num[1]))
    {
        numf = num[1] - '0';
    }

    return numf;
}

int getrA(string num)
{
    int numf = 0; // 默认值为 0

    if (isdigit(num[2]))
    {
        numf = num[2] - '0';
    }
    else if (islower(num[2]))
    {
        numf = num[2] - 'a' + 10;
    }

    return numf;
}

int getrB(string num)
{
    int numf = 0; // 默认值为 0

    if (isdigit(num[3]))
    {
        numf = num[3] - '0';
    }
    else if (islower(num[3]))
    {
        numf = num[3] - 'a' + 10;
    }

    return numf;
}

int getaddr(string num)
{
    char numm[4];
    numm[0] = num[4];
    numm[1] = num[5];
    numm[2] = num[2];
    numm[3] = num[3];

    int ret = 0;
    for (int i = 0; i < 4; i++)
    {
        int digit;
        if (isdigit(numm[i]))
        {
            digit = numm[i] - '0';
        }
        else if (isxdigit(numm[i]))
        {
            digit = tolower(numm[i]) - 'a' + 10;
        }
        else
        {
            return 0;
        }

        ret = digit + ret * 16;
    }

    return ret;
}

long long getvalC(string num)
{
    string a(num, 4);
    string b;
    if (a == "0000000000000000")
        return 0;
    int size = a.size();
    for (int i = size - 1; i > 0; i -= 2)
    {
        b.push_back(a[--i]);
        b.push_back(a[++i]);
    }
    string c;
    int flag = 1;
    for (int i = 0; i < size; i++)
    {
        if (b[i] == '0' && flag)
            continue;
        else
        {
            flag = 0;
            c.push_back(b[i]);
        }
    }
    stringstream all1;
    long long int d1;
    if (c[0] <= '7' || c.size() < 16)
    {

        all1 << hex << c;
        all1 >> d1;
        return d1;
    }
    else
    {
        string temp = processMapping(c);

        all1 << hex << temp;
        all1 >> d1;
        return -(d1 + 1);
    }
}
int canculation(int code, int fun)
{
    int numf;
    numf = 10 * code + fun;
    return numf;
}
