
#define LED   PIN_A6
#include "main.h"
#include "stdlib.h"
#include "24512.C"
#include <eeprom.c>
#include "ds1307.C"
int8 count_spam=0,booting=0;
//#include "KBD.c"
//#include "I2c.h"
int16 count_checking=10;
#include <scan_key.c>
#include <uart_tool.c>
int8 receiver_state=0,booting_done=0;
int16 over_byte=0;
//#include <GSM.c> 
/*#if defined(SIM900)
   #include <GSM.c>   
#endif*/


#include "mcr.c"
#include "mode.c"
//=========================

//==========================
#INT_TIMER0
void timer0()
{
   int16 buf;
   int8 temp;
   
   //========strobe keyboarddata============
   //=======================================  
   //output_toggle(ROW0);
   //KP_mode=read_ext_eeprom(strobe_keypressmode);
   if(KP_mode)
   {
      if(count_kp<500)count_kp++;
      if(count_kp==500)
      {
            fprintf(COM2,"\n\rKey release\n\r");
            keyprss_off;
            kp_st=0;    
            count_kp++;
      }
   }
   if(keydebug_en==1)
   {
      if(mode_sl==0)kbd_getc();
         else temp=kbd_getc_slv();//kbd_getc_slv1();
   }
   if(enable_getpin==1)
   {
      //output_high(PIN_C2);
      if(mode_sl==0)kbd_getc();
         else temp=kbd_getc_slv();
   }
  if(mcr_timeout<5) mcr_timeout++;
  if(mcr_timeout==5)
  {
      //fprintf(COM2,"\r\nSaving Card Data\r\n");
      if(reading_fg==1)saving_card();
         //else fprintf(COM2,"\r\nreading error\r\n");
      memset(buffertrack1, 0, sizeof(buffertrack1));   
      memset(buffertrack2, 0, sizeof(buffertrack2));
      //debug_card();
      count_reading_error=0;
      mcr_timeout++;  
      //saving_flag=0;
  }
  delaycharaction=read_ext_eeprom(strobe_delaytime);
  //fprintf(COM2,"\r\ndelaycharaction %lu\r\n",charac_timeout);
  if(charac_timeout<(int16)(700*delaycharaction)) charac_timeout++;
  if(charac_timeout==(int16)(700*delaycharaction))
  {
      charac_timeout++;
      buf=get_countcard();
      if(buf<countcards)
      {
         save_key_new();
         buf=buf+1;
         save_coutcard(buf);
      }
         else
         {
            fprintf(COM2,"\r\nFull memory\r\n");
            return;
         }
      //if(cardread_st==0)return;
      fprintf(COM2,"\r\nSaving PIN\r\n");
      fprintf(COM2,"\r\nnext transaction (%ld)\r\n",buf); 
      data_avai=0;
      enable_getpin=0;
      //dis_getpin;
      output_low(LED);
  }
  //=================================
  //=================================
}
//=========================
void main()
{ 
   int8 i,temp,count_char=0;
   static int16 num_card,cards,countc,count_dwla;
   static int16 lastlen;
   int16 len=0,addr_dat;
   int8 strl,j;
   char  string[5],buftemp[124],buftemp2[50];
   //RPINR2=6;
    RPINR1=5;
   charac_timeout=0xffffffff;
   key_timeout=0xffff;
   setup_oscillator(OSC_32MHZ);
   keyprss_off;
   //=============================
   disable_interrupts(GLOBAL);
   init_ext_eeprom();
   #if defined(rst_pwd)
      delay_ms(1000);
      fprintf(COM2,"\r\nreset password\r\n");
      delay_ms(1000);
      reset_password();
      init_password();
      for(i=0;i<10;i++) fprintf(COM2,"%c",read_ext_eeprom(strobe_pass_addr+i));
      fprintf(COM2,"\r\nDone\r\n");
      while(1);
   #endif     
      delay_ms(1000);
   init_password();
   EEPROM_read(strobe_pass_addr,20,password);
   KP_mode=read_ext_eeprom(strobe_keypressmode);
   //=====================================
   rtc_init();
   //RTC_init();
   //mysettime();
   //write_ext_eeprom(65535,12);
   ptr_card=get_ptrcard(strobe_ptrcard_addr);
   
  // fprintf(COM2," save_ptrcard=%lu\n\r",ptr_card);
   ptr_card_key=get_ptrcard(strobe_ptrcard_key);
   fprintf(COM2," save_ptrcard=%lu\n\r",ptr_card_key);
   countcard=get_countcard();
  // fprintf(COM2," countcard=%lu\n\r",countcard);
   //fprintf(COM2,"read_ext_eeprom=%d\r\n",read_ext_eeprom(65535));
   charac_timeout=0xffffffff;
   setup_timer_0(T0_INTERNAL|T0_DIV_256|T0_8_BIT);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   delaycharaction=read_ext_eeprom(strobe_delaytime);
   if(delaycharaction==0)delaycharaction=1;
   //delaykey=read_ext_eeprom(strobe_delaykb);
   //fprintf(COM2," delaycharaction=%u\n\r",delaycharaction);
   //fprintf(COM2,"\n\r charac_timeout=%lu\n\r",charac_timeout);
    //==========================================
      enable_interrupts(INT_TIMER0);
      //enable_interrupts(INT_TIMER1);
      enable_interrupts(INT_EXT1_H2L);
   //enable_interrupts(INT_EXT2_H2L);
      enable_interrupts(INT_RDA);
      
      enable_interrupts(GLOBAL);
 
   mcr_timeout=10000;
    mode_sl=read_ext_eeprom(strobe_Master_SLV);
    debugmode=read_ext_eeprom(strobe_debugmode);
    key_timeout=1000;
   //==========================================
   charac_timeout=0xffffffff;
   //delay_ms(3000);
   
   booting_done=1;
   booting=1;
   charac_timeout=0xffffffff;
   
   EEPROM_read(strobe_nameconsole,16,console);
   EEPROM_read(strobe_pass_addr,20,password);
   set_tris_a(0xff); 
   
   setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN4|VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   
   KP_mode=read_ext_eeprom(strobe_keypressmode);
   type_KB=read_ext_eeprom(kindofKB);
   if(type_KB!=0) fprintf(COM2,"new keboard mode\n\r");
      else fprintf(COM2,"Old keboard mode\n\r");
   if(mode_sl==0)fprintf(COM2,"Slaver mode is disable\n\r");
      else fprintf(COM2,"Slaver mode is enable\n\r");
   

   memset(key_data,0,sizeof(key_data)); 
   //save_coutcard(349);
     /* write_ext_eeprom(200,'I');
      write_ext_eeprom(201,'J');
      write_ext_eeprom(202,'K');
      
      fprintf(COM2,"reading eeprom =%c",read_ext_eeprom(200));
      fprintf(COM2,"%c",read_ext_eeprom(201));
      fprintf(COM2,"%c",read_ext_eeprom(202));*/
    //write_ext_eeprom(131071,100);  
    //fprintf(COM2," eeprom test=%d\n\r",read_ext_eeprom(131071));
     //while(1);
   //save_coutcard(928); 
   while(1)
   {
      if(mode==LOGOFF)
      {
         fprintf(COM2,"password:");
         //fprintf(COM2,"\n\r charac_timeout=%lu\n\r",charac_timeout);
         USART_getstring(STAR_ECHO,20,buffer_uart);
         //EEPROM_read(strobe_pass_addr,20,password);
         /*fprintf(COM2,"\n\r");
         for(i=0;i<5;i++) fprintf(COM2," %x",read_ext_eeprom(i));
         fprintf(COM2,"\n\r");
         for(i=0;i<5;i++) fprintf(COM2," %x",password[i]);
         fprintf(COM2,"\n\r");
         for(i=0;i<20;i++) fprintf(COM2," %x",buffer_uart[i]);//*/
         //EEPROM_read(strobe_pass_addr,20,password);
         if(!stringcomp(buffer_uart,password))
         {
            mode=LOGON;
            fprintf(COM2," OK\n\r");
         }
         else fprintf(COM2," X\n\r");
      }
         else if(mode==LOGON)adminmode();//*/  
   }
}
