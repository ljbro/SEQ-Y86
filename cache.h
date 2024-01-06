#ifndef _CACHE_H_
#define _CACHE_H_
#include <vector>
struct CacheLine
{
    bool valid;
    bool dirty;
    int tag;
    std::vector<char> data;

    // 构造函数，初始化缓存行
    CacheLine(int lineSize) : valid(false), dirty(false), tag(-1), data(lineSize, 0) {}
};
void longLongToCharArray(long long value, char arr[8]);
// 定义缓存结构
class Cache
{
private:
    std::vector<CacheLine> lines; // 缓存行数组
    int lineSize;                 // 每个缓存行的大小
    int numLines;                 // 缓存行的数量

public:
    Cache(int numLines, int lineSize) : lines(numLines, CacheLine(lineSize)), lineSize(lineSize), numLines(numLines) {}
    bool isHit(int tag);
    char *readData(int tag, int index, int size);
    void writeData(int tag, int index, int size, char *data);
    void writeback(int tag);
    void updateCache(int tag);
};
extern Cache cache;
#endif