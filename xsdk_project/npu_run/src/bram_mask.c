
#include "bram_mask.h"

#include "xil_io.h"

void BramMask32Write(
    unsigned int MaskedAddr,
    unsigned int Mask32,
    unsigned int BramNumber
) {
    Xil_Out32(INJECTOR_BASEADDR+0x00, MaskedAddr);
    Xil_Out32(INJECTOR_BASEADDR+0x04, Mask32);
    Xil_Out32(INJECTOR_BASEADDR+0x0C, BramNumber);
    Xil_Out32(INJECTOR_BASEADDR+0x10, 0x1);
}

void BramMask64Write(
    unsigned int MaskedAddr,
    unsigned int MaskHigher32,
    unsigned int MaskLower32,
    unsigned int BramNumber
) {
    Xil_Out32(INJECTOR_BASEADDR+0x00, MaskedAddr);
    Xil_Out32(INJECTOR_BASEADDR+0x04, MaskLower32);
    Xil_Out32(INJECTOR_BASEADDR+0x08, MaskHigher32);
    Xil_Out32(INJECTOR_BASEADDR+0x0C, BramNumber);
    Xil_Out32(INJECTOR_BASEADDR+0x10, 0x1);
}

void BramMask64Higher32Write(
    unsigned int MaskedAddr,
    unsigned int MaskHigher32,
    unsigned int BramNumber
) {
    Xil_Out32(INJECTOR_BASEADDR+0x00, MaskedAddr);
    Xil_Out32(INJECTOR_BASEADDR+0x08, MaskHigher32);
    Xil_Out32(INJECTOR_BASEADDR+0x0C, BramNumber);
    Xil_Out32(INJECTOR_BASEADDR+0x10, 0x1);
}

void BramMaskReset() {
    Xil_Out32(INJECTOR_BASEADDR+0x10, 0x8);
}
