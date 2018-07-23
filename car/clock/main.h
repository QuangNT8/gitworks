//FUSES NOWDT, WDT128, PLL1, CPUDIV1, NOUSBDIV, INTRC_IO, NOBROWNOUT, NOLVP, NOXINST//#include <18F4431.h>
//#include <16F877a.h>
//#include <18F4455.h>
//#priority INT_EXT1,RDA,INT_TIMER0
#define dis_rev #ASM BCF    0xF94.7 #ENDASM
#define debug
#define debugkb
//#define PIC24
#define PIC26
//#define SIM900
//#define twonumber
//#define rst_pwd
//=========================
#if defined(PIC24)
   #include <18F24j11.h>
#else
   #include <18F26j11.h>
   //#include <18F46j11.h>
#endif
//=========================
//#include <18F26j11.h>
   //#device adc=16 
//#use STANDARD_IO(A)
//#FUSES NOWDT, INTRC_IO, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT,NOMCLR,NOUSBDIV,NOVREGEN, NOICPRT,NOPBADEN,NOICPRT
//#FUSES NOWDT, INTRC_IO, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOMCLR
//#FUSES NOWDT, WDT128, PLL1, CPUDIV1, NOUSBDIV, INTRC_IO, NOBROWNOUT, NOLVP, NOXINST,NOMCLR
#FUSES NOWDT, WDT128, NOXINST,INTRC_PLL_IO

#device ADC=8
#use delay(clock=32000000)
//#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=COM1)
//#use rs232(uart1, baud=9600)
#use rs232(baud=115200,parity=N,xmit=PIN_C0,rcv=PIN_C1,bits=8,stream=COM2)

