
#include "npu_op.h"

#include "xscugic.h"
#include "xil_exception.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "xparameters.h"

#include "Addr_PAR.h"

#define INTC_DEVICE_INT_ID      XPAR_FABRIC_NPU_TOP_0_DMA_INTO_INTR
#define ICDICFR3                0xF8F01C0C

int Inst_Init_Config(unsigned int instStartAddr, int instDepth) {
    Xil_Out32(REG_BASE_ADDR + 0x34, instStartAddr);
    Xil_Out32(REG_BASE_ADDR + 0x3C, instDepth);

    Xil_Out32(REG_BASE_ADDR, 0x2104);
    
    return 0;
}

int ScuGicExample(u16 DeviceId) {
    int Status;

    GicConfig = XScuGic_LookupConfig(DeviceId);
    if (NULL == GicConfig) {
        return XST_FAILURE;
    }

    Status = XScuGic_CfgInitialize(&InterruptController, GicConfig,
                    GicConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Xil_Out32(ICDICFR3,0xFFFFFFFF);

    Status = XScuGic_SelfTest(&InterruptController);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }



    Status = SetUpInterruptSystem(&InterruptController);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }


    Status = XScuGic_Connect(&InterruptController, INTC_DEVICE_INT_ID,
               (Xil_ExceptionHandler)DeviceDriverHandler,
               (void *)&InterruptController);

    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }


    XScuGic_Enable(&InterruptController, INTC_DEVICE_INT_ID);

    return XST_SUCCESS;
}

int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr) {
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
        (Xil_ExceptionHandler) XScuGic_InterruptHandler,
        XScuGicInstancePtr);

    Xil_ExceptionEnable();

    return XST_SUCCESS;
}

void DeviceDriverHandler(void *CallbackRef) {
    extern int InterruptProcessed;
    InterruptProcessed = TRUE;
}

