################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/List.cpp \
../src/String.cpp \
../src/TD1.cpp \
../src/main.cpp 

CPP_DEPS += \
./src/List.d \
./src/String.d \
./src/TD1.d \
./src/main.d 

OBJS += \
./src/List.o \
./src/String.o \
./src/TD1.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/List.d ./src/List.o ./src/String.d ./src/String.o ./src/TD1.d ./src/TD1.o ./src/main.d ./src/main.o

.PHONY: clean-src

