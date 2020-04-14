
#ifndef BRAM_MASK_H_
#define BRAM_MASK_H_

#define     INJECTOR_BASEADDR       0x40000000

void BramMask32Write(
    unsigned int MaskedAddr,
    unsigned int Mask32,
    unsigned int BramNumber
);

void BramMask64Write(
    unsigned int MaskedAddr,
    unsigned int MaskHigher32,
    unsigned int MaskLower32,
    unsigned int BramNumber
);

void BramMask64Higher32Write(
    unsigned int MaskedAddr,
    unsigned int MaskHigher32,
    unsigned int BramNumber
);

void BramMaskReset();

#endif
