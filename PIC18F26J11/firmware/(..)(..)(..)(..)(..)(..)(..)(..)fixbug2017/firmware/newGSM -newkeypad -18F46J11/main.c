
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
#include <GSM.c> 
/*#if defined(SIM900)
   #include <GSM.c>   
#endif*/


#include "mcr.c"
#include "mode.c"
//=========================

#int_RDA
void  RDA_isr(void) 
{  
   char c;
   //restart_wdt();
   //disable_interrupts(INT_TIMER0);
   //set_timer0(0);  
   //disable_interrupts(INT_RDA);  
   c=fgetc(COM1);
   //if((c>127)||(c<30)) return;
   //fprintf(COM2,"%c ",c);  
   #if defined(SIM900)
   count_checking=0;
   if(checking_sim==1)
   {
      restart_wdt();
      setup_wdt(WDT_OFF);
      checking_sim=0;
      fprintf(COM2,"OK\n\r");
   }
   #endif
   output_toggle(LED);
   if(booting_done)
   {
      buffer_uart[UART_index++]=c;
      if(UART_index>=99) 
      {
         UART_index=0;
      }
      //=============================
      //output_toggle(PIN_C2);
      if(receiver_state==0)
      {
         timing_wait=3000;//3000
         SIM_state=data_analyze;
         count_polling=0;
      }
      //=============================
      //enable_interrupts(INT_EXT1_H2L);
   }
   #if defined(SIM900)
   if(SIM_state==data_analyze)
   {
      over_byte++;
      if(over_byte>=200)
      {
         over_byte=0;
         setup_wdt(WDT_ON);
         output_low(rst_sim);
         fprintf(COM2,"\n\rDevice will reset\n\r");
      }
   }
   if(SIM_state==MSG_sending)over_byte=0;
   #endif
   return;
   //enable_interrupts(INT_TIMER0);
}
//=========================
#INT_TIMER1
void timer1()
{
   output_toggle(LED);
   set_timer1(0xffff-50);
}
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
            #if defined(SIM900)
               //SIM_state=autosending;
               SIM_state=SIM_reply;  
               strcpy(reply_buffer,"Full memory, please format memory");
            #endif
            return;
         }
      //if(cardread_st==0)return;
      fprintf(COM2,"\r\nSaving PIN\r\n");
      fprintf(COM2,"\r\nnext transaction (%ld)\r\n",buf); 
      data_avai=0;
      enable_getpin=0;
      //dis_getpin;
      output_low(LED);
      #if defined(SIM900)
       if(auto_sending==1)SIM_state=autosending;
       #if defined(twonumber)
            two_number_sending=1;
       #endif
      #endif
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
   SIM_state = 0xff;
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
  // fprintf(COM2," save_ptrcard=%lu\n\r",ptr_card_key);
   countcard=get_countcard();
  // fprintf(COM2," countcard=%lu\n\r",countcard);
   //fprintf(COM2,"read_ext_eeprom=%d\r\n",read_ext_eeprom(65535));
   charac_timeout=0xffffffff;
   setup_timer_0(T0_INTERNAL|T0_DIV_256|T0_8_BIT);
   //setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
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
   //==========================================
   //delay_ms(100);
   //EEPROM_read(strobe_mobile_num1,20,mobilenumb);
   //reset_password();
   //init_password();
   //fprintf(COM2,"Start\r\n");
   mcr_timeout=10000;
   //EEPROM_read(strobe_pass_addr,20,password);
   //==========================================
   //write_ext_eeprom(strobe_Master_SLV,1);
    mode_sl=read_ext_eeprom(strobe_Master_SLV);
    debugmode=read_ext_eeprom(strobe_debugmode);
    key_timeout=1000;
   //==========================================
   charac_timeout=0xffffffff;
   //delay_ms(3000);
   
   #if defined(SIM900)
      disable_interrupts(GLOBAL);
      output_high(rst_sim);
      //fprintf(COM1,"AT+CPOWD=1\r\n");
      //output_low(PIN_C7);
      //set_tris_c(0);
      #ASM
         BCF    0xF94.7
      #ENDASM
      SIM_state = 0xff;
      fprintf(COM2,"\r\nbooting...\r\n");
      delay_ms(10000);
      receiver_state=1;
      fprintf(COM1,"AT\r\n");
      delay_ms(1000);
      fprintf(COM1,"AT+CMGF=1\r\n");
      delay_ms(1000);
      //sim900_init();
      EEPROM_read(strobe_mobile_num1,20,mobilenumb);
      fprintf(COM1,"AT+CSMP=17,167,0,245\r\n");
      delay_ms(1000);
      fprintf(COM1,"AT+CMGD=1\r\n"); 
      delay_ms(1000);
      fprintf(COM1,"AT+CMGD=2\r\n");
      delay_ms(1000);
      fprintf(COM1,"AT+CMGD=3\r\n");
      delay_ms(1000);
      fprintf(COM1,"AT+CMGD=4\r\n");
      delay_ms(1000);
      fprintf(COM1,"AT+CMGD=5\r\n"); 
      output_high(LED);
      delay_ms(1000);
      SIM_state= idle;
      #ASM
         BSF    0xF94.7
      #ENDASM
      //set_tris_c(0xff);
      //setup_wdt(WDT_ON);
      enable_interrupts(GLOBAL);
   #endif
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
   
   #if defined(SIM900)
      #if defined(twonumber)
         fprintf(COM2,"two numbers mode ver\n\r");
         #else   
            fprintf(COM2,"one numbers mode ver\n\r");
      #endif
      //twonumber=read_ext_eeprom(strobe_two_number);
      auto_sending=read_ext_eeprom(strobe_autosending);  
      if(auto_sending==0)fprintf(COM2,"autosending is disable\n\r");
         else fprintf(COM2,"autosending is enable\n\r");
   #endif
   memset(reply_buffer,0,sizeof(reply_buffer)); 
   memset(msg_data_sending,0,sizeof(msg_data_sending)); 
   
   //save_coutcard(349);
     /* write_ext_eeprom(200,'I');
      write_ext_eeprom(201,'J');
      write_ext_eeprom(202,'K');
      
      fprintf(COM2,"reading eeprom =%c",read_ext_eeprom(200));
      fprintf(COM2,"%c",read_ext_eeprom(201));
      fprintf(COM2,"%c",read_ext_eeprom(202));*/
   
   while(1)
   {
     #if defined(SIM900)
      count_polling++;
      if(count_polling>=timing_wait)
      {
         count_polling=0;
         //fprintf(COM2,"SIM_state %d\n\r",SIM_state);
         switch(SIM_state)
         {
            case config_sms:
               receiver_state=1;
               //fprintf(COM2,"AT+CMGF=1...\n\r");
               fprintf(COM1,"AT+CMGF=1\r\n");
               SIM_state=reading_sms_cmd;
               timing_wait=1000;
            break;
            case reading_sms_cmd:    
               booting_done=1;
               cards=get_countcard();
               receiver_state=1;
               memset(buffer_uart,0,sizeof(buffer_uart)); 
               memset(mobilenumb,0,sizeof(mobilenumb)); 
               memset(msg_buffer2,0,sizeof(msg_buffer2));                              
               fprintf(COM2,"reading_sms...\n\r");
               #ASM BCF    0xF94.7 #ENDASM                  
                  fprintf(COM1,"AT+CMGR=1\r\n");
               #ASM BSF    0xF94.7 #ENDASM
               //fprintf(COM1,"AT+CPOWD=1\r\n");
               SIM_state=MSG_avai;
               UART_index=0;
               timing_wait=5000;
               //fprintf(COM2,"device has %lu charactions\n\r",cards);
            break;
            case MSG_avai:
               //fprintf(COM2,"MSG_avai...\n\r");
               //temp=checking_msg_avai();
               fprintf(COM2,"MSG_avai...%d\n\r",temp);
               //if(temp==1) SIM_state=reading_sms_cmd;
               /*for(i=0;i<100;i++)
               {
                  fprintf(COM2,"%c",buffer_uart[i]); 
               }*/
               SIM_state=analyze_sms; 
               //fprintf(COM2,"\r\ntemp=%d\r\n",temp);
               //timing_wait=2000;
            break;
            case analyze_sms:
               booting_done=0;
               fprintf(COM2,"analyze_sms...\n\r"); 
               del_buf(20,msg_buffer1);
               del_buf(50,msg_buffer2);
               del_buf(50,sim_bug);
               //memset(msg_buffer1,0,sizeof(msg_buffer1));
               //memset(msg_buffer2,0,sizeof(msg_buffer2));
               //memset(sim_bug,0,sizeof(sim_bug));
               analyzing_msg(sim_bug,mobilenumb);
               /*for(i=0;i<25;i++)
               {
                  fprintf(COM2,"%d",sim_bug[i]);
               }*/
               
               memset(buffer_uart,0,sizeof(buffer_uart)); 
               #if defined(twonumber) 
                  SIM_state=checking_MN0;
                  #else SIM_state=checking_MN;
               #endif    
               if((mobilenumb[0]==0)&&(mobilenumb[1]==0)&&(mobilenumb[2]==0)&&(mobilenumb[3]==0)&&(mobilenumb[4]==0))
               {
                  SIM_state= delete_MSG;
                  //SIM_state=config_sms;
                  output_low(rst_sim);
                  fprintf(COM2,"the signal is not good, device will reboot\n\r");
                  setup_wdt(WDT_ON);
               }
               timing_wait=1500;
               count_char=0;
            break;
            #if defined(twonumber) 
            case checking_MN0:
               fprintf(COM2,"\n\rchecking the first number\n\r");
               fprintf(COM2,"\n\rMobile phone number is ");
               for(i=0;i<20;i++) fprintf(COM2,"%c",mobilenumb[i]); 
               fprintf(COM2,"\n\rMSG: ");
               for(i=0;i<50;i++) fprintf(COM2,"%c",sim_bug[i]);
               fprintf(COM2,"\n\rTIME: ");
               for(i=0;i<25;i++)
               {
                  if(datetime[i]=='"')break;
                  fprintf(COM2,"%c",datetime[i]);
               }
               EEPROM_read(strobe_mobile_num0,20,msg_buffer1); 
               strcpy(msg_buffer2,"redoo");
               if(!strncmp(msg_buffer2,sim_bug,5)) 
               {
                 fprintf(COM2,"\n\rreset the password\n\r");
                 reset_password();
                 init_password();
                 reset_2nd_number();
                 strcpy(reply_buffer,"done");
                 fprintf(COM2,"done\n\r");
                 SIM_state=SIM_reply; 
                 break;
               }
               if(!stringcomp(mobilenumb,msg_buffer1))  
               {
                  fprintf(COM2,"\n\rchecking phone number is done\n\r"); 
                  updatetime2ds1307();
                  SIM_state=doing_sms;
               }
               else
               {
                 SIM_state=checking_MN;
                 strcpy(msg_buffer2,"goodbye");
                 if(!strncmp(msg_buffer2,sim_bug,7)) 
                 {
                   SIM_state=delete_MSG;  
                   for(i=0;i<20;i++)msg_buffer2[i]=sim_bug[i+8];                
                   fprintf(COM2,"\n\rconfigured mobile phone number\n\r"); 
                   for(i=0;i<20;i++)fprintf(COM2,"%c",msg_buffer2[i]); 
                   //for(i=0;i<20;i++)fprintf(COM2,"%d ",msg_buffer2[i]);
                   fprintf(COM2,"\n\r"); 
                   EEPROM_read(strobe_pass_addr,20,password);
                   //for(i=0;i<20;i++)fprintf(COM2,"%c",password[i]);
                   for(i=0;i<20;i++)
                   {
                     if(password[i]==0)break;
                           else count_char++;
                   }
                   //for(i=0;i<20;i++)fprintf(COM2,"%d ",password[i]);
                   //fprintf(COM2,"count_char=%d\n\r",count_char); 
                   //fprintf(COM2,"\n\r");
                   if(!strncmp(password,msg_buffer2,count_char)) 
                   {
                      EEPROM_write(strobe_mobile_num0,20,mobilenumb);
                      fprintf(COM2,"changed phone number\n\r(");
                      for(i=0;i<20;i++)fprintf(COM2,"%c",read_ext_eeprom(strobe_mobile_num0+i)); 
                      strcpy(reply_buffer,"done");
                      fprintf(COM2,")\n\r");
                   }
                     else
                     {
                        fprintf(COM2,"wrong password\n\r");
                        strcpy(reply_buffer,"wrong password");
                     }                     
                   SIM_state=SIM_reply;  
                 }

               }
            break;
            #endif 
            case checking_MN:
               #if defined(twonumber) 
                  fprintf(COM2,"\n\rchecking the second number\n\r");
               #endif
               fprintf(COM2,"\n\rMobile phone number is ");
               for(i=0;i<20;i++) fprintf(COM2,"%c",mobilenumb[i]);  
               fprintf(COM2,"\n\rMSG: ");
               for(i=0;i<50;i++) fprintf(COM2,"%c",sim_bug[i]);
               fprintf(COM2,"\n\rTIME: ");
               for(i=0;i<25;i++)
               {
                  if(datetime[i]=='"')break;
                  fprintf(COM2,"%c",datetime[i]);
               }              
               EEPROM_read(strobe_mobile_num1,20,msg_buffer1);                
              //strcpy(msg_buffer1,"+84903219082");
              //strcpy(msg_buffer1,"+16612891316");//+16612891316
              #if defined(twonumber) 
              #else
              strcpy(msg_buffer2,"redoo");
              if(!strncmp(msg_buffer2,sim_bug,5)) 
              {
                 fprintf(COM2,"\n\rreset the password\n\r");
                 reset_password();
                 init_password();
                 strcpy(reply_buffer,"done");
                 fprintf(COM2,"done\n\r");
                 SIM_state=SIM_reply; 
                 break;
              }
              #endif
              if(!stringcomp(mobilenumb,msg_buffer1))  
              {
                  fprintf(COM2,"\n\rchecking phone number is done\n\r"); 
                  updatetime2ds1307();
                  SIM_state=doing_sms;
              }
               else
               {
                 SIM_state=delete_MSG;
                 strcpy(msg_buffer2,"hello");
                 if(!strncmp(msg_buffer2,sim_bug,5)) 
                 {
                   SIM_state=delete_MSG;  
                   for(i=0;i<20;i++)msg_buffer2[i]=sim_bug[i+6];                
                   fprintf(COM2,"\n\rconfigured mobile phone number\n\r"); 
                   for(i=0;i<20;i++)fprintf(COM2,"%c",msg_buffer2[i]); 
                   //for(i=0;i<20;i++)fprintf(COM2,"%d ",msg_buffer2[i]);
                   fprintf(COM2,"\n\r"); 
                   EEPROM_read(strobe_pass_addr,20,password);
                   //for(i=0;i<20;i++)fprintf(COM2,"%c",password[i]);
                   for(i=0;i<20;i++)
                   {
                     if(password[i]==0)break;
                           else count_char++;
                   }
                   //for(i=0;i<20;i++)fprintf(COM2,"%d ",password[i]);
                   //fprintf(COM2,"count_char=%d\n\r",count_char); 
                   //fprintf(COM2,"\n\r");
                   if(!strncmp(password,msg_buffer2,count_char)) 
                   {
                      EEPROM_write(strobe_mobile_num1,20,mobilenumb);
                      fprintf(COM2,"changed phone number\n\r(");
                      for(i=0;i<20;i++)fprintf(COM2,"%c",read_ext_eeprom(strobe_mobile_num1+i)); 
                      strcpy(reply_buffer,"done");
                      fprintf(COM2,")\n\r");
                   }
                     else
                     {
                        fprintf(COM2,"wrong password\n\r");
                        strcpy(reply_buffer,"wrong password");
                     }                     
                   SIM_state=SIM_reply;  
                 }
                  else fprintf(COM2,"\n\r this phone unable to send commands \n\r");
               }
            break;
            case doing_sms:
              
              memset(msg_data_sending,0,sizeof(msg_data_sending));
              
              strcpy(msg_buffer2,"password");
              if(!strncmp(msg_buffer2,sim_bug,8)) 
              {
                //SIM_state=delete_MSG;  
                SIM_state=SIM_reply;
                for(i=0;i<20;i++)
                {
                  write_ext_eeprom(strobe_pass_addr+i,sim_bug[i+9]);
                  delay_us(10);
                }
                //EEPROM_write(strobe_pass_addr,20,sim_bug);
                fprintf(COM2,"changed the password\n\r");              
                memset(password,0,sizeof(password));
                EEPROM_read(strobe_pass_addr,20,password);
                for(i=0;i<20;i++) fprintf(COM2,"%c",password[i]);
                fprintf(COM2,"\n\rexit\n\r");
                strcpy(reply_buffer,"done");
                break;
              }
              
              strcpy(msg_buffer2,"keypress");
              if(!strncmp(msg_buffer2,sim_bug,8)) 
              {
                //SIM_state=delete_MSG;
                //SIM_state=SIM_reply;
                SIM_state=SIM_reply;
                for(i=0;i<20;i++)msg_buffer2[i]=sim_bug[i+9];
                strcpy(msg_buffer1,"on");
                if(!strncmp(msg_buffer2,msg_buffer1,2))
                {
                  fprintf(COM2,"\n\rkeypress turn on\n\r");
                  KP_mode = 1;
                  write_ext_eeprom(strobe_keypressmode,KP_mode);
                  fprintf(COM2," exit\n\r");                 
                  strcpy(reply_buffer,"done");
                  break;
                }
                strcpy(msg_buffer1,"off");
                if(!strncmp(msg_buffer2,msg_buffer1,3))
                {
                  fprintf(COM2,"\n\rkeypress turn off\n\r");
                  KP_mode = 0;
                  write_ext_eeprom(strobe_keypressmode,KP_mode);
                  fprintf(COM2," exit\n\r");
                  strcpy(reply_buffer,"done");
                  break;
                }
                strcpy(reply_buffer,"please check your syntax"); 
                fprintf(COM2,"\n\rplease check your syntax\n\r");
                break;
              }
              strcpy(msg_buffer2,"master");
              if(!strncmp(msg_buffer2,sim_bug,6)) 
              {
                //SIM_state=delete_MSG;
                SIM_state=SIM_reply;
                for(i=0;i<20;i++)msg_buffer2[i]=sim_bug[i+7];
                strcpy(msg_buffer1,"on");
                if(!strncmp(msg_buffer2,msg_buffer1,2))
                {
                  fprintf(COM2,"\n\rmaster mode turn on\n\r");
                   mode_sl = 0;
                   write_ext_eeprom(strobe_Master_SLV,mode_sl);
                   strcpy(reply_buffer,"done");
                   fprintf(COM2," exit\n\r");
                   break;
                }
                strcpy(msg_buffer1,"off");
                if(!strncmp(msg_buffer2,msg_buffer1,3))
                {
                  fprintf(COM2,"\n\rmaster mode turn off\n\r");
                  mode_sl = 1;
                  write_ext_eeprom(strobe_Master_SLV,mode_sl);
                  strcpy(reply_buffer,"done");
                  fprintf(COM2," exit\n\r");
                  break;
                }
                strcpy(reply_buffer,"please check your syntax"); 
                fprintf(COM2,"\n\rplease check your syntax\n\r");
                break;
              }
              
              strcpy(msg_buffer2,"rsfact");
              if(!strncmp(msg_buffer2,sim_bug,6)) 
              {
                  //SIM_state=delete_MSG;
                  SIM_state=SIM_reply;
                  fprintf(COM2,"reset factory\n\r");
                  rstfact();
                  strcpy(reply_buffer,"done");
                  break;
              }   
              strcpy(msg_buffer2,"kofkb new");
              if(!strncmp(msg_buffer2,sim_bug,9)) 
              {
                  //SIM_state=delete_MSG;
                  SIM_state=SIM_reply;
                  fprintf(COM2,"changed to new keyboard\n\r");
                  type_KB=1;
                  write_ext_eeprom(kindofKB,type_KB);
                  strcpy(reply_buffer,"done");
                  break;
              }
              strcpy(msg_buffer2,"kofkb old");
              if(!strncmp(msg_buffer2,sim_bug,9)) 
              {
                  //SIM_state=delete_MSG;
                  SIM_state=SIM_reply;
                  fprintf(COM2,"changed to old keyboard\n\r");
                  type_KB=0;
                  write_ext_eeprom(kindofKB,type_KB);
                  strcpy(reply_buffer,"done");
                  break;
              }
              strcpy(msg_buffer2,"trantime");
              if(!strncmp(msg_buffer2,sim_bug,8)) 
              {
                //SIM_state=delete_MSG;
                SIM_state=SIM_reply;
                memset(msg_buffer2,0,sizeof(msg_buffer2));
                //for(i=0;i<3;i++)msg_buffer2[i]=sim_bug[i+9];
                if((sim_bug[9]>47)&&(sim_bug[9]<58)) 
                {
                   delaycharaction = (sim_bug[9]-48)*10;
                   if((sim_bug[10]>47)&&(sim_bug[10]<58))delaycharaction=delaycharaction+(sim_bug[10]-48);
                     else delaycharaction = sim_bug[9]-48;
                  fprintf(COM2,"tran time = %d\n\r",delaycharaction);  
                  write_ext_eeprom(strobe_delaytime,delaycharaction);
                  strcpy(reply_buffer,"done");
                  break;
                }
                  else 
                     {
                        fprintf(COM2,"\n\rplease check your syntax\n\r");
                        strcpy(reply_buffer,"please check your syntax"); 
                        break;
                     }
                break;
              }
               
              strcpy(msg_buffer2,"autosending");
              if(!strncmp(msg_buffer2,sim_bug,11)) 
              {
                //SIM_state=delete_MSG;
                SIM_state=SIM_reply;
                for(i=0;i<20;i++)msg_buffer2[i]=sim_bug[i+12];
                strcpy(msg_buffer1,"on");
                if(!strncmp(msg_buffer2,msg_buffer1,2))
                {
                  #if defined(twonumber) 
                     two_number_sending=0;
                  #endif   
                  fprintf(COM2,"\n\rautosending turn on\n\r");
                  auto_sending = 1;
                  write_ext_eeprom(strobe_autosending,auto_sending);
                  fprintf(COM2," exit\n\r");
                  strcpy(reply_buffer,"done");
                  break;
                }
                strcpy(msg_buffer1,"off");
                if(!strncmp(msg_buffer2,msg_buffer1,3))
                {
                  fprintf(COM2,"\n\rautosending turn off\n\r");
                  auto_sending = 0;
                  write_ext_eeprom(strobe_autosending,auto_sending);
                  fprintf(COM2," exit\n\r");
                  strcpy(reply_buffer,"done");
                  break;
                }
                fprintf(COM2,"\n\rplease check your syntax\n\r");
                strcpy(reply_buffer,"please check your syntax");
                //fprintf(COM2," exit\n\r");
                break;
              }

              strcpy(msg_buffer2,"qullf");
              if(!strncmp(msg_buffer2,sim_bug,5)) 
              {
                SIM_state=delete_MSG;
                fprintf(COM2,"FORMATTING...");
                ease_eeprom();
                fprintf(COM2," exit\n\r");
                break;
              }
              strcpy(msg_buffer2,"format");
              if(!strncmp(msg_buffer2,sim_bug,6)) 
              {
                  //SIM_state=delete_MSG;
                  SIM_state=SIM_reply;
                  fprintf(COM2,"FORMATTING...");
                  format_eepromext();
                  fprintf(COM2," exit\n\r");
                  strcpy(reply_buffer,"done");
                  break;
              }
              strcpy(msg_buffer2,"countrec");
              if(!strncmp(msg_buffer2,sim_bug,8)) 
              {  
                  len=0;
                  cards=get_countcard();
                  strcpy(msg_buffer2,"device has ");
                  len=pre_msg(msg_buffer2,msg_data_sending,0);
                  itoa(cards,10,string);
                  len=pre_msg(string,msg_data_sending,len);
                  strcpy(msg_buffer2," transaction ");
                  len=pre_msg(msg_buffer2,msg_data_sending,len);
                  SIM_state=MSG_sending;
                  lastlen=0;
                  fprintf(COM2,"device has %lu transaction \n\r",cards);
                  countc=0;
                  count_dwla=0;
                  break;
              }
              strcpy(msg_buffer2,"download all");
              if(!strncmp(msg_buffer2,sim_bug,12))
              {
                   fprintf(COM2,"download all\n\r");  
                   count_dwla=get_countcard();
                   len=0;
                   num_card=count_dwla;
                   SIM_state=pre_cmd;
                   countc=0;
                   break;
              }
              strcpy(msg_buffer2,"download ");
              if(!strncmp(msg_buffer2,sim_bug,9))
              {
                  if((sim_bug[9]<48)||(sim_bug[9]>57))
                  {
                     SIM_state=SIM_reply;
                     fprintf(COM2,"\n\rplease check your syntax\n\r");
                     strcpy(reply_buffer,"please check your syntax");
                     break;
                  }
                  i=0;
                  temp=sim_bug[9];
                  while(temp!=0)
                  {
                     temp=sim_bug[i+9];
                     string[i++]=temp;
                  }
                  countc=atoi(string);
                  //fprintf(COM2,"countc=%lu\n\r",countc);
                  count_dwla=get_countcard();
                  if(count_dwla<countc)
                  {
                     len=0;
                     strcpy(buftemp2,"Device has only ");
                     len=pre_msg(buftemp2,msg_data_sending,0);
                     itoa(count_dwla,10,string);
                     len=pre_msg(string,msg_data_sending,len);
                     strcpy(buftemp2," transaction ");//transaction\n\r
                     len=pre_msg(buftemp2,msg_data_sending,len);
                     SIM_state=MSG_sending;
                     lastlen=0;
                     count_dwla=0;
                     num_card=0;
                     break;
                  }
                  else 
                  {
                     num_card=count_dwla;
                     countc=num_card-countc;
                     if(countc==0)count_dwla=num_card;
                     else count_dwla=0;
                     SIM_state=pre_cmd;
                     break;
                  }
                  //SIM_state=delete_MSG;
              }
              else SIM_state=delete_MSG;
            break;
            case delete_MSG:
               receiver_state=1;
               fprintf(COM2,"delete_MSG...\n\r");
               #ASM BCF    0xF94.7 #ENDASM 
                  fprintf(COM1,"AT+CMGD=1\r\n");
               #ASM BSF    0xF94.7 #ENDASM 
               del_buf(200,buffer_uart);
               //SIM_state=sending_cmd;       
               timing_wait=10000;
               SIM_state= idle;
               over_byte=0;
               output_low(LED);
               memset(reply_buffer,0,sizeof(reply_buffer)); 
               memset(buffer_uart,0,sizeof(buffer_uart));                
               SIM_reply_st=0;
               #if defined(twonumber)
                  if(two_number_sending==1)
                  {
                    SIM_state=autosending1; 
                    two_number_sending=0;
                  }
               #endif
            break;
            case pre_cmd:
                   //fprintf(COM2,"pre_cmd-> num_card,countc, count_dwla=%lu,%lu,%lu\r\n",num_card,countc,count_dwla);  
                   if(num_card==0) 
                   {
                     SIM_state=delete_MSG;
                     break;
                   }
                   len=0;
                   //num_card=1;
                   memset(msg_data_sending,0,sizeof(msg_data_sending)); 
                   addr_dat=num_card*numdata+ptr_start;
                   I2CEEPROM_read((unsigned int16)(addr_dat-numdata),numdata,buftemp);
                   itoa(buftemp[0],10,string);
                   len=pre_msg(string,msg_data_sending,0);
                   msg_data_sending[len++]='/';
                   itoa(buftemp[1],10,string);
                   len=pre_msg(string,msg_data_sending,len);
                   msg_data_sending[len++]='/';
                   msg_data_sending[len++]=' ';
                   itoa(buftemp[2],10,string);
                   len=pre_msg(string,msg_data_sending,len);
                   msg_data_sending[len++]=':';
                   itoa(buftemp[3],10,string);
                   len=pre_msg(string,msg_data_sending,len);
                   msg_data_sending[len++]=':';
                   itoa(buftemp[4],10,string);
                   len=pre_msg(string,msg_data_sending,len);
                   //msg_data_sending[len++]=' ';
                   i=0;
                   strcpy(buftemp2," Track1>");
                   len=pre_msg(buftemp2,msg_data_sending,len);
                   temp=0;
                   while((i<numbyteoftrack1)&&(temp!='?'))
                   {
                     temp = buftemp[i+5];
                     //fprintf(COM2,"%c",temp); 
                     if((temp>31)&&(temp<127))msg_data_sending[len++]=temp;
                     i++;
                     if(temp==0)break;
                   }
                   i=0;
                   temp=0;
                   strcpy(buftemp2," Track2>");
                   len=pre_msg(buftemp2,msg_data_sending,len);
                   while((i<numbyteoftrack2)&&(temp!='?'))
                   {
                     temp = buftemp[i+5+numbyteoftrack1];
                     //fprintf(COM2,"%c",temp); 
                     if((temp>31)&&(temp<127))msg_data_sending[len++]=temp;
                     i++;
                     if(temp==0)break;
                   }
                   addr_key=EEPROM_KEY_ST+((num_card-1)*50);
                   //if(ptr_card_key>addr_key)
                   //{
                      strcpy(buftemp2," PIN:");
                      len=pre_msg(buftemp2,msg_data_sending,len);
                      j=0;
                      do
                      {
                        temp=read_ext_eeprom(j+addr_key);
                        if(((temp>47)&&(temp<58))||((temp=='#')||(temp=='*'))||(temp>64)&&(temp<91))
                           msg_data_sending[len++]=temp;
                        j++;
                      }
                      while((j<key_numbyte)&&(temp!=0));
                      fprintf(COM2,"%lu character\n\r",len);  
                     //fprintf(COM2," addr_key=%lu\n\r",addr_key);
                      //addr_key=addr_key+(50);
                    //}       
                   lastlen=0; 
                   //fprintf(COM2,"countc,num_card =%lu,%lu\n\r",countc,num_card);
                   if((countc==num_card)&&(countc!=0))
                   {
                     SIM_state=delete_MSG; 
                     break;
                   }
                   num_card--;
                   //fprintf(COM2,"send_oneMSG =%d\n\r",send_oneMSG);
                   if(send_oneMSG==1)
                   {
                     num_card=0;
                     send_oneMSG=0;
                   }
                   SIM_state=MSG_sending;              
            break;
            case MSG_sending:
                  fprintf(COM2,"MSG_sending %d\n\r",MSG_sending_st);
                  //fprintf(COM2,"lastlen = %ld\n\r",lastlen);
                  
                  //fprintf(COM2,"MSG_sending_st %d\n\r",MSG_sending_st);
                  timing_wait=10000;
               output_high(LED);
               #ASM BCF    0xF94.7 #ENDASM 
               switch(MSG_sending_st)
               {
                  case 0:// configure MSG
                     receiver_state=1;
                        fprintf(COM1,"AT+CMGF=1\r\n"); // text mode
                     //fprintf(COM2,"AT+CMGF=1\r\n"); // text mode
                     strl= strlen(msg_data_sending);
                     MSG_sending_st=1;
                     if((lastlen==140)&&(msg_data_sending[lastlen]==0))
                     {
                        MSG_sending_st=0;
                        SIM_state=delete_MSG;
                        break;
                     }
                     //EEPROM_read(strobe_mobile_num1,20,mobilenumb); 
                  break;
                  case 1:
                        fprintf(COM1,"AT+CMGS=");
                     //fprintf(COM2,"AT+CMGS=");
                     fputc('"',COM1);
                     i=0;
                     while((mobilenumb[i]!=0)&&(i<20))
                     {
                        fputc(mobilenumb[i],COM1);
                        //fprintf(COM2,"%c",mobilenumb[i]); 
                        i++;
                     }
                     fputc('"',COM1);
                     fprintf(COM1,"\r\n");
                     MSG_sending_st=2;
                  break;
                  case 2:
                     //while((msg_data_sending[i]!=0)&&(i<140))
                     for(i=0;i<140;i++)
                     {
                        if(msg_data_sending[i+lastlen]==0)
                        {
                           SIM_state=delete_MSG;   
                           MSG_sending_st=0;
                           fputc(26,COM1);
                           break;
                        }
                        fputc(msg_data_sending[i+lastlen],COM1);
                     }
                     //fprintf(COM2,"i= %u\n\r",i); 
                     fputc(26,COM1);
                     MSG_sending_st=0;
                     if(i<140)
                     {
                        SIM_state=check_st;
                        lastlen=0;
                     }
                     else
                     {
                        lastlen=140;
                        if(msg_data_sending[140]==0)SIM_state=check_st;
                     }
                     if(lastlen==140)SIM_state=MSG_sending;
                     //fprintf(COM2,"lastlen %lu\n\r",lastlen);  
                  break;
               }
               #ASM BSF    0xF94.7 #ENDASM 
            break;
            case check_st:
               //fprintf(COM2,"check_st->num_card,countc, count_dwla, lastlen =%lu,%lu,%lu,%lu\r\n",num_card,countc,count_dwla,lastlen);  
               if(count_dwla>1)
               {
                  count_dwla--;
                  SIM_state=pre_cmd;
                  break;
               }
               else if(countc>0) 
               {
                  SIM_state=pre_cmd;
                  break;
               }
               else SIM_state=delete_MSG;
            break;
            case autosending: 
                  //auto_sending=read_ext_eeprom(strobe_autosending);
                  //if(auto_sending==1)
                  //{
                     EEPROM_read(strobe_mobile_num1,20,mobilenumb); 
                     #if defined(twonumber) 
                        fprintf(COM2,"\r\nSending to the first number\r\n");
                     #else 
                        fprintf(COM2,"\r\nSending\r\n");
                     #endif
                     num_card=get_countcard();
                     countc=0;
                     count_dwla=0;
                     lastlen=0;
                     send_oneMSG=1;
                     SIM_state=pre_cmd;
                  //}
                  //else SIM_state= reading_sms_cmd;
            break;
            #if defined(twonumber) 
            case autosending1: 
                  //auto_sending=read_ext_eeprom(strobe_autosending);
                  //if(auto_sending==1)
                  //{
                     EEPROM_read(strobe_mobile_num0,20,mobilenumb); 
                     if((mobilenumb[0]==0)&&(mobilenumb[1]==0)&&(mobilenumb[2]==0)&&(mobilenumb[3]==0)&&(mobilenumb[5]==0)&&(mobilenumb[7]==0)&&(mobilenumb[9]==0))
                     {
                        send_oneMSG=0;
                        fprintf(COM2,"\r\nthe 2nd number is not installed\r\n");
                        SIM_state=delete_MSG;   
                     }
                     else
                     {
                        fprintf(COM2,"\r\nSending to the second number\r\n");
                        send_oneMSG=1;
                        num_card=get_countcard();
                        SIM_state=pre_cmd;
                     }                     
                     countc=0;
                     count_dwla=0;
                     lastlen=0;                     
                  //}
                  //else SIM_state= reading_sms_cmd;
            break;
            #endif
            case idle:
                  output_toggle(LED);
                  fprintf(COM2,"idle\n\r");
                  fprintf(COM1,"AT+CMGF=1\r\n");
                  output_low(LED);
                  SIM_state= data_analyze;
                  receiver_state=0;
                  timing_wait=1500;
            break;
            case data_analyze:
            
                  /*use for debug*/
                  /*fprintf(COM2,"\n\rcard_timeout %lu\n\r",card_timeout);
                  fprintf(COM2,"\n\rbug_countbit_T1 %lu\n\r",bug_countbit_T1);
                  fprintf(COM2,"\n\rbug_countbit_T2 %lu\n\r",bug_countbit_T2);*/                  
                  /*use for debug*/
                  //fprintf(COM2,"data_analyze=%d\n\r",temp);
                  booting_done=1;
                  output_low(LED);
                  temp=SMS_coming();
                  if(temp==2)
                  {
                     output_high(LED);
                     SIM_state=reading_sms_cmd;
                     count_checking=0;
                  }
                  count_checking++;
                  if(count_checking==20)
                  {
                     //SIM_state=phone_activity_status;
                     SIM_state=check_comunication;
                     count_checking=0;
                  }
            break;
            case SIM_reply:
                  fprintf(COM2,"SIM is replying\n\r");
                  //send_sms(mobilenumb,reply_buffer);
                  len=0;
                  cards=get_countcard();
                  //strcpy(msg_buffer2,"device has ");
                  len=pre_msg(reply_buffer,msg_data_sending,0);
                  //itoa(cards,10,string);
                  //len=pre_msg(string,msg_data_sending,len);
                  //strcpy(msg_buffer2," transaction ");
                  //len=pre_msg(msg_buffer2,msg_data_sending,len);
                  SIM_state=MSG_sending;
                  lastlen=0;
                  //fprintf(COM2,"device has %lu transaction \n\r",cards);
                  countc=0;
                  count_dwla=0;
            break;
            case check_comunication:
               fprintf(COM2,"check comunication\n\r");
               //SIM_state= data_analyze;
               SIM_state= phone_activity_status;
               booting_done=0;
               checking_sim=1;
               setup_wdt(WDT_ON);
               fprintf(COM1,"AT\r\n");  
            break;
            case phone_activity_status:
               booting_done=1;
               fprintf(COM2,"SIM activity status\n\r");
               SIM_state= data_analyze;
               fprintf(COM1,"AT+CPAS\r\n"); 
            break;
            case check_inbox:
               fprintf(COM2,"check inbox\n\r");
               SIM_state= data_analyze;
               fprintf(COM1,"AT+CMGR=1\r\n");
            break;
            case reset_module:
               setup_wdt(WDT_ON);
               fprintf(COM2,"reset_module\n\r");
            break;
         }        
      }
      //restart_wdt();
      delay_us(500);
     #else
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
     #endif    
   }
}
