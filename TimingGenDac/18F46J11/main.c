
#define LED   PIN_A6
#include "main.h"
#include "stdlib.h"


//=========================
#INT_TIMER1
void timer1()
{
   //output_toggle(LED);
   output_toggle(PIN_B5);
   //set_timer1(0xffff-10);
}
//==========================
#INT_TIMER0
void timer0()
{
  //=================================
  output_toggle(PIN_B5);
  set_timer0(0xff-2);
  //=================================
}
//=========================
void main()
{ 
   //uint tmp;
   RPINR4=11;
   setup_oscillator(OSC_32MHZ);

   //=============================
   disable_interrupts(GLOBAL);
   
   delay_ms(1000);
   //setup_timer_1(T1_EXTERNAL|T1_DIV_BY_1);
   //setup_counters(RTCC_EXT_L_TO_H|RTCC_8_BIT,RTCC_DIV_1);
   setup_timer_0(RTCC_EXT_L_TO_H|RTCC_8_BIT|RTCC_DIV_1);
    //==========================================
   enable_interrupts(INT_RTCC);//INT_RTCC //INT_TIMER1

      
   enable_interrupts(GLOBAL);
   //==========================================
   //set_timer1(100);
   set_timer0(0xff-2);
   
   while(1)
   {
      /*
      while(get_timer0()<6)
      {
      
      }
      #asm
      nop
      #endasm
      output_toggle(PIN_B5);
      set_rtcc(0);
      */
     // delay_ms(1);
   }
}
