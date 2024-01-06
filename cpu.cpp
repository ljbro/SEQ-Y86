
#include "cpu.h"
#include "define.h"
#include <string>
#include <iostream>
#include <unordered_set>
#include <math.h>
using namespace std;
void cpu::Fetch()
{
    imem_error = PC < 0 || PC > 1024;

    if (imem_error)
    {
        icode = INOP;
        ifun = 0;
    }
    else
    {
        icode = geticode(datum[counter].val);
        ifun = getifun(datum[counter].val);
        instrvalid = icode > 15;
    }
    icode = geticode(datum[counter].val);
    unordered_set<int> need_regids = {IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ, IIOPQ};
    Needregids = (need_regids.find(icode) != need_regids.end());
    std::unordered_set<int> need_valC = {IRMMOVQ, IMRMOVQ, JXX, ICALL, IIRMOVQ, IIOPQ};
    NeedvalC = (need_valC.find(icode) != need_valC.end());
    valP = PC + 1;
    if (Needregids)
        valP = valP + 1;
    if (NeedvalC)
        valP = valP + 8;
    if (Needregids)
    {
        rA = getrA(datum[counter].val);
        rB = getrB(datum[counter].val);
    }
    else
    {
        rB = RNONE;
        rA = RNONE;
    }
    if (NeedvalC)
    {
        if (icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ || icode == IIOPQ)
        {
            valC = getvalC(datum[counter].val);
        }
        else if (icode == JXX || icode == ICALL)
            valC = getaddr(datum[counter].val);
    }
}
void cpu::getsrcA()
{
    bool need_rA;
    bool need_rsp;

    std::unordered_set<int> need_rA_code = {IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ, IIOPQ};

    std::unordered_set<int> need_rsp_code = {IPOPQ, IRET};

    need_rA = need_rA_code.find(icode) != need_rA_code.end();
    need_rsp = need_rsp_code.find(icode) != need_rsp_code.end();
    if (need_rA)
        srcA = rA;
    else if (need_rsp)
        srcA = RSP;
    else
        srcA = RNONE;
}
void cpu::getsrcB()
{
    bool need_rB;
    bool need_rsp;

    std::unordered_set<int> need_rB_code = {IMRMOVQ, IOPQ, IRMMOVQ, IIOPQ};

    std::unordered_set<int> need_rsp_code = {IPUSHQ, IPOPQ, ICALL, IRET};

    need_rB = need_rB_code.find(icode) != need_rB_code.end();
    need_rsp = need_rsp_code.find(icode) != need_rsp_code.end();
    if (need_rB)
        srcB = rB;
    else if (need_rsp)
        srcB = RSP;
    else
        srcB = RNONE;
}

