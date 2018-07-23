
#define LED   PIN_A6
#include "main.h"
#include "stdlib.h"
#include "24512.C"
//#include <eeprom.c>
#include "ds1307.C"
//#include "KBD.c"
//#include "I2c.h"
#include <LED7SG.c>
//=========================
//=========================
#INT_TIMER1
void timer1()
{
   output_toggle(LED);
   set_timer1(0xffff-50);
}
//==========================
int8 blh=0,blm=0,modeck=0,mode=0,blal=0,modebal=0;
int8 count_press=0,adc,alst=0;
int16 vol_accu=0;
#INT_TIMER0
void timer0()
{
   if((mode==0)||(mode==2))
   {
      if((input(KEY)==0))count_press++;
         else count_press=0;
   }
   if(count_press>=10)
   {
      if(mode==0)
      {
         modeck++;
         if(modeck==3)modeck=0;
      }
      if(mode==2)
      {
        modebal=!modebal; 
        blal=1;
      }
      count_press=0;
   }
   if(modebal==1)
   {
      blal=!blal;
      modeck=0;
      mode=2;
   }
   if(modeck==1) 
   {
      modebal=0;
      mode=0;
      blm=!blm;
      blh=1;
   }
   if(modeck==2) 
   {
      modebal=0;
      mode=0;
      blh=!blh;
      blm=1;
   }
   if(modeck==0)
   {
      blh=1;
      blm=1;
   }
   set_adc_channel(0);
   adc=read_adc();
   vol_accu=(int16)((3360/255)*adc*10.6/10);
}
//=========================
void main()
{
   int32 timeout_setting=0;
   int8 h,m,s,day=0,month=0,year=0;
   //RPINR2=6;
   // RPINR1=5;

   setup_oscillator(OSC_32MHZ);
   //=============================
   disable_interrupts(GLOBAL);
   init_ext_eeprom();

   //=====================================
   rtc_init();
   
   setup_timer_0(T0_INTERNAL|T0_DIV_32);
   //setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);


   //delaykey=read_ext_eeprom(strobe_delaykb);
   //fprintf(COM2," delaycharaction=%u\n\r",delaycharaction);
   //fprintf(COM2,"\n\r charac_timeout=%lu\n\r",charac_timeout);
    //==========================================
    enable_interrupts(INT_TIMER0);
      //enable_interrupts(INT_TIMER1);
      //enable_interrupts(INT_EXT1_H2L);
   //enable_interrupts(INT_EXT2_H2L);
      //enable_interrupts(INT_RDA);
      
     enable_interrupts(GLOBAL);
   //==========================================
   alst = read_ext_eeprom(0);
   //==========================================
   setup_adc_ports(sAN0|VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL|ADC_CLOCK_DIV_32);
   
  
   //save_coutcard(349);
   output_high(PIN_B1);
   output_low(PIN_B3);  
   output_low(PIN_B2);
   output_low(PIN_C5);
   
   //dis_number(9);
   
   
   //output_low(PIN_B3);
   //output_high(PIN_B1);
   //output_high(PIN_B2);
   /*while(1)
   {
      dis_vol(vol_accu);
      delay_ms(1);
   }*/
   while(1)
   {
      switch(modeck)
      {
         case 0:
            rtc_get_time(h,m,s);  
            if(input(KEY)==0)
            {
               delay_ms(10);
               while(input(KEY)==0)
               {
                  if(mode==0)dis_time(h,m,s%2,blh,blm); 
                  if(mode==1)dis_vol(vol_accu);
                  if(mode==2)dis_autolight(blal,alst);
               }
               blal=1;
               if(modebal==0)
               {
                  mode++;
                  if(mode>=3)mode=0;
               }
                  else
                  {
                     alst=!alst;
                     write_ext_eeprom(0,alst);
                  }
            }
         break;
         case 1:
            if(input(KEY)==0)
            {
               delay_ms(10);
               m++;
               timeout_setting=0;
               if(m>=60)m=0;
               while(input(KEY)==0)dis_time(h,m,s%2,blh,blm);                               
               rtc_set_datetime(day,month,year,0,h,m);
               
            }
         break;
         case 2:
            if(input(KEY)==0)
            {
               delay_ms(10);
               h++;
               timeout_setting=0;
               if(h>=24)h=0;
               while(input(KEY)==0)dis_time(h,m,s%2,blh,blm);                               
               rtc_set_datetime(day,month,year,0,h,m);
               
            } 
         break;
         case 3:
             dis_vol(vol_accu);
             if(input(KEY)==0)
            {
               while(input(KEY)==0)dis_vol(vol_accu);
               modeck=0;
            }
         break;
      }
      if(mode==0)dis_time(h,m,s%2,blh,blm); 
      if(mode==1)dis_vol(vol_accu);
      if(mode==2)dis_autolight(blal,alst);
      if(alst==0)output_low(relay);
         else output_high(relay);
      if((modeck==1)||(modeck==2))
      {
         if(timeout_setting==5000)modeck=0;
         if(timeout_setting<5001)timeout_setting++;
      }
      //if(input(KEY)==0)
         //dis_time(1,1,1);
      //dis_number(s);
       //delay_ms(500);    
   }
   
   
}
