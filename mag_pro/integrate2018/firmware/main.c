
#define LED   PIN_A6
#include "main.h"
#include "stdlib.h"
#include "24512.C"
#include <eeprom.c>

#include <scan_key.c>
#include <uart_tool.c>
#include "mcr.c"
#include "mode.c"

//==========================
#INT_TIMER0
void timer0()
{
   int16 buf;
   int8 temp;
   
   //========strobe keyboarddata============
   //=======================================  
   //output_toggle(ROW0);
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
      if(reading_fg==1)
      {
          if(cryption_enable==0) 
          {
                saving_card();
          }
          else 
          {
                saving_card_encrypt();
          }
      }
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
         if(cryption_enable==0) 
         {
            save_key_new();
         }
         else
         {
            save_key_encrypt();
         }
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
   RPINR1=5;
   charac_timeout=0xffffffff;
   setup_oscillator(OSC_32MHZ);
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
   //=====================================
   delay_ms(1000);
   init_password();
   //=====================================
   
   ptr_card=get_ptrcard(strobe_ptrcard_addr);
   printf(COM2," save_ptrcard=%lu\n\r",ptr_card);
   
   ptr_card_key=get_ptrcard(strobe_ptrcard_key);
   fprintf(COM2,"booting\n\r");
   fprintf(COM2," save_ptrcard_key=%lu\n\r",ptr_card_key);
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
   //==========================================
   charac_timeout=0xffffffff;
   
   EEPROM_read(strobe_nameconsole,wide_strobe_nameconsole,console);
   set_tris_a(0xff); 
   
   setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN4|VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   
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
#if defined(nocheckpass)
   for(i=0;i<20;i++)
   {
      password[i] = 0;
   }
#endif
#if 0
   disable_interrupts(GLOBAL);
   save_coutcard(0);
   //save_ptrcard(0,strobe_ptrcard_key);
   //save_ptrcard(0,strobe_ptrcard_addr);
   //write_ext_eeprom(0xFFFE,0x0F);
   //fprintf(COM2,"eeprom is testing: %x",read_ext_eeprom(0x10000));
   strcpy(key_data,"123456");
   memset(crypto_key,0,sizeof(crypto_key)); 
   strcpy(crypto_key,"quang102");
   
   save_key_encrypt();
  /*  for(i=0;i<8;i++)
   {
       fprintf(COM2,"%c",key_data[i]);  
   } */
   //for(i=0;i<8;i++) fprintf(COM2,"%c",crypto_key[i]);
   
   EEPROM_read(EEPROM_KEY_ST,16,buftemp2);
   memset(crypto_key,0,sizeof(crypto_key)); 
   strcpy(crypto_key,"quang102");
   //for(i=0;i<16;i++) fprintf(COM2," %x",buftemp2[i]);
   delay_ms(3000);
   
   fprintf(COM2,"\n\r");
   fprintf(COM2,"test_data in:\n\r");
   for(i=0;i<16;i++) fprintf(COM2,"%x",buftemp2[i]);
   fprintf(COM2,"\n\r");
   fprintf(COM2,"test_key in:\n\r");
   for(i=0;i<16;i++) fprintf(COM2,"%x",crypto_key[i]);
   //strcpy(crypto_key,"@quang102");
   //rijndael('d', (unsigned int8 *)&test_data[0], (unsigned int8 *)&test_key[0]);
   aes_enc_dec((unsigned int8 *)&buftemp2[0], (unsigned char *)&crypto_key[0],1);
   //aes_enc_dec((unsigned int8 *)&test_data[0], (unsigned int8 *)&test_key[0],1);
   fprintf(COM2,"\n\r");
   fprintf(COM2,"\n\rdecrypted:\n\r");
   for(i=0;i<16;i++) fprintf(COM2,"%x",buftemp2[i]);
   fprintf(COM2,"\n\r");
   fprintf(COM2,"\n\rcrypto_key:\n\r");
   for(i=0;i<16;i++) fprintf(COM2,"%x",crypto_key[i]);
   while(1);
#endif
#if 0
    fprintf(COM2,"\n\reeprom test\n\r");
    fprintf(COM2,"\n\reeprom is writing\n\r");
    for(i=0;i<16;i++)
    {
        //write_ext_eeprom(115535+i,i);
    }
    write_ext_eeprom(EEPROM_KEY_ST,210);
    write_ext_eeprom(EEPROM_SIZE_endofkey,123);
    fprintf(COM2,"\n\r");
    fprintf(COM2,"eeprom is reading: %u",read_ext_eeprom(EEPROM_KEY_ST));
    for(i=0;i<16;i++)
    {
        fprintf(COM2," %u",read_ext_eeprom(115535+i));
    }
    while(1);
#endif
   EEPROM_read(strobe_pass_addr,20,password);
   cryption_enable = read_ext_eeprom(crypto_en);
   if(cryption_enable!=0)
   {
        fprintf(COM2,"\n\crypto is enable\n\r");
        EEPROM_read(strobe_crypto_key,CRYPTO_KEY_SIZE,crypto_key); 
   }
   else
   {
       fprintf(COM2,"\n\crypto is disable\n\r");
   }
#if 0   
   fprintf(COM2,"\n\crypto key:\n\r");
   for(i=0;i<CRYPTO_KEY_SIZE;i++)
   {
        fprintf(COM2,"%c",crypto_key[i]);
   }
   fprintf(COM2,"\n\r");
#endif   
   while(1)
   {
      if(mode==LOGOFF)
      {
         fprintf(COM2,"password:");
         //fprintf(COM2,"\n\r charac_timeout=%lu\n\r",charac_timeout);
         USART_getstring(STAR_ECHO,20,buffer_uart);
        // for(i=0;i<5;i++) fprintf(COM2," %c",password[i]);
         /*fprintf(COM2,"\n\r");
         for(i=0;i<5;i++) fprintf(COM2," %x",read_ext_eeprom(i));
         fprintf(COM2,"\n\r");
         for(i=0;i<5;i++) fprintf(COM2," %x",password[i]);
         fprintf(COM2,"\n\r");
         for(i=0;i<20;i++) fprintf(COM2," %x",buffer_uart[i]);//*/
         EEPROM_read(strobe_pass_addr,20,password);
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
