#include "main.h"

#define timing_pro_store 0

#define  timer_ex             5
#define  Pulse_per_around     3
#define  Tire_perimeter       1.758

#define  door_UnLock_C    PIN_A3
#define  door_Lock_C      PIN_A2

#define  Speed_Sig      PIN_B0
#define  UnLock_SIG     PIN_B1
#define  Lock_SIG       PIN_B2
#define  Keep_PWR       PIN_B4
#define  BT_IO          PIN_B5
#define  Mirro_open     PIN_A6
#define  Mirro_close    PIN_A7


#define  ACC_CHECK      PIN_B3

#define  disable_blink  disable_interrupts(INT_TIMER0)
#define  enable_blink   enable_interrupts(INT_TIMER0)

//===========================
int16 count_msec=0,count_s=timer_ex;
int1 mirror_st=0;
int8 count_pulse=0;
volatile int8 count_keepPWR=0,timekeeping=10,enable_sleeping_flag=0;
int8 speed=0,repeat_check_speed=0;
volatile int8 timer_bt=0;
volatile int8 second_refer=0;
int1 disable_mirror_button=0;
int1 door_lock_flag=0;
int1 bt_flag_h=0;
int1 bt_flag_l=0;
//===========================
/* 
   V = (m/1000)*3600
   m = 3/CV (3 number pulse per around)
   V = ((count_pulse x Tire_perimeter)/Pulse_per_around)x3.6
   V = (count_pulse x 1.7584)/3x3.6 ~ count_pulse*2.1
*/
int8 calculate_speed()
{
   int8 result=0;
   result = ((count_pulse*Tire_perimeter)/Pulse_per_around)*3.6;
   return result;
}
//===========================
#INT_EXT
void ext0_isr()
{
   //fprintf(DEBUG,"ext0_isr %u\r\n, ",count_pulse);
   //delay_us(100);
   if(input(Speed_Sig)==1) count_pulse++; 
}
//===========================
#int_TIMER0
void  TIMER0_isr(void) 
{
   count_msec++;
   disable_interrupts(INT_EXT);
   timer_bt++;
   if(count_msec>=935) // 1second
   {
      //output_toggle(BT_IO);
      count_msec=0;
      speed = calculate_speed();
      //fprintf(DEBUG,"speed = %u, pulse = %u\r\n, ", speed,count_pulse);
      count_pulse=0;
      if(speed>20) 
      {
         disable_mirror_button = 1;
      }
      else
      {
         disable_mirror_button = 0;
      }
      second_refer++;
      if(count_s<timer_ex) 
      {
         count_s++;
         //fprintf(DEBUG,"Timer overflow %lu\r\n, ",count_s);
         //output_high(Mirro_close);
      }
     if(enable_sleeping_flag==1)
     {
        if(count_keepPWR<timekeeping)
        {
           count_keepPWR++;
           fprintf(DEBUG,".");
        }
        else
        {
           fprintf(DEBUG,"\r\nPower off\r\n");
           output_low(Keep_PWR);
        }
     }
   }
   enable_interrupts(INT_EXT);
}
//===========================
void sleeping(int8 insec)
{
   fprintf(DEBUG,"\r\nPower will to turn off in %ds\r\n",insec);
   timekeeping = insec;
   count_keepPWR = 0;
   enable_sleeping_flag = 1;
}
//===========================
void sleeping_disable()
{
   enable_sleeping_flag = 0;
   count_keepPWR = 0;
}
//===========================
void door_lock()
{
   if(door_lock_flag==0)
   {
      output_high(door_Lock_C);
      delay_ms(500);
      output_low(door_Lock_C);
      door_lock_flag=1;
   }
}
//===========================
void door_unlock()
{
   if(door_lock_flag==1)
   {
      output_high(door_UnLock_C);
      delay_ms(500);
      output_low(door_UnLock_C);
      door_lock_flag=0;
   }
}
//===========================
int8 open_mirror_remote()
{
   int8 result = 0;
   count_s = 0;
   sleeping_disable();
   fprintf(DEBUG,"open_mirror_remote\r\n, ");
   while(count_s<timer_ex)
   {
      if((input(Lock_SIG)==1)&&(input(UnLock_SIG)==0))
      {
         result = 1;
         output_low(Mirro_open);      
       sleeping(10);
         return result;
      }
      output_high(Mirro_open);
      mirror_st = 1;
   }
   output_low(Mirro_open);
   return result;
}
//==========================
int8 close_mirror_remote()
{
   int8 result = 0;
   count_s = 0;
   fprintf(DEBUG,"close_mirror_remote\r\n, ");
   while(count_s<timer_ex)
   {
      if((input(UnLock_SIG)==1)&&(input(Lock_SIG)==0))
      {
         result = 1;
         output_low(Mirro_close);
         return result;
      }
      output_high(Mirro_close);
      mirror_st = 0;
   }
   output_low(Mirro_close);
   return result;
}
//===========================
void open_mirror_speedover()
{
    count_s = 0;
    fprintf(DEBUG,"open_mirror_BT\r\n, ");
    if(mirror_st==0)
    {
      while(count_s<timer_ex)
      {
         output_high(Mirro_open);
      }
      mirror_st = 1;
      output_low(Mirro_open);
    }
}
//===========================
void open_mirror_BT()
{
   if(disable_mirror_button==0)
   {
      count_s = 0;
      fprintf(DEBUG,"open_mirror_BT\r\n, ");
      while(count_s<timer_ex)
      {
         output_high(Mirro_open);
         mirror_st = 1;
      }
      output_low(Mirro_open);
   }
}
//==========================
void close_mirror_BT()
{
   if(disable_mirror_button==0)
   {
      count_s = 0;
      fprintf(DEBUG,"close_mirror_BT\r\n, ");
      while(count_s<timer_ex)
      {
         output_high(Mirro_close);
         mirror_st = 0;
      }
      output_low(Mirro_close);
   }
}
//===========================
void button_touching()
{
   if((input(BT_IO)==1)&&(timer_bt>100))
   {
      bt_flag_h = 1;
      timer_bt=0;
   }
   if((input(BT_IO)==0)&&(timer_bt>100))
   {
      bt_flag_l = 1;
      timer_bt=0;
   }
   if((bt_flag_l==1) && (bt_flag_h==1))
   {
      if(mirror_st == 1)
      {
         close_mirror_BT();
      }
      else
      {
         open_mirror_BT();
      }
      bt_flag_l=0;
      bt_flag_h=0;
   }  
}
//===========================
void main()
{
   setup_timer_0(T0_INTERNAL|T0_DIV_8|T0_8_BIT);      //1.0 ms overflow
   disable_interrupts(INT_TIMER0);
   disable_interrupts(INT_EXT_L2H);
   disable_interrupts(GLOBAL);
   setup_oscillator(OSC_8MHZ|OSC_INTRC);
   
   //setup_adc(ADC_CLOCK_DIV_64|ADC_TAD_MUL_2|VSS_VDD);
   //setup_adc_ports(sAN0|sAN2);
   //set_adc_channel(0);
   //set_pwm1_duty(1);
  
   //delay_ms(1000);
   ext_int_edge( L_TO_H );
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   
   output_high(Keep_PWR);
   open_mirror_remote();  
   
   fprintf(DEBUG,"Start...\r\n");
   delay_ms(1000);
    while(1)
    {
      if(input(ACC_CHECK)==0)
      {
         if((second_refer>2)&&(input(ACC_CHECK)==0))
         {
            //door_unlock();
            if((input(Lock_SIG)==1)&&(input(UnLock_SIG)==0))
            {
               //fprintf(DEBUG,"lock is low %lu\r\n, ", count_s);
            sleeping(10);
               if(close_mirror_remote()==1)
               {
                   open_mirror_remote();
               }
            }
            if((input(UnLock_SIG)==1)&&(input(Lock_SIG)==0))
            {
               //fprintf(DEBUG,"unlock is low %lu\r\n, ", count_s);
               if(open_mirror_remote()==1)
               {
                  close_mirror_remote();
               }
            }
            door_unlock();
         }
      }
      else
      {
         if(input(ACC_CHECK)==1)
         {
            second_refer=0;
            button_touching();
            if(speed>20)
            {
                if(repeat_check_speed<10)
                {
                    repeat_check_speed++;
                }
                else
                {
                    door_lock();
                    open_mirror_speedover();
                }
            }
            else
            {
               //door_unlock();
               repeat_check_speed=0;
            }
            if((input(UnLock_SIG)==1)&&(input(Lock_SIG)==0))
            {
               door_lock_flag = 0;
            }
            else if((input(Lock_SIG)==1)&&(input(UnLock_SIG)==0))
            {
               door_lock_flag = 1;
            }
         }
       fprintf(DEBUG,"speed = %u, pulse = %u\r\n, ", speed,count_pulse);
      }     
      delay_ms(50);
    }
}
