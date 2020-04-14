

#include "file_operation.h"

#include "Addr_PAR.h"
#include "char2int.h"

#include "ff.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xstatus.h"
#include <stdio.h>

static FATFS FS_instance;

int MountSD() {
    FRESULT rc;

    rc = f_mount(&FS_instance, "", 1);
    if (rc) {
        xil_printf("f_mount failed: %d\n", rc);
        return XST_FAILURE;
    }
    return 0;
}

int ReadWeightASCII(unsigned int weightStartAddr, int weightDepth, char* fileName) {
    FRESULT rc;
    char *Buffer_Rlogger = (char *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;

    rc = f_open(&file, fileName, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for(int line = 0; line < weightDepth; line++) {
        rc = f_read(&file, Buffer_Rlogger, 33, &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Buffer_Rlogger[32] = '\0';
        u32 weigth[4] = {0};
        charArray2intArray(Buffer_Rlogger, weigth);
        for (int i = 0; i < 4; i++) {
            Xil_Out32(weightStartAddr + i * 4 + line * 16, weigth[3 - i]);
        }
    }

    rc = f_close(&file);
    if (rc != 0) {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }

    xil_printf("SD TO DDR: WEIGHT OK~\r\n");
    return 0;
}

int ReadInstASCII(unsigned int instStartAddr, int instDepth, char* fileName) {
    FRESULT rc;
    char *Buffer_Rlogger = (char *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;

    rc = f_open(&file, fileName, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    int counter = 0;
    for(int line = 0; line < instDepth; line++) {
        counter++;
        rc = f_read(&file, Buffer_Rlogger, 33, &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Buffer_Rlogger[32] = '\0';
        u32 inst[4] = {0};
        charArray2intArray(Buffer_Rlogger, inst);
        for(int i = 0; i < 4; i++) {
            Xil_Out32(instStartAddr + i * 4 + line * 16, inst[3 - i]);
        }
    }

    rc = f_close(&file);
    if (rc!=0) {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }

    xil_printf("SD TO DDR: INSTRUCTION OK~\r\n");
    return 0;
}

int ReadIndexASCII(unsigned int indexStartAddr, int indexDepth, char* fileName) {
    FRESULT rc;
    char *Buffer_Rlogger = (char *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;

    rc = f_open(&file, fileName, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for (int line = 0; line < indexDepth; line++) {
        rc = f_read(&file, Buffer_Rlogger, 20, &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Buffer_Rlogger[19] = '\0';
        int index = 0;

        index = char2int(Buffer_Rlogger);
        Xil_Out32(indexStartAddr + line * 4, index);

    }

    rc = f_close(&file);
    if (rc != 0) {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }

    xil_printf("SD TO DDR: WEIGHT_INDEX OK~\r\n");
    return 0;
}

int ReadBiasASCII(unsigned int biasStartAddr, int biasDepth, char* fileName) {
    FRESULT rc;
    char *Buffer_Rlogger = (char *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;

    rc = f_open(&file, fileName, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if(rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for(int line = 0; line < biasDepth; line++) {
        rc = f_read(&file, Buffer_Rlogger, (512/4 + 1), &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Buffer_Rlogger[512/4] = '\0';
        u32 ibias[(512/4)/8] = {0};
        charArray2intArray(Buffer_Rlogger, ibias);
        for(int i = 0; i < (512/4)/8; i++) {
            Xil_Out32(biasStartAddr + i * 4 + line * (512/8), ibias[(512/4)/8 -1 - i]);
        }
    }

    rc = f_close(&file);
    if (rc != 0) {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }

    xil_printf("SD TO DDR: BIAS OK~\r\n");
    return 0;
}

int ReadDataASCII(unsigned int dataStartAddr, int dataDepth, char* fileName) {
    FRESULT rc;
    char *Buffer_Rlogger = (char *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;

    rc = f_open(&file, fileName, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for(int line = 0; line < dataDepth; line++) {
        rc = f_read(&file, Buffer_Rlogger, 65, &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Buffer_Rlogger[64] = '\0';
        u32 idata[8] = {0};
        charArray2intArray(Buffer_Rlogger, idata);
        for (int i = 0; i < 8; i++) {
            Xil_Out32(dataStartAddr + i * 4 + line * 32, idata[7 - i]);
        }
    }

    rc = f_close(&file);
    if (rc != 0)
    {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("SD TO DDR: INPUTDATA OK~\r\n");
    return 0;
}

int ReadLutASCII(unsigned int lutStartAddr, int lutDepth, char* fileName) {
    FRESULT rc;
    char *Buffer_Rlogger = (char *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;

    rc = f_open(&file, fileName, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for(int line = 0; line < lutDepth; line++) {
        rc = f_read(&file, Buffer_Rlogger, 9, &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Buffer_Rlogger[8] = '\0';
        u32 idata[1] = {0};
        charArray2intArray(Buffer_Rlogger, idata);
        Xil_Out32(lutStartAddr + line * 4, idata[0]);
    }

    rc = f_close(&file);
    if (rc != 0) {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }

    xil_printf("SD TO DDR: LUT OK~\r\n");
    return 0;
}

int WriteWatchdogError(int dataNo) {
    FIL file;
    UINT bw;
    FRESULT rc;

    char *Output_File = "watchdog_errorinfo.txt";
    char dataNo_str[20];

    rc = f_open(&file, Output_File, FA_OPEN_ALWAYS | FA_WRITE);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, file.fsize);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    sprintf(dataNo_str, "%d\n\0", dataNo);
    rc = f_write(&file, (void*)dataNo_str, strlen(dataNo_str), &bw);
    if (rc) {
        xil_printf("ERROR : f_write returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_close(&file);
    if (rc) {
        xil_printf("ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }

    xil_printf("Watchdog: npu run test %d time out\r\n", dataNo);
    return XST_SUCCESS;
}

int YoloReadInputData(int dataNo) {
    FRESULT rc;
    u32 *Buffer_Rlogger = (u32 *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;
    char Log_File[20];

    sprintf(Log_File, "data/data_%d\0", dataNo);

    rc = f_open(&file, Log_File, FA_READ);
    if (rc)
    {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for(int line = 0; line < DATA_DEPTH; line++)
    {
        rc = f_read(&file, Buffer_Rlogger, 3, &br);
        if (rc)
        {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }
        *Buffer_Rlogger = *Buffer_Rlogger << 8;

        for (int i = 0; i < 7; i++){
            Xil_Out32(DATA_ADDR_START + i * 4 + line * 32, 0);
        }
        Xil_Out32(DATA_ADDR_START + 28 + line * 32, *Buffer_Rlogger);
    }

    rc = f_close(&file);
    if (rc != 0)
    {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("SD TO DDR: INPUTDATA OK~\r\n");

    return 0;
}

int ResnetReadInputData(int dataNo) {
    FRESULT rc;
    u32 *Buffer_Rlogger = (u32 *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;
    char Log_File[20];

    sprintf(Log_File, "data/data_%d\0", dataNo);

    rc = f_open(&file, Log_File, FA_READ);
    if (rc)
    {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    for (int i = 0; i < 224*224*3/4; i++) {
        rc = f_read(&file, Buffer_Rlogger, 4, &br);
        if (rc)
        {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        Xil_Out32(DATA_ADDR_START + i * 4, *Buffer_Rlogger);
    }

    rc = f_close(&file);
    if (rc != 0)
    {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("SD TO DDR: INPUTDATA OK~\r\n");

    return 0;
}

int DCGANReadInputData(int dataNo) {
    FRESULT rc;
    u32 *Buffer_Rlogger = (u32 *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;
    char Log_File[20];

    sprintf(Log_File, "data/data_%d\0", dataNo);

    rc = f_open(&file, Log_File, FA_READ);
    if (rc)
    {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    unsigned char tmpchar[4];
    unsigned char tmpint[4];
    for (int i = 0; i < 4; i++) {
        for (int j=0; j<32/4; j++) {
            rc = f_read(&file, tmpchar, 4, &br);
            if (rc)
            {
                xil_printf("ERROR : f_read returned %d\r\n", rc);
                return XST_FAILURE;
            }
            tmpint[3] = tmpchar[0];
            tmpint[2] = tmpchar[1];
            tmpint[1] = tmpchar[2];
            tmpint[0] = tmpchar[3];

            Xil_Out32(DATA_ADDR_START + i * 8 * 4 + 28 - j * 4, *(unsigned int*)tmpint);
        }
    }

    rc = f_close(&file);
    if (rc != 0)
    {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("SD TO DDR: INPUTDATA OK~\r\n");

    return 0;
}

int LSTMReadInputData(int dataNo) {
    FRESULT rc;
    u32 *Buffer_Rlogger = (u32 *)Ddr3StoreBaseAddr;
    UINT br;
    FIL file;
    char Log_File[20];

    sprintf(Log_File, "data/%d\0", dataNo);

    rc = f_open(&file, Log_File, FA_READ);
    if (rc) {
        xil_printf("ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&file, 0);
    if (rc) {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    unsigned char tmpchar[40];
    unsigned char tmpint[4];
    for (int j=0; j<64/4; j++) {
        Xil_Out32(DATA_ADDR_START + j * 4, 0);
    }
    for (int i = 1; i <= 101; i++) {
        rc = f_read(&file, tmpchar, 40, &br);
        if (rc) {
            xil_printf("ERROR : f_read returned %d\r\n", rc);
            return XST_FAILURE;
        }

        for (int j=0; j<32/4; j++) {
            tmpint[3] = tmpchar[4*j+0];
            tmpint[2] = tmpchar[4*j+1];
            tmpint[1] = tmpchar[4*j+2];
            tmpint[0] = tmpchar[4*j+3];

            Xil_Out32(DATA_ADDR_START + i * 8 * 4 * 2 + 28 - j * 4, *(unsigned int*)tmpint);
        }
        for (int j=8; j<40/4; j++) {
            tmpint[3] = tmpchar[4*j+0];
            tmpint[2] = tmpchar[4*j+1];
            tmpint[1] = tmpchar[4*j+2];
            tmpint[0] = tmpchar[4*j+3];

            Xil_Out32(DATA_ADDR_START + i * 8 * 4 * 2 + 60 - j * 4 + 32, *(unsigned int*)tmpint);
        }
        for (int j=10; j<64/4; j++) {
            Xil_Out32(DATA_ADDR_START + i * 8 * 4 * 2 + 60 - j * 4 + 32, 0);
        }
    }

    rc = f_close(&file);
    if (rc != 0) {
        xil_printf("f_close failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("SD TO DDR: INPUTDATA OK~\r\n");

    return 0;
}

int YoloResult2SD(int dataNo) {
    FIL file;
    UINT bw;
    FRESULT rc;

    char Output_File[30];
    sprintf(Output_File, "output/%d\0", dataNo);

    rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
    if (rc) {
        if (rc==5) {
            xil_printf("no OUTPUT dir, mkdir...\r\n", rc);
            char *output_dir = "output";
            rc = f_mkdir(output_dir);
            if (rc) {
                xil_printf("f_mkdir failed: %d\n", rc);
                return XST_FAILURE;
            }
            rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
            if (rc) {
                xil_printf("ERROR : f_open returned %d\r\n", rc);
                return XST_FAILURE;
            }
        }
        else {
            xil_printf("ERROR : f_open returned %d\r\n", rc);
            return XST_FAILURE;
        }
    }

    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    u32 addr = NPU_CHKDATA_RSLT;
    for(int i=0; i<RSLT_LINE; i++) {
        u8 data[32];
        char data_ascii[64];
        for(int j = 0; j < 32; j++) {
             data[31 - j] = Xil_In8(addr++);
        }

        for(int j = 0; j < 32; j++){
             sprintf(data_ascii+2*j, "%02X", data[j]);
        }

        rc = f_write(&file, (void*)data_ascii, sizeof(data_ascii), &bw);
        if (rc) {
            xil_printf("ERROR : f_write returned %d\r\n", rc);
            return XST_FAILURE;
        }

        rc = f_write(&file, (void*)"\n", 1, &bw);
        if (rc) {
            xil_printf("ERROR : f_write returned %d\r\n", rc);
            return XST_FAILURE;
        }
    }

    rc = f_close(&file);
    if (rc) {
        xil_printf("ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }

    xil_printf("output data write to output/%d\r\n", dataNo);
    return XST_SUCCESS;
}

int ResnetResult2SD(int dataNo) {
    FIL file;
    UINT bw;
    FRESULT rc;

    char Output_File[30];
    sprintf(Output_File, "output/%d\0", dataNo);

    rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
    if (rc) {
        if (rc==5) {
            xil_printf("no OUTPUT dir, mkdir...\r\n", rc);
            char *output_dir = "output";
            rc = f_mkdir(output_dir);
            if (rc) {
                xil_printf("f_mkdir failed: %d\n", rc);
                return XST_FAILURE;
            }
            rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
            if (rc) {
                xil_printf("ERROR : f_open returned %d\r\n", rc);
                return XST_FAILURE;
            }
        }
        else {
            xil_printf("ERROR : f_open returned %d\r\n", rc);
            return XST_FAILURE;
        }
    }

    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    u32 addr = NPU_CHKDATA_RSLT;

    u8 data[15];
    char data_ascii[30];
    for(int i = 0; i < 15; i++) {
         data[i] = Xil_In8(addr++);
    }

    for(int i = 0; i < 15; i++){
         sprintf(data_ascii+2*i, "%02X", data[i]);
    }

    rc = f_write(&file, (void*)data_ascii, sizeof(data_ascii), &bw);
    if (rc) {
        xil_printf("ERROR : f_write returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_write(&file, (void*)"\n", 1, &bw);
    if (rc) {
        xil_printf("ERROR : f_write returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_close(&file);
    if (rc) {
        xil_printf("ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }

    xil_printf("output data write to output/%d\r\n", dataNo);
    return XST_SUCCESS;
}

int DCGANResult2SD(int dataNo) {
    FIL file;
    UINT bw;
    FRESULT rc;

    char Output_File[30];
    sprintf(Output_File, "output/%d\0", dataNo);

    rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
    if (rc) {
        if (rc==5) {
            xil_printf("no OUTPUT dir, mkdir...\r\n", rc);
            char *output_dir = "output";
            rc = f_mkdir(output_dir);
            if (rc) {
                xil_printf("f_mkdir failed: %d\n", rc);
                return XST_FAILURE;
            }
            rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
            if (rc) {
                xil_printf("ERROR : f_open returned %d\r\n", rc);
                return XST_FAILURE;
            }
        }
        else {
            xil_printf("ERROR : f_open returned %d\r\n", rc);
            return XST_FAILURE;
        }
    }

    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    u32 addr = NPU_CHKDATA_RSLT;
    for(int i=0; i<RSLT_LINE; i++) {
        u8 data[32];
        char data_ascii[64];
        for(int j = 0; j < 32; j++) {
             data[31 - j] = Xil_In8(addr++);
        }

        for(int j = 0; j < 32; j++){
             sprintf(data_ascii+2*j, "%02X", data[j]);
        }

        rc = f_write(&file, (void*)data_ascii, sizeof(data_ascii), &bw);
        if (rc) {
            xil_printf("ERROR : f_write returned %d\r\n", rc);
            return XST_FAILURE;
        }

        rc = f_write(&file, (void*)"\n", 1, &bw);
        if (rc) {
            xil_printf("ERROR : f_write returned %d\r\n", rc);
            return XST_FAILURE;
        }
    }

    rc = f_close(&file);
    if (rc) {
        xil_printf("ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }

    xil_printf("output data write to output/%d\r\n", dataNo);
    return XST_SUCCESS;
}

int LSTMResult2SD(int dataNo) {
    FIL file;
    UINT bw;
    FRESULT rc;

    char Output_File[30];
    sprintf(Output_File, "output/%d\0", dataNo);

    rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
    if (rc) {
        if (rc==5) {
            xil_printf("no OUTPUT dir, mkdir...\r\n", rc);
            char *output_dir = "output";
            rc = f_mkdir(output_dir);
            if (rc) {
                xil_printf("f_mkdir failed: %d\n", rc);
                return XST_FAILURE;
            }
            rc = f_open(&file, Output_File, FA_CREATE_ALWAYS | FA_WRITE);
            if (rc) {
                xil_printf("ERROR : f_open returned %d\r\n", rc);
                return XST_FAILURE;
            }
        }
        else {
            xil_printf("ERROR : f_open returned %d\r\n", rc);
            return XST_FAILURE;
        }
    }

    rc = f_lseek(&file, 0);
    if (rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    u32 addr = NPU_CHKDATA_RSLT;

    u8 data[15];
    char data_ascii[30];
    for(int i = 0; i < 15; i++) {
         data[i] = Xil_In8(addr++);
    }

    for(int i = 0; i < 15; i++){
         sprintf(data_ascii+2*i, "%02X", data[i]);
    }

    rc = f_write(&file, (void*)data_ascii, sizeof(data_ascii), &bw);
    if (rc) {
        xil_printf("ERROR : f_write returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_write(&file, (void*)"\n", 1, &bw);
    if (rc) {
        xil_printf("ERROR : f_write returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_close(&file);
    if (rc) {
        xil_printf("ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }

    xil_printf("output data write to output/%d\r\n", dataNo);
    return XST_SUCCESS;
}

