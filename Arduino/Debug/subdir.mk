################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../eth.c \
../lcd.c \
../main.c \
../spi.c \
../tcp.c \
../tiltsensor.c \
../uart.c \
../udp.c 

OBJS += \
./eth.o \
./lcd.o \
./main.o \
./spi.o \
./tcp.o \
./tiltsensor.o \
./uart.o \
./udp.o 

C_DEPS += \
./eth.d \
./lcd.d \
./main.d \
./spi.d \
./tcp.d \
./tiltsensor.d \
./uart.d \
./udp.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


