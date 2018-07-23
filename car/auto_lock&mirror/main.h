#include <18F1220.h>

#device adc=8
#FUSES NOWDT,INTRC_IO,NOPUT,NOPROTECT,NODEBUG,NOBROWNOUT,NOLVP,NOCPD,NOMCLR//18F1220
#use delay(clock=8000000)
#use rs232(baud=9600,parity=N,xmit=PIN_A1,rcv=PIN_A0,bits=8,stream=DEBUG)
