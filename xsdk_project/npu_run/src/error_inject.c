
#include "error_inject.h"

#include "test_params.h"
#include "bram_mask.h"
#include "hwicap.h"

#include <stdlib.h>
#include "xil_printf.h"

void ConfigMemSingleBitUpsetInject(CfgMemSEUInfo ConfigMemError) {
    unsigned int frameTmp[101];
    HwicapReadFrame(ConfigMemError.FarAddr, frameTmp);
    frameTmp[ConfigMemError.UpsetWord] = frameTmp[ConfigMemError.UpsetWord] ^ (0x1 << ConfigMemError.UpsetBit);
    HwicapWriteFrame(ConfigMemError.FarAddr, frameTmp, g_Idcode);
}

void BramSingleBitUpsetInject(BramSEUInfo BramError) {
    if (BramError.UpsetBit>=32) {
        unsigned int MaskHigher32 = 0x1 << (BramError.UpsetBit-32);
        BramMask64Higher32Write(BramError.MaskedAddr, MaskHigher32, BramError.BramNumber);
    }
    else {
        unsigned int Mask32 = 0x1 << BramError.UpsetBit;
        BramMask32Write(BramError.MaskedAddr, Mask32, BramError.BramNumber);
    }
}

void ClearBramError() {
    BramMaskReset();
}

void RandomSEU(
    int TotalErrorNum,
    CfgMemSEUInfo *ConfigMemError,
    int *ConfigMemErrorNum,
    BramSEUInfo *BramError,
    int *BramErrorNum
) {
    int configerror = 0;
    int bramerror = 0;

    for (int i=0; i<TotalErrorNum; i++) {
        int seu = rand() % 10000;

        if (seu < 8414) {
            ConfigMemError[configerror].FarAddr.BlockType = 0;
            ConfigMemError[configerror].FarAddr.TopBottom = rand() % 2;
            ConfigMemError[configerror].FarAddr.RowAddress = rand() % 6;
            ConfigMemError[configerror].FarAddr.ColumnAddress = rand() % 101;
            ConfigMemError[configerror].FarAddr.MinorAddress = rand() % 36;
            ConfigMemError[configerror].UpsetWord = rand() % 101;
            ConfigMemError[configerror].UpsetBit = rand() % 32;
            configerror++;
        }
        else if (seu < 8897) {
            int bram_random_num = rand() % 100000;

            if (bram_random_num < 8575) {
                BramError[bramerror].BramNumber = rand() % 8;
                BramError[bramerror].MaskedAddr = rand() % 1024;
                BramError[bramerror].UpsetBit = rand() % 64;
            }
            else if(bram_random_num < 42874) {
                BramError[bramerror].BramNumber = rand() % 32 + 8;
                BramError[bramerror].MaskedAddr = rand() % 1024;
                BramError[bramerror].UpsetBit = rand() % 64;
            }
            else if(bram_random_num < 98610) {
                BramError[bramerror].BramNumber = rand() % 52 + 40;
                BramError[bramerror].MaskedAddr = rand() % 2048;
                BramError[bramerror].UpsetBit = rand() % 32;
            }
            else if(bram_random_num < 98928) {
                BramError[bramerror].BramNumber = 92;
                rand();
                BramError[bramerror].MaskedAddr = rand() % 1024;
                BramError[bramerror].UpsetBit = rand() % 19;
            }
            else if(bram_random_num < 100000) {
                BramError[bramerror].BramNumber = rand() % 8 + 93;
                BramError[bramerror].MaskedAddr = rand() % 128;
                BramError[bramerror].UpsetBit = rand() % 64;
            }
            bramerror++;
            rand();
            rand();
        }
        else {
            rand();
            rand();
            rand();
            rand();
            rand();
            rand();
        }
    }

    xil_printf("%d in config mem, %d in used bram.\r\n", configerror, bramerror);
    *ConfigMemErrorNum = configerror;
    *BramErrorNum = bramerror;

    return;
}