void cpu::getdstE()
{
    bool need_dstE;
    bool need_rsp;

    std::unordered_set<int> need_rB_code = {IIRMOVQ, IOPQ, IRRMOVQ, IIOPQ};

    std::unordered_set<int> need_rsp_code = {IPUSHQ, IPOPQ, ICALL, IRET};

    need_dstE = need_rB_code.find(icode) != need_rB_code.end();
    need_rsp = need_rsp_code.find(icode) != need_rsp_code.end();
    if (need_dstE)
    {
        if (icode == IIRMOVQ || icode == IOPQ || icode == IIOPQ || (icode == IRRMOVQ && ifun == 0))
            dstE = rB;
        else if (icode == IRRMOVQ && ifun != 0 && cnd)
        {
            dstE = rB;
        }
        else if (icode == IRRMOVQ && ifun != 0 && !cnd)
        {
            dstE = RNONE;
        }
    }
    else if (need_rsp)
        dstE = RSP;
    else
        dstE = RNONE;
}
void cpu::getdstM()
{
    bool need_dstM;
    bool need_rsp;

    std::unordered_set<int> need_rA_code = {IMRMOVQ, IPOPQ};

    need_dstM = need_rA_code.find(icode) != need_rA_code.end();
    if (need_dstM)
        dstM = rA;
    else
        dstM = RNONE;
}
void cpu::Decode()
{
    getsrcA();
    getsrcB();
    if (srcA != 15)
        valA = reg[srcA].ll;
    if (srcB != 15)
        valB = reg[srcB].ll;
}
void cpu::getALUA()
{
    std::unordered_set<int> need_valA = {IRRMOVQ, IOPQ};
    std::unordered_set<int> need_valC = {IIRMOVQ, IRMMOVQ, IMRMOVQ, IIOPQ};
    std::unordered_set<int> need_eight = {IRET, IPOPQ};
    std::unordered_set<int> need_negeight = {ICALL, IPUSHQ};
    bool needA, needC, need8, needn8;
    needA = need_valA.find(icode) != need_valA.end();
    needC = need_valC.find(icode) != need_valC.end();
    need8 = need_eight.find(icode) != need_eight.end();
    needn8 = need_negeight.find(icode) != need_negeight.end();
    if (needA)
        ALUA = valA;
    else if (needC)
    {
        ALUA = valC;
    }
    else if (need8)
        ALUA = 8;
    else if (needn8)
        ALUA = -8;
    else
        ALUA = 0;
}
void cpu::getALUB()
{
    std::unordered_set<int> need_valB = {IRMMOVQ, IMRMOVQ, IOPQ, ICALL, IPUSHQ, IRET, IPOPQ, IIOPQ};
    bool flag;
    flag = need_valB.find(icode) != need_valB.end();
    if (flag)
        ALUB = valB;
    else
        ALUB = 0;
}
void cpu::getalufun()
{
    if (icode == IOPQ || icode == IIOPQ)
        alufun = ifun;
    else
        alufun = 0;
}
void cpu::set_valE_cnd()
{
    getalufun();
    long long mask;
    uint64_t a_low;
    uint64_t a_high;
    uint64_t b_low;
    uint64_t b_high;
    uint64_t low_mul;
    uint64_t mid1_mul;
    uint64_t mid2_mul;
    uint64_t mid_combined;
    switch (alufun)
    {
    case 0:
        valE.ll = ALUA + ALUB;
        break;
    case 1:
        valE.ll = ALUB - ALUA;
        break;
    case 2:
        valE.ll = ALUA & ALUB;
        break;
    case 3:
        valE.ll = ALUA ^ ALUB;
        break;
    case 4:
        valE.ll = ALUB << ALUA;
        break;
    case 5:
        mask = (1LL << (64 - ALUA)) - 1;
        valE.ll = (ALUB >> ALUA) & mask;
        break;
    case 6:
        valE.ll = ALUB << ALUA;
        break;
    case 7:
        valE.ll = ALUB >> ALUA;
        break;
    case 8:
        valE.ll = max(ALUB, ALUA);
        break;
    case 9:
        valE.ll = min(ALUB, ALUA);
        break;
    case 10:
        a_low = ALUA & 0xFFFFFFFF;
        a_high = ALUA >> 32;
        b_low = ALUB & 0xFFFFFFFF;
        b_high = ALUB >> 32;
        low_mul = a_low * b_low;
        mid1_mul = a_high * b_low;
        mid2_mul = a_low * b_high;
        mid_combined = (mid1_mul & 0xFFFFFFFF) + (mid2_mul & 0xFFFFFFFF) + (low_mul >> 32);
        valE.ll = (mid_combined << 32) | (low_mul & 0xFFFFFFFF);
        break;
    }
    if (icode == IOPQ || icode == IIOPQ)
    {
        if (valE.ll == 0)
            ZF = 1;
        else
            ZF = 0;
        if (valE.ll < 0)
            SF = 1;
        else
            SF = 0;
        if (alufun == 0)
        {
            if (ALUA > 0 && ALUB > 0 && valE.ll <= 0)
                OF = 1;
            else if (ALUA < 0 && ALUB < 0 && valE.ll >= 0)
                OF = 1;
            else
                OF = 0;
        }
    }
}
void cpu::set_cnd()
{
    if (ifun == 0)
        cnd = true;
    else if (ifun == 1)
    {
        if ((SF ^ OF) | ZF)
            cnd = true;
        else
            cnd = false;
    }
    else if (ifun == 2)
    {
        if (SF ^ OF)
            cnd = true;
        else
            cnd = false;
    }
    else if (ifun == 3)
    {
        if (ZF == 1)
            cnd = true;
        else
            cnd = false;
    }
    else if (ifun == 4)
    {
        if (ZF == 0)
            cnd = true;
        else
            cnd = false;
    }
    else if (ifun == 5)
    {
        if (!(SF ^ OF))
            cnd = true;
        else
            cnd = false;
    }
    else if (ifun == 6)
    {
        if (((!(SF ^ OF)) && !ZF))
            cnd = true;
        else
            cnd = false;
    }
    else
        cnd = false;
}
void cpu::Execute()
{
    getALUA();
    getALUB();
    set_valE_cnd();
    set_cnd();
}
int cpu::jump(long long addr)
{

    auto it = codes.find(addr);
    if (it == codes.end())
    {
        stat = 3;
        return totalnums + 1;
    }
    else
    {
        return it->second;
    }
}
void cpu::get_set_or_not()
{
    if (icode == 13)
        set_or_not = false;
    else
        set_or_not = true;
}
void cpu::get_read_bits()
{
    if (icode == IMRMOVQ)
    {
        switch (ifun)
        {
        case 0:
            read_bits = 8;
            sign = false;
            break;
        case 1:
            read_bits = 4;
            sign = false;
            break;
        case 2:
            read_bits = 2;
            sign = false;
            break;
        case 3:
            read_bits = 1;
            sign = false;
            break;
        case 4:
            read_bits = 4;
            sign = true;
            break;
        case 5:
            read_bits = 2;
            sign = true;
            break;
        case 6:
            read_bits = 1;
            sign = true;
            break;
        }
    }
    else
        read_bits = 8;
}
void cpu::get_write_bits()
{
    if (icode == IRMMOVQ)
    {
        switch (ifun)
        {
        case 0:
            write_bits = 8;
            break;
        case 1:
            write_bits = 4;
            break;
        case 2:
            write_bits = 2;
            break;
        case 3:
            write_bits = 1;
            break;
        }
    }
    else
        write_bits = 8;
}
void cpu::Memory()
{
    getdstE();
    getdstM();
    get_mem_addr();
    get_mem_data();
    get_memread_write();
    get_read_bits();
    get_write_bits();
    get_set_or_not();
    if (mem_write)
    {
        if (write_bits == 8)
        {
            int tag = (mem_addr) / 64;
            int index = (((mem_addr) % 64) / 8) * 8;
            cache.writeData(tag, index, 8, mem_data.ch);
            cache.writeback(tag);
        }
        else if (write_bits == 4)
        {
            long long tag = mem_addr / 64;
            long long index = mem_addr % 64;
            if (index > 60)
            {
                cache.writeData(tag, index, 4, mem_data.ch + 4);
                cache.writeData(tag + 1, 0, index - 60, mem_data.ch + 68 - index);
                cache.writeback(tag);
                cache.writeback(tag + 1);
            }
            else
            {
                cache.writeData(tag, index, 4, mem_data.ch + 4);
                cache.writeback(tag);
            }
        }
        else if (write_bits == 2)
        {

            long long tag = mem_addr / 64;
            long long index = mem_addr % 64;
            if (index > 62)
            {
                cache.writeData(tag, index, 2, mem_data.ch + 6);
                cache.writeData(tag + 1, 0, index - 62, mem_data.ch + 70 - index);
                cache.writeback(tag);
                cache.writeback(tag + 1);
            }
            else
            {
                cache.writeData(tag, index, 2, mem_data.ch + 6);
                cache.writeback(tag);
            }
        }
        else
        {
            long long tag = mem_addr / 64;
            long long index = mem_addr % 64;
            cache.writeData(tag, index, 1, mem_data.ch + 7);
            cache.writeback(tag);
        }
    }
    if (mem_read)
    {
        if (read_bits == 8)
        {
            int tag = mem_addr / 64;
            int index = ((mem_addr % 64) / 8) * 8;
            char *data = cache.readData(tag, index, 8);
            for (int i = 0; i < 8; i++)
            {
                valM.ch[i] = data[i];
            }
            delete[] data;
        }
        else if (read_bits == 4)
        {
            long long tag = mem_addr / 64;
            long long index = mem_addr % 64;

            for (int i = 0; i < 4; i++)
            {
                valM.ch[i] = 0;
            }
            if (index > 60)
            {
                char *data1 = cache.readData(tag, index, 64 - index);
                char *data2 = cache.readData(tag + 1, 0, index - 60);
                for (int i = 0; i < 64 - index; i++)
                {
                    valM.ch[3 - i] = data1[i];
                }
                for (int i = 0; i < index - 60; i++)
                {
                    valM.ch[index - 61 - i] = data2[i];
                }
                if (sign && ((data1[0] & char(0x80)) != 0))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        valM.ch[7 - i] = 0xff;
                    }
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        valM.ch[7 - i] = 0;
                    }
                }
            }
            else
            {
                char *data = cache.readData(tag, index, 4);
                for (int i = 0; i < 4; i++)
                {
                    valM.ch[3 - i] = data[i];
                }
                if (sign && ((data[0] & char(0x80)) != 0))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        valM.ch[7 - i] = 0xff;
                    }
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        valM.ch[7 - i] = 0;
                    }
                }
            }
        }
        else if (read_bits == 2)
        {
            long long tag = mem_addr / 64;
            long long index = mem_addr % 64;
            if (index > 62)
            {
                char *data1 = cache.readData(tag, index, 64 - index);
                char *data2 = cache.readData(tag + 1, 0, index - 62);
                for (int i = 0; i < 64 - index; i++)
                {
                    valM.ch[1 - i] = data1[i];
                }
                for (int i = 0; i < index - 62; i++)
                {
                    valM.ch[index - 63 - i] = data2[i];
                }
                if (sign && ((data1[0] & char(0x80)) != 0))
                {
                    for (int i = 0; i < 6; i++)
                    {
                        valM.ch[7 - i] = 0xff;
                    }
                }
                else
                {
                    for (int i = 0; i < 6; i++)
                    {
                        valM.ch[7 - i] = 0;
                    }
                }
            }
            else
            {
                char *data = cache.readData(tag, index, 2);
                for (int i = 0; i < 2; i++)
                {
                    valM.ch[1 - i] = data[i];
                }
                if (sign && ((data[0] & char(0x80)) != 0))
                {
                    for (int i = 0; i < 6; i++)
                    {
                        valM.ch[7 - i] = 0xff;
                    }
                }
                else
                {
                    for (int i = 0; i < 6; i++)
                    {
                        valM.ch[7 - i] = 0;
                    }
                }
            }
        }
        else if (read_bits == 1)
        {
            long long tag = mem_addr / 64;
            long long index = mem_addr % 64;

            char *data = cache.readData(tag, index, 1);
            for (int i = 0; i < 1; i++)
            {
                valM.ch[i] = data[i];
            }
            if (sign && ((data[0] & char(0x80)) != 0))
            {
                for (int i = 0; i < 7; i++)
                {
                    valM.ch[7 - i] = 0xff;
                }
            }
            else
            {
                for (int i = 0; i < 7; i++)
                {
                    valM.ch[7 - i] = 0;
                }
            }
        }
    }
    if (dstE != RNONE && set_or_not)
    {
        for (int i = 0; i < 8; i++)
        {
            reg[dstE].ch[i] = valE.ch[i];
        }
    }
    if (dstM != RNONE)
    {

        for (int i = 0; i < 8; i++)
        {
            reg[dstM].ch[i] = valM.ch[i];
        }
    }
}
void cpu::updatePC()
{
    if (icode == ICALL)
        PC = valC;
    else if (icode == JXX)
    {
        if (cnd)
        {
            PC = valC;
        }
        else
            PC = valP;
    }
    else if (icode == IRET)
        PC = valM.ll;
    else
        PC = valP;
}
void cpu::get_mem_addr()
{
    if (icode == IRMMOVQ || icode == IPUSHQ || icode == ICALL || icode == IMRMOVQ)
    {
        mem_addr = valE.ll;
    }
    else
        mem_addr = valA;
}
void cpu::get_mem_data()
{
    if (icode == IRMMOVQ || icode == IPUSHQ)
        mem_data.ll = valA;
    if (icode == ICALL)
        mem_data.ll = valP;
}
void cpu::get_memread_write()
{
    if (icode == IPOPQ || icode == IRET || icode == IMRMOVQ)
        mem_read = true;
    else
        mem_read = false;
    if ((icode == IPUSHQ && reg[RSP].ll > 0) || icode == ICALL || icode == IRMMOVQ)
        mem_write = true;
    else
        mem_write = false;
}
void cpu::updateStat()
{
    if (icode == IHALT)
        stat = 2;
}
void cpu::run()
{
    std::cout << "[" << endl;
    while (counter <= totalnums)
    {
        Fetch();
        Decode();
        Execute();
        Memory();
        updatePC();
        updateStat();

        if (reg[RSP].ll < 0)
        {
            stat = 3;
        }
        output();

        if (stat == 2 || stat == 3 || stat == 4)
        {
            break;
        }
        counter = jump(PC);
    }
    std::cout << "]" << endl;
}

