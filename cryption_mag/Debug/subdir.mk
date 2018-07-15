################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../aes.c \
../eeprom.c \
../i2ceeprom.c \
../kb.c \
../magtek.c \
../main.c \
../rtc_ds1337.c \
../smalltools.c \
../twi.c \
../usart.c 

OBJS += \
./aes.o \
./eeprom.o \
./i2ceeprom.o \
./kb.o \
./magtek.o \
./main.o \
./rtc_ds1337.o \
./smalltools.o \
./twi.o \
./usart.o 

C_DEPS += \
./aes.d \
./eeprom.d \
./i2ceeprom.d \
./kb.d \
./magtek.d \
./main.d \
./rtc_ds1337.d \
./smalltools.d \
./twi.d \
./usart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


