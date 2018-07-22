

#include "STRING.H"

#define strobe_pass_addr            0
#define strobe_ptrcard_addr         25     // 4 bytes
#define strobe_ptrcard_key          30     // 4 bytes
#define strobe_count_card           36     // 2 bytes
#define strobe_nameconsole          40     // 16 bytes 
#define strobe_Master_SLV           58     // 1 byte
#define strobe_debugmode            59     // 1 byte
#define strobe_delaytime            60     // 1 byte
#define kindofKB                    61     // 1 byte
#define crypto_en                   62     // 1 byte
#define strobe_crypto_key           63     // 32 byte


#define wide_strobe_nameconsole     16 
//==============================================================
#define numdata                  124 // 5+79+40=124
#define countcards               930   //43590/124
//===============================================================

unsigned int8 temp[5];
unsigned int8 buffer1[20];
unsigned int8 buffer2[20];
unsigned int8 password[20];
unsigned int8 entpassword[20];
unsigned int8 rec[numdata];
int16 countcard=0;
int16 charac_timeout=0xffff;
//int8 twonumber=0;
//==============================================
void del_buf(int8 lenght, unsigned int8 *inputdat)
{
   int8 i;
   for(i=0;i<lenght;i++)
   {
      inputdat[i]=0;
   }
}
//===============================================================
int8 EEPROM_write(unsigned int32 adr, unsigned int32 len, int8 *buf)
{
   unsigned int32 i;
   for(i=0;i<len;i++)
   {
      write_ext_eeprom(adr+i, buf[i]);
      delay_us(10);
   }
   return 1;
}
//===============================================================
void EEPROM_read(unsigned int32 adr, unsigned int32 len, int8 *buf)
{
   unsigned int32 i;
   for(i=0;i<len;i++)
   {
      buf[i]=read_ext_eeprom(adr+i);
      delay_us(10);
   }
}
//================================================================
unsigned int32 get_ptrcard(int8 addr)
{
   unsigned int32 buffer=0;
   unsigned int32 temp1=0,temp2=0,temp3=0,temp4=0;
   temp1 = (unsigned int32)read_ext_eeprom(addr+0)&0x000000ff;
   temp2 = (unsigned int32)read_ext_eeprom(addr+1);
   temp2<<=8;
   temp3 = (unsigned int32)read_ext_eeprom(addr+2);
   temp3<<=16;
   temp4 = (unsigned int32)read_ext_eeprom(addr+3);
   temp4<<=24;
   buffer = temp4|temp3|temp2|temp1;
   return(buffer);
}
//=======================================================
void save_ptrcard(unsigned int32 ptr,int8 addr)
{
   unsigned int32 buffer=0;
   unsigned int8 temp1=0,temp2=0,temp3=0,temp4=0;
   buffer=ptr;
   temp1 = (unsigned int8)buffer&0x000000ff;
   temp2 = (unsigned int8)(buffer>>8)&0x000000ff;
   temp3 = (unsigned int8)(buffer>>16)&0x000000ff;
   temp4 = (unsigned int8)(buffer>>24)&0x000000ff;
   write_ext_eeprom((addr+0),temp1);
   write_ext_eeprom((addr+1),temp2);
   write_ext_eeprom((addr+2),temp3);
   write_ext_eeprom((addr+3),temp4);
}
//=============================================
void save_coutcard(unsigned int16 ptr)
{
   unsigned int16 buffer=0;
   unsigned int8 temp1=0,temp2=0;
   buffer=ptr;
   temp1 = (unsigned int8)buffer&0x000000ff;
   temp2 = (unsigned int8)(buffer>>8)&0x000000ff;
   write_ext_eeprom((strobe_count_card+0),temp1);
   write_ext_eeprom((strobe_count_card+1),temp2);
}
//=============================================
unsigned int16 get_countcard()
{
   unsigned int16 buffer=0;
   unsigned int16 temp1=0,temp2=0;
   temp1 = (unsigned int32)read_ext_eeprom(strobe_count_card+0)&0x000000ff;
   temp2 = (unsigned int32)read_ext_eeprom(strobe_count_card+1);
   temp2<<=8;
   buffer = temp2|temp1;
   return(buffer);
}
//===========================================
void format_eepromext()
{
   int32 i;
   //fprintf(COM2," ptr_card_key=%lu\n\r",ptr_card_key);
   /*for(i=EEPROM_KEY_ST;i<ptr_card_key;i++)
   {
     write_ext_eeprom(i,0);
     //fprintf(COM2," i=%lu\n\r",i);
   }*/
   //fprintf(COM2," ptr_card=%lu\n\r",ptr_card);
   /*for(i=ptr_start;i<ptr_card;i++)
   {
     write_ext_eeprom(i,0);
   }*/
   //fprintf(COM2," countcard=%lu\n\r",countcard);
   ptr_card=ptr_start;
   ptr_card_key=EEPROM_KEY_ST;
   save_ptrcard(ptr_card,strobe_ptrcard_addr);
   save_ptrcard(ptr_card_key,strobe_ptrcard_key);
   save_coutcard(0);
   countcard=get_countcard();
   /*fprintf(COM2," save_ptrcard=%lu\n\r",get_ptrcard(strobe_ptrcard_addr));
   fprintf(COM2," save_ptrcard_key=%lu\n\r",get_ptrcard(strobe_ptrcard_key));
   fprintf(COM2," countcard=%lu\n\r",countcard);*/
}
//============================================
void ease_eeprom()
{
   int32 i;
   ptr_card=ptr_start;
   ptr_card_key=EEPROM_KEY_ST;
   save_ptrcard(ptr_card,strobe_ptrcard_addr);
   save_ptrcard(ptr_card_key,strobe_ptrcard_key);
   save_coutcard(0);
}
//===========================================
void init_password()
{
      EEPROM_read(strobe_pass_addr,20,password);
      if((password[0]==0xff)&&(password[1]==0xff)&&(password[15]==0xff))
      {
         memset(buffer1,0,20);
         strcpy(buffer1,"admin");
         EEPROM_write(strobe_pass_addr,20,buffer1);
         EEPROM_read(strobe_pass_addr,20,password);
      }
}
//=============================================
void reset_password()
{
   int8 buf[20],i;
   for(i=0;i<20;i++)buf[i]=0xff;
   EEPROM_write(strobe_pass_addr,20,buf);
}
//============================================