
#include "test_params.h"

#include <stdio.h>
#include "platform.h"
#include "xparameters_ps.h"
#include "xsdps.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xil_cache.h"
#include "sleep.h"
#include "time.h"
#include "xscutimer.h"

#include <math.h>
#include <malloc.h>
#include "xdebug.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "Addr_PAR.h"
#include "file_operation.h"
#include "npu_op.h"

#include "xscugic.h"
#include "xil_exception.h"
#include "xil_assert.h"
#include "xil_types.h"
#include "xstatus.h"


#include <stdlib.h>
#include "xtime_l.h"

#include "error_inject.h"
#include "hwicap.h"


#define TIMER_DEVICE_ID		    XPAR_SCUTIMER_DEVICE_ID

#define INTC_DEVICE_INT_ID	  	XPAR_FABRIC_NPU_TOP_0_DMA_INTO_INTR
#define DMA_FAULT_INTR			XPAR_XDMAPS_0_FAULT_INTR
#define TIMER_IRPT_INTR		    XPAR_SCUTIMER_INTR
#define TIMER_LOAD_VALUE	    60000000
#define INTC_DEVICE_INT_ID0 	61U
#define INTC_DEVICE_INT_ID1 	62

#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define GPIO_INTERRUPT_ID	XPAR_XGPIOPS_0_INTR

#define GPIO_BANK	XGPIOPS_BANK0


volatile int InterruptProcessed = FALSE;

int Const_Num=1;

XScuGic InterruptController;
XScuGic_Config *GicConfig;

#define HERE do{xil_printf("On line %d, at file %s\r\n", __LINE__, __FILE__);} while(0);

unsigned int g_Idcode;

