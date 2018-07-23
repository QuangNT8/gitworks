#include "main.h"

#define timing_pro_store 0

#define  LED         PIN_B5 
#define  relay1       PIN_B0
#define  relay2       PIN_A1
#define  disable_blink  disable_interrupts(INT_TIMER0)
#define  enable_blink   enable_interrupts(INT_TIMER0)
//===========================
int8 main_st=0,VR=0,SS1=0,adc2;
int16 led_fre=100;
unsigned int16 count1=0,count2=0;
int16 count_msec=0,count_s=0;
int8 timing_pro=0;
//===========================
#int_TIMER0
void  TIMER0_isr(void) 
{
   char temp=0;
   count_msec++;
   temp=VR-SS1;  
   if(count_msec>=led_fre) // 1second
   {
      //
      output_toggle(LED);
      count_msec=0;
      count_s++;
   }
}
//===========================
int8 get_sample(int8 ch,int32 numsam)
{
   int8 result;
   int32 i;
   int32 temp=0;
   set_adc_channel(ch);
   for(i=0;i<numsam;i++)
   {
      temp=read_adc()+temp;
   }
   result=(int8)(temp/numsam);
   return result;
}
//=========================
int8 get_refer(int8 numsam)
{
   int8 i,result;
   int32 temp=0;
   set_adc_channel(2);
   for(i=0;i<numsam;i++)
   {
      temp=read_adc()+temp;
   }
   result=(int8)(temp/numsam);
   return result;
}
//===========================
void main()
{
   unsigned char i,j,temp;
   int16 count_off=0,count_on=0,count_ill=0,count_on100=0;;
   output_low(relay1);
   output_high(LED);
   //port_a_pullups (0xff);
   setup_timer_0(T0_INTERNAL|T0_DIV_8|T0_8_BIT);      //1.0 ms overflow
   disable_interrupts(INT_TIMER0);
   disable_interrupts(INT_EXT_L2H);
   disable_interrupts(GLOBAL);
   setup_oscillator(OSC_8MHZ|OSC_INTRC);
   
   setup_adc(ADC_CLOCK_DIV_64|ADC_TAD_MUL_2|VSS_VDD);
   setup_adc_ports(sAN0|sAN2);
   set_adc_channel(0);
   //set_pwm1_duty(1);
  
     //i = input(PIN_B3); 
      //j = input(PIN_A6);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   disable_blink;
   
    /*(while(1)
    {
      output_toggle(relay1);
      output_toggle(relay2);
      delay_ms(500);
    }*/
    while(true)
    {
      set_adc_channel(0);
      VR=get_refer(100);
      delay_us(100);
      set_adc_channel(1);
      SS1=get_sample(0,4000);//read_adc();
      delay_us(100);
      //output_toggle(PIN_B0); 
      printf("com = %u, ss1 = %u, count on100 = %lu, count on = %lu, count off = %lu, countill= %lu\r\n",VR,SS1,count_on100,count_on,count_off,count_ill);
      if(VR==0)
      {
         output_low(LED);
         output_low(relay1);
         disable_blink;
      }
      else if(VR==255)
      {
         output_high(LED);
         output_low(relay1);
         disable_blink;
      }
       else
       {
         temp=VR-SS1;
         if(temp>=100)
         {
            led_fre=500;
            disable_blink;
            output_high(led);
         }
         if((temp>=50)&&(temp<100))
         {
            led_fre=500;
            enable_blink;
            count_ill=0;
            count_on=0;
            count_on100=0;
         }
         if((temp>0)&&(temp<50))
         {
            if(count_on100<2)count_on100++;
            else
            {
               led_fre=10+temp*2;
               enable_blink;            
               output_high(relay1);
               count_off=0;
               count_on=0;
               if(count_ill<60)count_ill++;
                  else output_low(relay2);
            }
         }         
         if(SS1>=VR)
         {
            if(count_on<2)count_on++;
            else
            {
               output_high(relay1);
               output_high(relay2);
               disable_blink;
               output_low(LED);
               count_off=0;
               count_ill=0;
               count_on100=0;
            }
         }
            else
            {
               if(count_off<60)count_off++;
                  else
                  {
                     output_low(relay1);
                     output_low(relay2);
                     count_ill=0;
                     count_on=0;
                  }
            }
       }
    }

}
