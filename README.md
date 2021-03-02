Persistent fault analysis on FPGA-based DNN acceleration system
========================
Prior works typically conducted the fault analysis of neural network accelerator computing arrays with simulation and focused on the prediction accuracy loss of the neural network models. There is still a lack of systematic fault analysis of the neural network acceleration system that considers both the accuracy degradation and system exceptions such as system stall and running over time. To that end, we implemented a representative neural network accelerator and corresponding fault injection modules on a Xilinx ARM-FPGA platform, and evaluated the reliability of the system under different fault injection rate when a series of typical neural network models are deployed on the neural network acceleration system. Currently, it targets at ZC706 board, but it should be able to ported to other zynq boards with minor modification. Details of the accelerator can be found in our open sourced deep learning accelerator project [DeepBurning][https://github.com/groupsada/DeepBurning.git].

# User Guide

1. Prepare the FPGA design suite Vivado 2018.2, hardware platform: ZC706 or other boards with ZC7045.
2. Import the three SDK projects in the xsdk_project directory to Xilinx SDK.

| Project name | Project property | Explanation |
|:-- |:-- |:-- |
| npu_run | Application Project | test program |
| zc706_npu_fault_bsp | Board Support Package | referenced BSP for npu_run |
| zc706_npu_fault_hw_platform | Hardware platform specification | generated FPGA hardware, hdf and bit file included |

3. You may modify the following parameters defined in head files in project npu_run for the fault analysis under various conditions such as the deployed neural network models, fault injection rate.

| File | Line Position | Parameter name | Explanation |
|:-- |:-- |:-- |:-- |
| addr_PAR.h | line 6 | DCGAN | modify this defination to change network application. Available options: YOLO, RESNET, LSTM, DCGAN. |
| test_params.h | line 4 | START_NUM | test start from input No.x |
| test_params.h | line 6 | PICTRUE_NUM | test end at input No.x |
| test_params.h | line 9 | ERROR_NUMBER | number of injected errors per run |
| test_params.h | line 12 | RANDOM_SEED | uncomment this line to use random seed for error injection |
| test_params.h | line 13 | ERROR_INJECT_SEED | define random seed in this line if not use random seed |

4. Copy input files to the root directory of the SD card. Make sure the format of SD card is FAT or FAT32.

5. Power on the board and it will boot from the SD card.

6. Check the output data/files located in the output directory of SD card after the execution. Note that the watchdog_errorinfo.txt in the SD card's root directory records the indices of the fault configurations which causes the timeout.

7. You may utilize the scripts for result analysis such as result comparison to the golden outputs.

# Reference
[1] Dawen Xu, Ziyang Zhu, Cheng Liu*, Ying Wang, Huawei Li, Lei Zhang, Kwang-Ting Cheng, Persistent Fault Analysis of Neural Networks on FPGA-based Acceleration System, The 31th IEEE International Conference on Application-specific Systems, Architectures and Processors (ASAP), July, 2020

[2] Dawen Xu, Ziyang Zhu, Cheng Liu*, Ying Wang, Shuang Zhao, Lei Zhang, Huaguo Liang, Huawei Li, Kwang-Ting Cheng, "Reliability Evaluation and Analysis of FPGA-based Neural Network Acceleration System", IEEE Transactions on Very Large Scale Integration (VLSI) Systems, 2021

----------------------------------------
