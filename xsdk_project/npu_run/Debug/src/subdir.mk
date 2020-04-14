################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/bram_mask.c \
../src/char2int.c \
../src/error_inject.c \
../src/file_operation.c \
../src/helloworld.c \
../src/hwicap.c \
../src/npu_op.c \
../src/platform.c 

OBJS += \
./src/bram_mask.o \
./src/char2int.o \
./src/error_inject.o \
./src/file_operation.o \
./src/helloworld.o \
./src/hwicap.o \
./src/npu_op.o \
./src/platform.o 

C_DEPS += \
./src/bram_mask.d \
./src/char2int.d \
./src/error_inject.d \
./src/file_operation.d \
./src/helloworld.d \
./src/hwicap.d \
./src/npu_op.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../zc706_npu_fault_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


