
#include "hwicap.h"

#include "xil_io.h"
#include "xil_printf.h"

void PCAP2ICAP() {
    unsigned int ctrl = Xil_In32(0xF8007000);
    Xil_Out32(0xF8007000,(ctrl&(~0x08000000)));
}

void HwicapSync() {
    unsigned int hwicap_wfv;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0xFFFFFFFF);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x000000BB);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x11220044);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0xFFFFFFFF);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0xAA995566);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);
}


unsigned int HwicapReadIdcode() {
    unsigned int hwicap_wfv;
    unsigned int hwicap_rfo;
    unsigned int hwicap_idcode;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x28018001);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);

    Xil_Out32(HWICAP_BASEADDR+0x108, 0x00000001);
    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000002);

    do {
        hwicap_rfo = Xil_In32(HWICAP_BASEADDR+0x118);
    } while (hwicap_rfo != 1);

    hwicap_idcode = Xil_In32(HWICAP_BASEADDR+0x104);
    return hwicap_idcode;
}


unsigned int HwicapReadStatus() {
    unsigned int hwicap_wfv;
    unsigned int hwicap_rfo;
    unsigned int hwicap_status;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x2800E001);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);

    Xil_Out32(HWICAP_BASEADDR+0x108, 0x00000001);
    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000002);

    do {
        hwicap_rfo = Xil_In32(HWICAP_BASEADDR+0x118);
    } while (hwicap_rfo != 1);

    hwicap_status = Xil_In32(HWICAP_BASEADDR+0x104);
    return hwicap_status;
}


void HwicapWriteCMD(unsigned int CMDCode) {
    unsigned int hwicap_wfv;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30008001);
    Xil_Out32(HWICAP_BASEADDR+0x100, CMDCode);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);
}


unsigned int FarAddress(FarAddrInfo FarAddr) {
    return((FarAddr.BlockType << 23) + (FarAddr.TopBottom << 22) + 
           (FarAddr.RowAddress << 17) + (FarAddr.ColumnAddress << 7) + 
           FarAddr.MinorAddress);
}


void HwicapReadFrame(FarAddrInfo FarAddr, unsigned int* ReadResult) {
    unsigned int hwicap_wfv;
    unsigned int hwicap_rfo;
    int i_frameword;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30002001);
    Xil_Out32(HWICAP_BASEADDR+0x100, FarAddress(FarAddr));
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30008001);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x00000004);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x280060CA);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);

    Xil_Out32(HWICAP_BASEADDR+0x108, 0x000000CB);
    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000002);

    do {
        hwicap_rfo = Xil_In32(HWICAP_BASEADDR+0x118);
    } while (hwicap_rfo != 203);

    for (i_frameword = 0; i_frameword<102; i_frameword++) {
        Xil_In32(HWICAP_BASEADDR+0x104);
    }

    for (i_frameword = 0; i_frameword<101; i_frameword++) {
        *(ReadResult+i_frameword) = Xil_In32(HWICAP_BASEADDR+0x104);
    }
}


void HwicapWriteFrame(
    FarAddrInfo FarAddr, 
    unsigned int* WriteFrameData, 
    unsigned int IDcode
) {
    unsigned int hwicap_wfv;
    int i_frameword;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30002001);
    Xil_Out32(HWICAP_BASEADDR+0x100, FarAddress(FarAddr));
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30018001);
    Xil_Out32(HWICAP_BASEADDR+0x100, IDcode);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30008001);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x00000001);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x300040CA);
    for (i_frameword = 0; i_frameword<101; i_frameword++) {
        Xil_Out32(HWICAP_BASEADDR+0x100, *(WriteFrameData+i_frameword));
    }
    for (i_frameword = 0; i_frameword<101; i_frameword++) {
        Xil_Out32(HWICAP_BASEADDR+0x100, *(WriteFrameData+i_frameword));
    }
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);
}


void HwicapDesync() {
    unsigned int hwicap_wfv;

    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x30008001);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x0000000D);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);
    Xil_Out32(HWICAP_BASEADDR+0x100, 0x20000000);

    Xil_Out32(HWICAP_BASEADDR+0x10C, 0x00000001);

    do {
        hwicap_wfv = Xil_In32(HWICAP_BASEADDR+0x114);
    } while (hwicap_wfv != HWICAP_WFIFO_DEPTH - 1);
}
