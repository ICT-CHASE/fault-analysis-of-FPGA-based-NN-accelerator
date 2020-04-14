
#include "char2int.h"

#include "xil_types.h"

int char2int(char* charArray)
{

    int i = 18;
    int temp = 0;
    int power;

    while (i >= 0) {
        if (i == 18) {
            power = 1;
        }
        else {
            power = power * 2;
        }

        if (*(charArray + i) == '1') {
            temp = temp + power;
        }
        else {
            temp = temp;
        }

        i--;
    }

    return temp;
}

void charArray2intArray(char* charArray, u32* intArray){

    int i = 0;
    int j = 0;
    u32 temp = 0;

    while (*(charArray + i) != '\0') {
        if (*(charArray + i) >= '0' && *(charArray + i) <='9') {
            temp = *(charArray + i) - '0';
        }
        else if(*(charArray + i) >= 'a' && *(charArray + i) <= 'f') {
            temp=*(charArray + i) - 'a' + 10;
        }
        else if(*(charArray + i) >= 'A' && *(charArray + i) <= 'F') {
            temp=*(charArray + i) - 'A' + 10;
        }

        *(intArray + j) |= temp << (28 - (4 * (i % 8)));
        if ((++i) % 8 == 0) {
            j++;
        }
    }
}