int main()
{
    init_platform();
    xil_printf("---zc706 DNN Fault Analysis---\r\n");

  #ifdef RANDOM_SEED
    XTime tCur;
    XTime_GetTime(&tCur);
    unsigned int seed = (unsigned int) tCur;
  #endif

    Xil_DCacheDisable();

    MountSD();
    ReadWeightASCII(WEIGHT_ADDR_START, WEIGHT_DEPTH, "weight");
    ReadInstASCII(INST_ADDR_START, INST_DEPTH, "inst");
    ReadIndexASCII(INDEX_ADDR_START, INDEX_DEPTH, "index");
    ReadBiasASCII(BIAS_ADDR_START, BIAS_DEPTH, "bias");
    #if defined(LSTM) || defined(DCGAN)
        ReadLutASCII(LUT_ADDR_START, LUT_DEPTH, "lut");
    #endif
    //ReadDataASCII(DATA_ADDR_START, DATA_DEPTH, "data");
    #ifdef LSTM
        ReadInstASCII(INSTFC_ADDR, INSTFC_DEPTH, "inst_fc");
    #endif

  #ifdef RANDOM_SEED
    XTime_GetTime(&tCur);
    seed *= (unsigned int) tCur;
  #else
    unsigned int seed = ERROR_INJECT_SEED;
  #endif
    srand(seed);
    xil_printf("seed=%u\r\n", seed);

  #ifdef ERROR_INJECT_SEED
    int generated_random = (START_NUM-1) * 7 * ERROR_NUMBER;
    for (int i=0; i<generated_random; i++) {
        rand();
    }
  #endif

    PCAP2ICAP();
    HwicapSync();
    g_Idcode = HwicapReadIdcode();

    Xil_Out32(REG_BASE_ADDR + 0x04,0x7EF);
    Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFF);

    int xStatus;
    xStatus = ScuGicExample(INTC_DEVICE_ID);
    if (xStatus != XST_SUCCESS) {
        xil_printf("GIC Example Test Failed\r\n");
        return XST_FAILURE;
    }

    for (int i=START_NUM; i<=PICTRUE_NUM;i++) {
        int reload = 0;
        InterruptProcessed = FALSE;
        Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFFFFFFF);

        xil_printf("------test %3d------\r\n", i);

        CfgMemSEUInfo ConfigMemError[ERROR_NUMBER];
        BramSEUInfo BramError[ERROR_NUMBER];
        int cfgMemErrorNum;
        int bramErrorNum;

        RandomSEU(ERROR_NUMBER, ConfigMemError, &cfgMemErrorNum, BramError, &bramErrorNum);

        for (int i_inject=0; i_inject<bramErrorNum; i_inject++) {
            BramSingleBitUpsetInject(BramError[i_inject]);
        }
        for (int i_inject=0; i_inject<cfgMemErrorNum; i_inject++) {
            ConfigMemSingleBitUpsetInject(ConfigMemError[i_inject]);
        }

        xil_printf("errors injected\r\n");

        #ifdef LSTM
            LSTMReadInputData((i%2000)?(i%2000):2000);
        #elif defined DCGAN
            DCGANReadInputData((i%2000)?(i%2000):2000);
        #elif defined YOLO
            YoloReadInputData((i%2000)?(i%2000):2000);
        #elif defined RESNET
            ResnetReadInputData((i%2000)?(i%2000):2000);
        #endif

		volatile int watchdog = 0;
		volatile int inst_succ = 0;

		Inst_Init_Config(INST_ADDR_START, INST_DEPTH);
		xil_printf("starting configure init inst_buffer\r\n");
		while(1) {
			watchdog++;
			if(InterruptProcessed) {
				inst_succ = 1;
				break;
			}
			if (watchdog > 100000000) {
				WriteWatchdogError(i);
				reload = 1;
				break;
			}
		}
		InterruptProcessed = FALSE;
		Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFFFFFFF);

		if (inst_succ) {
            Xil_Out32(REG_BASE_ADDR,1);
            xil_printf("starting npu process\r\n");
		}

        watchdog = 0;
        while(inst_succ) {
            watchdog++;
            if(InterruptProcessed) {
                #ifdef LSTM
                    //
                #elif defined DCGAN
                    DCGANResult2SD(i);
                #elif defined YOLO
                    YoloResult2SD(i);
                #elif defined RESNET
                    ResnetResult2SD(i);
                #endif

                break;
            }
            if (watchdog > 100000000) {
                WriteWatchdogError(i);
                reload = 1;
                break;
            }
        }
        InterruptProcessed = FALSE;
        Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFFFFFFF);

        #ifdef LSTM
            if(!reload) {
                unsigned int buf_tmp[6];
                unsigned int buffer[256] = {0};

                unsigned int *p = buffer;

                for (int j=0; j<50; j++) {
                    buf_tmp[0] = Xil_In32(DATA_OUT_ADDR + 64 * j + 0x1C);
                    buf_tmp[1] = Xil_In32(DATA_OUT_ADDR + 64 * j + 0x18);
                    buf_tmp[2] = Xil_In32(DATA_OUT_ADDR + 64 * j + 0x14);

                    buf_tmp[3] = Xil_In32(DATA_OUT_ADDR + 64 * j + 0x3C);
                    buf_tmp[4] = Xil_In32(DATA_OUT_ADDR + 64 * j + 0x38);
                    buf_tmp[5] = Xil_In32(DATA_OUT_ADDR + 64 * j + 0x34);

                    *(p++) = buf_tmp[0];
                    *(p++) = buf_tmp[1];
                    *(p++) = (buf_tmp[2] & 0xFFFF0000) | (buf_tmp[3] >> 16);
                    *(p++) = (buf_tmp[3] << 16) | (buf_tmp[4] >> 16);
                    *(p++) = (buf_tmp[4] << 16) | (buf_tmp[5] >> 16);
                }

                unsigned int data_fc_addr = DATA_FC_ADDR;
                for(int j=0; j<32; j++) {
                    for(int k=7; k>=0; k--) {
                        Xil_Out32(data_fc_addr, buffer[k + j*8]);
                        data_fc_addr += 4;
                    }
                }

                Inst_Init_Config(INSTFC_ADDR, INSTFC_DEPTH);
                while(1) {
                    if(InterruptProcessed) {
                        break;
                    }
                }
                InterruptProcessed = FALSE;
                Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFFFFFFF);

                Xil_Out32(REG_BASE_ADDR,1);

                watchdog = 0;
                while(1) {
                    watchdog++;
                    if(InterruptProcessed) {
                        LSTMResult2SD(i);
                        break;
                    }
                    if (watchdog > 1000) {
                        WriteWatchdogError(i);
                        reload = 1;
                        break;
                    }
                }
                InterruptProcessed = FALSE;
                Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFFFFFFF);
            }
        #endif

        for (int i_inject=cfgMemErrorNum-1; i_inject>=0; i_inject--) {
            ConfigMemSingleBitUpsetInject(ConfigMemError[i_inject]);
        }
        ClearBramError();
        xil_printf("errors reset\r\n");

        if (reload) {
            Xil_Out32(0xF8000008, 0xDF0D);
            Xil_Out32(0xF8000240, 0x01);

            sleep(1);

            Xil_Out32(0xF8000240, 0x00);
            Xil_Out32(0xF8000004, 0x076B);

            xil_printf("FPGA reset.\r\n");
            sleep(1);

            Xil_Out32(REG_BASE_ADDR + 0x04,0x7EF);
            Xil_Out32(REG_BASE_ADDR + 0x0c,0xFFF);

            sleep(1);
        }
    }

    xil_printf("test end.\r\n");
    xil_printf("seed=%u\r\n", seed);
    xil_printf("--------------------------------\r\n");
    cleanup_platform();
    return 0;
}



