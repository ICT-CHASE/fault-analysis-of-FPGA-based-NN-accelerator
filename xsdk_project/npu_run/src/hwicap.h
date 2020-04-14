
#ifndef HWICAP_H_
#define HWICAP_H_

#define     HWICAP_BASEADDR         0x41000000
#define     HWICAP_WFIFO_DEPTH      256

typedef struct faraddrinfo{
    unsigned int BlockType; 
    unsigned int TopBottom; 
    unsigned int RowAddress;
    unsigned int ColumnAddress;
    unsigned int MinorAddress;
} FarAddrInfo;

void PCAP2ICAP();

void HwicapSync();

unsigned int HwicapReadIdcode();

unsigned int HwicapReadStatus();

void HwicapWriteCMD(unsigned int CMDCode);

unsigned int FarAddress(FarAddrInfo FarAddr);

void HwicapReadFrame(FarAddrInfo FarAddress, unsigned int* ReadResult);

void HwicapWriteFrame(FarAddrInfo FarAddress, unsigned int* WriteFrameData, unsigned int IDcode);

void HwicapDesync();

#endif
