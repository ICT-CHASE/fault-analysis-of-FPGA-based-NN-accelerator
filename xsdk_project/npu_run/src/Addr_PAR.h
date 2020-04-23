
#define Ddr3StoreBaseAddr 0x01000000

#define REG_BASE_ADDR 0x43C00000

#define DCGAN

#ifdef LSTM
    #define DATA_ADDR_START     0x30000000
    #define WEIGHT_ADDR_START   0x31500000
    #define INDEX_ADDR_START    0x34000000
    #define BIAS_ADDR_START     0x34500000
    #define INST_ADDR_START     0x20000000
    #define LUT_ADDR_START      0x36000000
    #define DATA_OUT_ADDR       0x35000000
    #define NPU_CHKDATA_RSLT    0x35000CA0
    #define DATA_DEPTH  101
    #define BIAS_DEPTH  9
    #define INST_DEPTH 3205
    #define INDEX_DEPTH 64
    #define WEIGHT_DEPTH 6162
    #define LUT_DEPTH 1536
    #define RSLT_LINE 1
    #define INSTFC_ADDR         0x2000C850
    #define INSTFC_DEPTH 8
    #define DATA_FC_ADDR        0x30001940
#elif defined DCGAN
    #define DATA_ADDR_START     0x32000000
    #define WEIGHT_ADDR_START   0x32500000
    #define INDEX_ADDR_START    0x34000000
    #define BIAS_ADDR_START     0x34500000
    #define LUT_ADDR_START      0x35000000
    #define CHECKATA_ADDR_START 0x36000000
    #define INST_ADDR_START     0x20000000
    #define NPU_CHKDATA_RSLT    0x3601c000
    #define DATA_DEPTH 4
    #define BIAS_DEPTH  519
    #define INST_DEPTH 100
    #define INDEX_DEPTH 3508
    #define WEIGHT_DEPTH 277470
    #define LUT_DEPTH 768
    #define RSLT_LINE 1024
#elif defined YOLO
    #define DATA_ADDR_START     0x10000000
    #define WEIGHT_ADDR_START   0x11500000
    #define INDEX_ADDR_START    0x14000000
    #define BIAS_ADDR_START     0x14500000
    #define CHECKATA_ADDR_START 0x15000000
    #define INST_ADDR_START     0x20000000
    #define NPU_CHKDATA_RSLT    0x16207d00
    #define NPU_CHKDATA_RSLT_TAIL 0x162184E0
    #define DATA_DEPTH 173056
    #define BIAS_DEPTH  327
    #define INST_DEPTH 2346
    #define INDEX_DEPTH 49464
    #define WEIGHT_DEPTH 1915002
    #define RSLT_LINE 676
#elif defined RESNET
    #define DATA_ADDR_START     0x32000000
    #define WEIGHT_ADDR_START   0x32500000
    #define INDEX_ADDR_START    0x34000000
    #define BIAS_ADDR_START     0x34500000
    #define CHECKATA_ADDR_START 0x35000000
    #define INST_ADDR_START     0x20000000
    #define NPU_CHKDATA_RSLT    0x352A1C00
    #define RSLT_LINE 1
    #define DATA_DEPTH 4704
    #define BIAS_DEPTH  1184
    #define INST_DEPTH 682
    #define INDEX_DEPTH 11444
    #define WEIGHT_DEPTH 663022
#endif
