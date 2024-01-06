#ifndef _CPU_H_
#define _CPU_H_
#include "memory.h"
#include "Fetch.h"
#include "cache.h"

using namespace std;

class cpu
{
public:
    Val reg[15] = {0};
    int stat = 1;
    int counter = 0;
    bool ZF = 1;
    bool OF = 0;
    bool SF = 0;
    bool CF = 0;
    int PC = 0, f_pc;
    long long valC, valP, valA, valB;
    Val valM, valE;
    int icode, ifun, rA, rB;
    bool set_or_not;
    bool instrvalid = false, NeedvalC, Needregids, imem_error = false;
    bool dmem_error = false, cnd = false, mem_read, mem_write;
    int srcA, srcB, dstE, dstM;
    long long ALUA, ALUB;
    bool sign;
    int read_bits, write_bits;
    long long mem_addr = 0;
    Val mem_data;
    int alufun;
    void get_read_bits();
    void get_write_bits();
    void run();
    void Fetch();
    void Decode();
    void Execute();
    void Memory();
    void updatePC();
    void output();
    void getsrcA();
    void getsrcB();
    void getdstE();
    void getdstM();
    void getALUA();
    void getALUB();
    void getalufun();
    void set_valE_cnd();
    void set_cnd();
    void get_memread_write();
    void get_mem_addr();
    void get_mem_data();
    void updateStat();
    int jump(long long addr);
    void get_set_or_not();
};
#endif
