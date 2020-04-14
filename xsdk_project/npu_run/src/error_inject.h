
#ifndef ERROR_INJECT_H_
#define ERROR_INJECT_H_

#include "hwicap.h"

extern unsigned int g_Idcode;

typedef struct cfgmemseuinfo{
    FarAddrInfo FarAddr;
    unsigned int UpsetWord;
    unsigned int UpsetBit;
} CfgMemSEUInfo;

typedef struct bramseuinfo{
    unsigned int BramNumber;
    unsigned int MaskedAddr;
    unsigned int UpsetBit;
} BramSEUInfo;

void ConfigMemSingleBitUpsetInject(CfgMemSEUInfo ConfigMemError);

void BramSingleBitUpsetInject(BramSEUInfo BramError);

void ClearBramError();

void RandomSEU(
    int TotalErrorNum,
    CfgMemSEUInfo *ConfigMemError,
    int *ConfigMemErrorNum,
    BramSEUInfo *BramError,
    int *BramErrorNum
);

#endif