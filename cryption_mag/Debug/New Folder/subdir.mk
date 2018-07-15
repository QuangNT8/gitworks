################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../New\ Folder/a.c \
../New\ Folder/aes.c \
../New\ Folder/eeprom.c \
../New\ Folder/i2ceeprom.c \
../New\ Folder/kb.c \
../New\ Folder/magtek.c \
../New\ Folder/main.c \
../New\ Folder/rtc_ds1337.c \
../New\ Folder/smalltools.c \
../New\ Folder/twi.c \
../New\ Folder/usart.c 

ASM_SRCS += \
../New\ Folder/a.asm 

OBJS += \
./New\ Folder/a.o \
./New\ Folder/aes.o \
./New\ Folder/eeprom.o \
./New\ Folder/i2ceeprom.o \
./New\ Folder/kb.o \
./New\ Folder/magtek.o \
./New\ Folder/main.o \
./New\ Folder/rtc_ds1337.o \
./New\ Folder/smalltools.o \
./New\ Folder/twi.o \
./New\ Folder/usart.o 

C_DEPS += \
./New\ Folder/a.d \
./New\ Folder/aes.d \
./New\ Folder/eeprom.d \
./New\ Folder/i2ceeprom.d \
./New\ Folder/kb.d \
./New\ Folder/magtek.d \
./New\ Folder/main.d \
./New\ Folder/rtc_ds1337.d \
./New\ Folder/smalltools.d \
./New\ Folder/twi.d \
./New\ Folder/usart.d 

ASM_DEPS += \
./New\ Folder/a.d 


# Each subdirectory must supply rules for building sources it contributes
New\ Folder/a.o: ../New\ Folder/a.asm
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -g2 -gstabs -mmcu=atmega16 -MMD -MP -MF"New Folder/a.d" -MT"New\ Folder/a.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/a.o: ../New\ Folder/a.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/a.d" -MT"New\ Folder/a.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/aes.o: ../New\ Folder/aes.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/aes.d" -MT"New\ Folder/aes.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/eeprom.o: ../New\ Folder/eeprom.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/eeprom.d" -MT"New\ Folder/eeprom.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/i2ceeprom.o: ../New\ Folder/i2ceeprom.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/i2ceeprom.d" -MT"New\ Folder/i2ceeprom.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/kb.o: ../New\ Folder/kb.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/kb.d" -MT"New\ Folder/kb.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/magtek.o: ../New\ Folder/magtek.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/magtek.d" -MT"New\ Folder/magtek.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/main.o: ../New\ Folder/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/main.d" -MT"New\ Folder/main.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/rtc_ds1337.o: ../New\ Folder/rtc_ds1337.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/rtc_ds1337.d" -MT"New\ Folder/rtc_ds1337.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/smalltools.o: ../New\ Folder/smalltools.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/smalltools.d" -MT"New\ Folder/smalltools.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/twi.o: ../New\ Folder/twi.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/twi.d" -MT"New\ Folder/twi.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

New\ Folder/usart.o: ../New\ Folder/usart.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"New Folder/usart.d" -MT"New\ Folder/usart.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


