################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

CPP_SRCS += \
../src/convolution.cpp \
../src/fullyconnect.cpp \
../src/load.cpp \
../src/main.cpp \
../src/pool.cpp 

OBJS += \
./src/convolution.o \
./src/fullyconnect.o \
./src/load.o \
./src/main.o \
./src/pool.o 

CPP_DEPS += \
./src/convolution.d \
./src/fullyconnect.d \
./src/load.d \
./src/main.d \
./src/pool.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 g++ compiler'
	arm-none-eabi-g++ -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../standalone_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


