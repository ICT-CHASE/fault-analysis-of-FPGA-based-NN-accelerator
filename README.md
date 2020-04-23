ZC706-DNNFaultAnalysis
========================
FPGA-based DNN fault analysis tool deployed on Xilinx ZC706 evaluation board.

# How to use

## Hardware connection

JTAG connection for programming and debugging FPGA.  
URAT for printing execution information.

## Xilinx SDK project

Import the three SDK projects in the xsdk_project directory to Xilinx SDK.

| Project name | Project property | Explanation |
|:-- |:-- |:-- |
| npu_run | Application Project | test program |
| zc706_npu_fault_bsp | Board Support Package | referenced BSP for npu_run |
| zc706_npu_fault_hw_platform | Hardware platform specification | generated FPGA hardware, hdf and bit file included |

## Test Parameters

Modify the test parameters defined in head files in project npu_run:

| File | Line Position | Parameter name | Explanation |
|:-- |:-- |:-- |:-- |
| addr_PAR.h | line 6 | DCGAN | modify this defination to change network application. Available options: YOLO, RESNET, LSTM, DCGAN. |
| test_params.h | line 4 | START_NUM | test start from input No.x |
| test_params.h | line 6 | PICTRUE_NUM | test end at input No.x |
| test_params.h | line 9 | ERROR_NUMBER | number of injected errors per run |
| test_params.h | line 12 | RANDOM_SEED | uncomment this line to use random seed for error injection |
| test_params.h | line 13 | ERROR_INJECT_SEED | define random seed in this line if not use random seed |

## Input files

Make sure the format of SD card is FAT or FAT32, and copy the input files to the root directory of the SD card.

## Output files

Output files are located in the output directory of SD card after execution finished.  
The watchdog_errorinfo.txt in the SD card's root directory records the number of which runs timeout.

## Result handle

Use corresponding tools for application to process and compare output. The detailed instructions is described in each corresponding tool.

----------------------------------------