void cpu::output()
{
    int end;
    std::cout << "    {" << endl;
    if (stat == 2 || stat == 3 || stat == 4)
        std::cout << "      \"PC\""
                  << ":" << datum[counter].pc << "," << endl;
    else
        std::cout << "       \"PC\""
                  << ":" << PC << "," << endl;
    std::cout << "       \"REG\""
              << ":"
              << "{" << endl;
    std::cout << "           \"rax\""
              << ":" << reg[RAX].ll << "," << endl;
    std::cout << "           \"rcx\""
              << ":" << reg[RCX].ll << "," << endl;
    std::cout << "           \"rdx\""
              << ":" << reg[RDX].ll << "," << endl;
    std::cout << "           \"rbx\""
              << ":" << reg[RBX].ll << "," << endl;
    std::cout << "           \"rsp\""
              << ":" << reg[RSP].ll << "," << endl;
    std::cout << "           \"rbp\""
              << ":" << reg[RBP].ll << "," << endl;
    std::cout << "           \"rsi\""
              << ":" << reg[RSI].ll << "," << endl;
    std::cout << "           \"rdi\""
              << ":" << reg[RDI].ll << "," << endl;
    std::cout << "           \"r8\""
              << ":" << reg[R8].ll << "," << endl;
    std::cout << "           \"r9\""
              << ":" << reg[R9].ll << "," << endl;
    std::cout << "           \"r10\""
              << ":" << reg[R10].ll << "," << endl;
    std::cout << "           \"r11\""
              << ":" << reg[R11].ll << "," << endl;
    std::cout << "           \"r12\""
              << ":" << reg[R12].ll << "," << endl;
    std::cout << "           \"r13\""
              << ":" << reg[R13].ll << "," << endl;
    std::cout << "           \"r14\""
              << ":" << reg[R14].ll << endl;
    std::cout << "       }"
              << "," << endl;
    std::cout << "       \"MEM\""
              << ":"
              << "{" << endl;
    for (int t = 63; t >= 0; t--)
    {
        if (men[t].ll != 0)
        {
            end = t;
            break;
        }
    }
    for (int z = 0; z < 64; z++)
    {
        int t = z * 8;
        if (z != end && men[z].ll != 0)
            std::cout << "           \"" << t << "\""
                      << ":" << men[z].ll << "," << endl;
        else if (z == end)
            std::cout << "           \"" << t << "\""
                      << ":" << men[z].ll << endl;
    }
    std::cout << "       }"
              << "," << endl;
    std::cout << "       \"CC\""
              << ":"
              << "{" << endl;
    std::cout << "           \"ZF\""
              << ":" << static_cast<int>(ZF) << "," << endl;
    std::cout << "           \"SF\""
              << ":" << static_cast<int>(SF) << "," << endl;
    std::cout << "           \"OF\""
              << ":" << static_cast<int>(OF) << endl;
    std::cout << "       }"
              << "," << endl;
    std::cout << "       \"STAT\""
              << ":" << stat << endl;

    if (stat == 2 || stat == 3 || stat == 4)
        std::cout << "    }" << endl;
    else
        std::cout << "    }"
                  << "," << endl;
}

int main()
{
    memory mem;
    cpu CPU;
    mem.read_file();
    CPU.run();
    return 0;
}