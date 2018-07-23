#include "main.h"

#define timing_pro_store 0

#define  LED2 PIN_B1  //CHANGE PIN_XX TO YOUR LED PIN NUMBER, EX: PIN_A5
#define  program_pin PIN_B0
#define  relay PIN_B3
//===========================
int8 main_st=0;
int1 flag_h=0;
int1 flag_l=0;
int8 enable_direct_a=0,enable_direct_b=0;
unsigned int16 count1=0,count2=0;
int16 count_msec=0,count_s=0;
int8 timing_pro=0;
//===========================
#int_TIMER0
void  TIMER0_isr(void) 
{
   count_msec++;
   if(count_s>=5+timing_pro)
   {
      output_toggle(relay); 
      count_s=0;
   }
   if(count_msec>=1000) // 1second
   {
      //output_toggle(PIN_A0);
      count_msec=0;
      count_s++;
      output_low(relay);
   }
}
//===========================
#INT_EXT
void timing_setting(void)
{
   disable_interrupts(INT_TIMER0);
   delay_us(100);
   if(input(program_pin)==1)
   {
      //output_toggle(relay);
      timing_pro++;
      if(timing_pro>=15)timing_pro=0;
      write_eeprom (timing_pro_store,timing_pro);
      while(input(program_pin)==1);
      count_s=0;
   }
   enable_interrupts(INT_TIMER0);
}
//===========================
void main()
{
   int1 i,j;
   output_high(relay);
   timing_pro=read_eeprom(timing_pro_store);
   if(timing_pro>60)
   {
      write_eeprom (timing_pro_store,0);
   }
   //port_a_pullups (0xff);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_8|RTCC_8_bit);      //1.0 ms overflow
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_EXT_L2H);
   enable_interrupts(GLOBAL);
   setup_oscillator(OSC_8MHZ|OSC_INTRC);
   //set_pwm1_duty(1);
  
     //i = input(PIN_B3); 
      //j = input(PIN_A6); 
    while(true)
    {
      //output_toggle(PIN_B3); 
      //delay_ms(1000);
    }

}
