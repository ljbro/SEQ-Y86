#include "memory.h"
#include "cache.h"
#include <cstring>
Cache cache(4, 64);
void longLongToCharArray(long long value, char arr[8])
{
    std::memcpy(arr, &value, sizeof(value));
}
bool Cache::isHit(int tag)
{
    for (int i = 0; i < numLines; i++)
    {
        if (lines[i].tag == tag && lines[i].valid)
            return true;
    }
    return false;
}
void Cache::updateCache(int tag)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            lines[tag % 4].data[8 * i + j] = men[8 * tag + i].ch[j];
        }
    }
    lines[tag % 4].valid = true;
}
void Cache::writeback(int tag)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            men[8 * tag + i].ch[j] = lines[tag % 4].data[8 * i + j];
        }
    }
}
char *Cache::readData(int tag, int index, int size)
{
    if (isHit(tag))
    {
        char *data = new char[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = lines[tag % 4].data[index + i];
        }
        return data;
    }
    else
    {
        updateCache(tag);
        char *data = new char[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = lines[tag % 4].data[index + i];
        }
        return data;
    }
}
void Cache::writeData(int tag, int index, int size, char *data)
{
    if (isHit(tag))
    {
        for (int i = 0; i < size; i++)
        {
            lines[tag % 4].data[index + i] = data[i];
        }
    }
    else
    {
        updateCache(tag);
        for (int i = 0; i < size; i++)
        {
            lines[tag % 4].data[index + i] = data[i];
        }
    }
}
