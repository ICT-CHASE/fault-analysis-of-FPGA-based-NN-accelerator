
#ifndef NPU_OP_H_
#define NPU_OP_H_

#include "xscugic.h"
#include "xil_types.h"

extern XScuGic InterruptController;
extern XScuGic_Config *GicConfig;

int Inst_Init_Config();

int ScuGicExample(u16 DeviceId);

int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr);

void DeviceDriverHandler(void *CallbackRef);

#endif  //NPU_OP_H_