################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../NotificationMediator.cpp \
../TestObserver.cpp \
../TestSubject.cpp \
../main.cpp 

OBJS += \
./NotificationMediator.o \
./TestObserver.o \
./TestSubject.o \
./main.o 

CPP_DEPS += \
./NotificationMediator.d \
./TestObserver.d \
./TestSubject.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


