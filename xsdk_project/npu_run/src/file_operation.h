
#ifndef FILE_OPERATION_H_
#define FILE_OPERATION_H_

int MountSD();

int ReadWeightASCII(unsigned int weightStartAddr, int weightDepth, char* fileName);

int ReadInstASCII(unsigned int instStartAddr, int instDepth, char* fileName);

int ReadIndexASCII(unsigned int indexStartAddr, int indexDepth, char* fileName);

int ReadBiasASCII(unsigned int biasStartAddr, int biasDepth, char* fileName);

int ReadDataASCII(unsigned int dataStartAddr, int dataDepth, char* fileName);

int ReadLutASCII(unsigned int lutStartAddr, int lutDepth, char* fileName);

int WriteWatchdogError(int dataNo);

int YoloReadInputData(int dataNo);

int ResnetReadInputData(int dataNo);

int DCGANReadInputData(int dataNo);

int LSTMReadInputData(int dataNo);

int YoloResult2SD(int dataNo);

int ResnetResult2SD(int dataNo);

int DCGANResult2SD(int dataNo);

int LSTMResult2SD(int dataNo);

#endif  //FILE_OPERATION_H_
