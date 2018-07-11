

#include "STRING.H"
#define strobe_pass_addr         0
#define strobe_ptrcard_addr      25// from 21 to 24 use for stobe point
#define strobe_ptrcard_key       30// from 21 to 24 use for stobe point
//#define strobe_password          35
#define strobe_count_card        40
//#define strobe_ptrcard_addr      69// from 21 to 24 use for stobe point
#define strobe_count_num         65   //  num1   num2  num3 num4
#define strobe_mobile_num1       85 // 1->16// ->34//-> 50 -> 66
#define strobe_nameconsole       102 // 102+16
//#define strobe_mobile_num2       105 //18-34  
//#define strobe_mobile_num3       125


#define strobe_Master_SLV        145
#define strobe_debugmode         146
#define strobe_autosending       147
#define strobe_delaytime         148
#define strobe_delaykb           149
#define KB_time                  126
#define strobe_keypressmode      127


//#define ptr_start                150

//#define EEPROM_KEY_ST      43590+1

//#define install_password         0xfe
//==============================================================
#define numdata                  124 // 5+79+40=124
//===============================================================

unsigned int8 temp[5];
unsigned int8 buffer1[20];
unsigned int8 buffer2[20];
unsigned int8 password[20];
unsigned int8 entpassword[20];
unsigned int8 rec[numdata];
int16 countcard=0;
int16 charac_timeout=0xffff;
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
void EEPROM_write(unsigned int adr, unsigned int len, char *buf)
{
   unsigned int8 i;
   for(i=0;i<len;i++)
   {
      write_ext_eeprom(adr+i, buf[i]);
      delay_us(10);
   }
}
//===============================================================
void EEPROM_read(unsigned int adr, unsigned int len, char *buf)
{
   unsigned int8 i;
   for(i=0;i<len;i++)
   {
      buf[i]=read_ext_eeprom(adr+i);
      delay_us(10);
   }
}
//================================================================
/*void default_pass()
{
   unsigned int8 temp;
   temp = read_program_eeprom(strobe_password);
   if(temp==0xff)
   {
      EEPROM_write(strobe_password,11,defaul_password);
   }
}*/
//================================================================
/*int8 check_default_password()
{
   int8 st=0;
   EEPROM_read(strobe_password,11,buf_checkpass);
   //strcpy (command1,"Den 6 tat");
   if(!memcmp(buf_checkpass,defaul_password,11))st=1;
      else st=0;
   return(st);
}*/
//================================================================
/*void save_mobile_number(int8 count,unsigned int8 *mb_num)
{
   //strobe_count_num
   unsigned int8 i=0,temp;
   for(i=0;i<16;i++)
   {
      temp = mb_num[i];
      if((temp<48)||(temp>57))
      {
         if(temp!='+') mb_num[i]=0;
      }
   }
   switch (count)
   {
      case 0:
         EEPROM_write(strobe_mobile_num1,16,mb_num);
      break;
      case 1:
         EEPROM_write(strobe_mobile_num2,16,mb_num);
      break;
      case 2:
         EEPROM_write(strobe_mobile_num3,16,mb_num);
      break;      
      case 3:
         //EEPROM_write(strobe_mobile_num4,16,mb_num);
      break;      
   }
}*/
//=====================================================
/*void save_password(unsigned int8 lenght, unsigned int8 *pass_dat)
{
   EEPROM_write(strobe_password,lenght,pass_dat);
}*/
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
   int16 i;
   for(i=EEPROM_KEY_ST;i<ptr_card_key;i++)
   {
     write_ext_eeprom(i,0);
   }
   for(i=ptr_start;i<ptr_card;i++)
   {
     write_ext_eeprom(i,0);
   }
   ptr_card=ptr_start;
   ptr_card_key=EEPROM_KEY_ST;
   save_ptrcard(ptr_card,strobe_ptrcard_addr);
   save_ptrcard(ptr_card_key,strobe_ptrcard_key);
   save_coutcard(0);
   countcard=get_countcard();
   //fprintf(COM2," save_ptrcard=%lu\n\r",get_ptrcard(strobe_ptrcard_addr));
   //fprintf(COM2," save_ptrcard_key=%lu\n\r",get_ptrcard(strobe_ptrcard_key));
   //fprintf(COM2," countcard=%lu\n\r",countcard);
}
//============================================
void ease_eeprom()
{
   int16 i;
   for(i=ptr_start;i<EEPROM_SIZE_key;i++)
   {
     write_ext_eeprom(i,0);
   }
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

