#include "STRING.H"
//#include "myRS485.C"

#define        EMERGOFF       PIN_A7
#define        reading_sms_cmd      1
#define        MSG_avai             2
#define        analyze_sms          3
#define        checking_MN          4
#define        doing_sms            5
#define        delete_MSG           6
#define        pre_cmd              7
#define        MSG_sending          8
#define        check_st             9
#define        config               10
#define        autosending          11
#define        idle                 12
#define        data_analyze         13
#define        SIM_reply            14
#define        check_comunication   15
#define        phone_activity_status 16
#define        check_inbox          17
#define        checking_MN0         18
#define        autosending1         19
//#define     IGT            PIN_A6

//#define     batmb    sendto485(0,0x11,1)
//#define     tatmb    sendto485(0,0x11,0)


unsigned int8 msg_data_sending[280];
int8 sim_bug[50];
unsigned int8 msg_buffer1[20];
unsigned int8 msg_buffer2[50];
unsigned int8 reply_buffer[280];
unsigned int8 datetime[25];
unsigned int8 mobilenumb[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//unsigned int8 buf_checkMBN[16];
int16 count_polling=0;
int8 SIM_state=0,MSG_sending_st=0;
int16 timing_wait=300;
int8 auto_sending=0;
int8 send_oneMSG=0;
int8 SIM_reply_st=0;
int8 checking_sim=0;
int16 count_checking=90;
#if defined(twonumber)
  int8 two_number_sending=0;       
#endif
//unsigned int8 com_buffer1[20]="it is OK";
//==============================================
void earse_buffer_uart(unsigned int8 numbuf)
{
   unsigned int8 i;
   for(i=0;i<numbuf;i++)
   {
       buffer_uart[i]=0;
   }
}
//==============================================
void sim900_init()
{
   fprintf(COM1,"AT\r\n");
   delay_ms(500);
   fprintf(COM1,"AT+CMGF=1\r\n");
   delay_ms(500);
   //del_buf(26,mobilenumb);
   //strcpy(mobilenumb,"+84903219082");
}
//==============================================
int8 get_datasim900(int8 *datin)
{
   int8 c,i,index=0;
   int8 testtemp[20];
   while(index<5)
   {
      c = getc();
      index++;
      fprintf(COM2,"%c", c); 
   }
   for(i=0;i<15;i++)
   {
      fprintf(COM2,"%c", testtemp[i]);
   }
   fprintf(COM2,"%u\r\n", index);//*/
   return 0;
}
//==============================================
int8 check_module()
{
   int8 temp=0,i;
   earse_buffer_uart(10);
   fprintf(COM1,"AT\r\n");
   delay_ms(200);
   fprintf(COM1,"AT\r\n");
   delay_ms(200);
   fprintf(COM1,"AT\r\n");
   delay_ms(200);
   fprintf(COM1,"AT\r\n");
   delay_ms(200);
   fprintf(COM1,"AT\r\n");
   UART_index=0;
   delay_ms(200);
   //fputc('c',COM2);
   //fprintf("MSG is OK\r\n");
   for(i=0;i<10;i++)
   {
      //fputc(buffer_uart[i],COM2);
      if((buffer_uart[i]=='O')&&(buffer_uart[i+1]=='K'))
      {
         i=10;
         temp=1;
         return(temp);
      }
         else temp=0;
   }
   UART_index=0;
   return(temp);
}
//==============================================
void power_on()
{
   /*while(temp==0)
   {
      temp = check_module();
      if(temp==0)
      {
         output_high(EMERGOFF);
         delay_ms(3000);
         output_low(EMERGOFF);
         delay_ms(3000);
      }
         else break;
   }*/
         output_high(EMERGOFF);
         delay_ms(3000);
         output_low(EMERGOFF);
         delay_ms(3000);
}
//==============================================
void power_off()
{
   fprintf(COM1,"AT+CPOWD\r\n");
}
//==============================================
unsigned int8 mymemcmp(void * s1,char *s2,size_t n)
{
   char *su1, *su2;
   unsigned int8 temp=0,i=0;
   for(su1=s1, su2=s2; 0<n; ++su1, ++su2, --n)
   {
      if(*su1!=*su2)
      {
         temp=1;
         if(*su1>*su2)
         {
            if((i==0)&&(*su1==(*su2)+32)) temp = 0;
               //else temp = 0;
         }
            else 
               {
                  if((i==0)&&(*su2==(*su1)+32)) temp = 0;
                    // else temp = 0;
               }
      }
      i++;
   }
   return (temp);
}
//==============================================
int8 pre_msg(int8 *datin,int8 *datout,int16 index)
{
   int8 i,len=0;
   len = strlen(datin);
   for(i=0;i<len;i++)datout[index++]=datin[i];
   return index;
}
//==============================================
void delete_sms(char number_sms)
{
   switch(number_sms)
   {
       case  1:
            fprintf(COM1,"AT+CMGD=1\r\n"); 
       break;
       case  2:
            fprintf(COM1,"AT+CMGD=2\r\n");
       break;
       case  3:
            fprintf(COM1,"AT+CMGD=3\r\n");
       break;
       case  4:
            fprintf(COM1,"AT+CMGD=4\r\n");       
       break;
       case  5:
            fprintf(COM1,"AT+CMGD=5\r\n");
       break;
       case  6:
            fprintf(COM1,"AT+CMGD=6\r\n");
       break;
       case  7:
             fprintf(COM1,"AT+CMGD=7\r\n");
       break;       
       case  8:
            fprintf(COM1,"AT+CMGD=8\r\n");
       break;       
   }
   delay_ms(500);
}
//==============================================
void del_multi_sms(int8 num_sms)
{
   int8 i;
   for(i=1;i<=num_sms;i++)
   {
     delete_sms(i); 
     delay_ms(100);
   }
}
//==============================================
//==============================================
void send_sms(unsigned int8* mobile_num,unsigned int8* MSG)
{
      unsigned int8 i=0,strl=0;
      strl= strlen(MSG);  
      fprintf(COM1,"AT+CMGS=");
      delay_ms(500);
      fputc('"',COM1);
      while((mobile_num[i]!=0)&&(i<20))
      {
         //fputc(mobile_num[i],COM2);
         //putc(mobile_num[i]);
         fputc(mobile_num[i],COM1);
         i++;
      }
      fputc('"',COM1);
      fprintf(COM1,"\r\n");
      delay_ms(500);        
      for(i=0;i<strl;i++)
      {
         fputc(MSG[i],COM1);
      }
      delay_ms(500);
      fputc(26,COM1);
}
//=============================================
//==============================================
void reading_sms()
{
   unsigned int8 i;
   for(i=0;i<16;i++) mobilenumb[i]=0;
   earse_buffer_uart(200);
   printf("AT+CMGF=1\r\n");
   printf("AT+CMGR=1\r\n"); 
   UART_index=0; 
   delay_ms(1000);
   for(i=0;i<200;i++)
   {
      if((buffer_uart[i]=='a')&&(buffer_uart[i+1]=='d')&&(buffer_uart[i+2]=='m')&&(buffer_uart[i+3]=='i')&&(buffer_uart[i+4]=='n')&&(buffer_uart[i+5]==' '))
      {
         write_ext_eeprom(strobe_mobile_num1,buffer_uart[i+6]);
         fputc(buffer_uart[i+6],COM2);
         write_ext_eeprom(strobe_mobile_num1+1,buffer_uart[i+7]);
         fputc(buffer_uart[i+7],COM2);
         write_ext_eeprom(strobe_mobile_num1+2,buffer_uart[i+8]);
         fputc(buffer_uart[i+8],COM2);
         write_ext_eeprom(strobe_mobile_num1+3,buffer_uart[i+9]);
         fputc(buffer_uart[i+9],COM2);
         write_ext_eeprom(strobe_mobile_num1+4,buffer_uart[i+10]);
         fputc(buffer_uart[i+10],COM2);
         write_ext_eeprom(strobe_mobile_num1+5,buffer_uart[i+11]);
         fputc(buffer_uart[i+11],COM2);
         write_ext_eeprom(strobe_mobile_num1+6,buffer_uart[i+12]);
         fputc(buffer_uart[i+12],COM2);
         write_ext_eeprom(strobe_mobile_num1+7,buffer_uart[i+13]);
         fputc(buffer_uart[i+13],COM2);
         write_ext_eeprom(strobe_mobile_num1+8,buffer_uart[i+14]);
         fputc(buffer_uart[i+14],COM2);
         write_ext_eeprom(strobe_mobile_num1+9,buffer_uart[i+15]);
         fputc(buffer_uart[i+15],COM2);
         write_ext_eeprom(strobe_mobile_num1+10,buffer_uart[i+16]);
         fputc(buffer_uart[i+16],COM2);
         write_ext_eeprom(strobe_mobile_num1+11,buffer_uart[i+17]);
         fputc(buffer_uart[i+17],COM2);
         write_ext_eeprom(strobe_mobile_num1+12,buffer_uart[i+18]);
         fputc(buffer_uart[i+18],COM2);
         write_ext_eeprom(strobe_mobile_num1+13,buffer_uart[i+19]);
         fputc(buffer_uart[i+19],COM2);
         write_ext_eeprom(strobe_mobile_num1+14,buffer_uart[i+20]);
         fputc(buffer_uart[i+20],COM2);
         write_ext_eeprom(strobe_mobile_num1+15,buffer_uart[i+21]);
         fputc(buffer_uart[i+21],COM2);
         write_ext_eeprom(strobe_mobile_num1+16,buffer_uart[i+22]);
         fputc(buffer_uart[i+22],COM2);
         write_ext_eeprom(strobe_mobile_num1+17,buffer_uart[i+23]);
         fputc(buffer_uart[i+23],COM2);
         write_ext_eeprom(strobe_mobile_num1+18,buffer_uart[i+24]);
         fputc(buffer_uart[i+24],COM2);         
         write_ext_eeprom(strobe_mobile_num1+19,buffer_uart[i+25]);
         fputc(buffer_uart[i+25],COM2);
         write_ext_eeprom(strobe_mobile_num1+20,buffer_uart[i+26]);
         fputc(buffer_uart[i+26],COM2);         
         fprintf(COM2,"\r\n");
         EEPROM_read(strobe_mobile_num1,20,mobilenumb);
         i=250;
         sms_unread_flag=0;
         strcpy (msg_data_sending, "done");
         send_sms(mobilenumb,msg_data_sending);
         delay_ms(5000);
         delete_sms(1);
         delay_ms(5000);
         UART_index=0;
      }
        else 
         {
            if(i>198) 
            {
               delay_ms(5000);
               delete_sms(1);
               delay_ms(5000);
               UART_index=0;
               sms_unread_flag=0;
               EEPROM_read(strobe_mobile_num1,20,mobilenumb);
            }
         }
   }
}
//==============================================
/*void del_buf(int8 lenght, unsigned int8 *inputdat)
{
   int8 i;
   for(i=0;i<lenght;i++)
   {
      inputdat[i]=0;
   }
}*/
//============================================================
int8 checking_msg_avai()
{
   int8 i;
   /*for(i=0;i<100;i++)
   {
      fprintf(COM2,"%c", buffer_uart[i]);
   }
   fprintf(COM2,"%u\r\n", UART_index);//*/
   for(i=0;i<20;i++)
   {
      if((buffer_uart[i]=='O')&&(buffer_uart[i+1]=='K')) return 1;
   }
   return 0;
}
//============================================================
int8 SMS_coming()
{
//+CMTI: "SM",25
   int8 i,st=0;
   //int8 msg_temp[20],msg_temp1[20];
   //strncpy(msg_temp1,buffer_uart,20);
   //strcpy(msg_temp,"+CMTI:");
   for(i=0;i<20;i++)
   {
      if((buffer_uart[0+i]=='+')&&(buffer_uart[1+i]=='C')&&(buffer_uart[2+i]=='M')&&(buffer_uart[3+i]=='T')&&(buffer_uart[4+i]=='I')&&(buffer_uart[5+i]==':')) 
      { 
         fprintf(COM2,"\n\rNew SMS\n\r");
         //output_high(PIN_C2);
         SIM_state=reading_sms_cmd;
         st = 2;
         i=23;
      }
         else st=0;
    //fprintf(COM2,"%c",'t');  
    //fprintf(COM2,"%c",buffer_uart[i]); 
   }
   for(i=0;i<20;i++)
   {
      if((buffer_uart[0+i]=='+')&&(buffer_uart[1+i]=='C')&&(buffer_uart[2+i]=='M')&&(buffer_uart[3+i]=='G')&&(buffer_uart[4+i]=='R')&&(buffer_uart[5+i]==':')) 
      { 
         fprintf(COM2,"\n\rREC UNREAD\n\r");
         //output_high(PIN_C2);
         SIM_state=reading_sms_cmd;
         st = 2;
         i=23;
      }
         else st=0;
    //fprintf(COM2,"%c",'t');  
    //fprintf(COM2,"%c",buffer_uart[i]); 
   }
   for(i=0;i<20;i++)
   {
      if((buffer_uart[0+i]=='R')&&(buffer_uart[1+i]=='I')&&(buffer_uart[2+i]=='N')&&(buffer_uart[3+i]=='G')) 
      { 
         fprintf(COM2,"\n\rRING\n\r");
         output_high(LED);
         st = 1;
         i=23;
      }
         else st=0;
   }
   for(i=0;i<20;i++)
   {
      if((buffer_uart[0+i]=='+')&&(buffer_uart[1+i]=='C')&&(buffer_uart[2+i]=='P')&&(buffer_uart[3+i]=='A')&&(buffer_uart[4+i]=='S')&&(buffer_uart[5+i]==':')) 
      { 
         if(buffer_uart[7+i]=='0') 
         {
            fprintf(COM2,"Ready (MT allows commands from TA/TE)\n\r");
            SIM_state= check_inbox;
         }
         if(buffer_uart[7+i]=='2')
         {
            fprintf(COM2,"No network\n\r");
            SIM_state=check_comunication;
         }
         if(buffer_uart[7+i]=='3') fprintf(COM2,"Ringing\n\r");
         if(buffer_uart[7+i]=='4') fprintf(COM2,"Call in progress\n\r");
         st = 3;
         i=23;
      }
         else st=0;
   }
   for(i=0;i<20;i++)
   {
      if((buffer_uart[0+i]=='U')&&(buffer_uart[1+i]=='N')&&(buffer_uart[2+i]=='D')&&(buffer_uart[3+i]=='E')&&(buffer_uart[4+i]=='R')&&(buffer_uart[5+i]=='-')&&(buffer_uart[6+i]=='V')&&(buffer_uart[7+i]=='O')&&(buffer_uart[8+i]=='L')) 
      { 
         fprintf(COM2,"\n\rUNDER-VOLTAGE WARNNING\n\r");
         SIM_state=reading_sms_cmd;
         st = 4;
         i=23;
      }
         else st=0;
   }
   for(i=0;i<20;i++)
   {
      if((buffer_uart[0+i]=='O')&&(buffer_uart[1+i]=='V')&&(buffer_uart[2+i]=='E')&&(buffer_uart[3+i]=='R')&&(buffer_uart[4+i]=='-')&&(buffer_uart[5+i]=='V')&&(buffer_uart[6+i]=='O')&&(buffer_uart[7+i]=='L')&&(buffer_uart[8+i]=='T')) 
      { 
         fprintf(COM2,"\n\rOVER-VOLTAGE POWER DOWN\n\r");
         SIM_state=reading_sms_cmd;
         st = 4;
         i=23;
      }
         else st=0;
   }

   UART_index=0;
   memset(buffer_uart,0,sizeof(buffer_uart)); 
   return st;
}
//============================================================
unsigned int8 analyzing_msg(char* MSG,char* mobile_number)
{
   unsigned int8 i=0,a=0,b=0,d=0,u=0,v=0;
   unsigned int8 st=0,temp;
   del_buf(50,MSG);
   del_buf(20,mobile_number);
   for(i=0;i<100;i++)
   {
      if((buffer_uart[i]=='+')&&(buffer_uart[i+1]=='C')&&(buffer_uart[i+2]=='M')&&(buffer_uart[i+3]=='G')&&(buffer_uart[i+4]=='R')&&(buffer_uart[i+5]==':')&&(buffer_uart[i+6]==' ')&&(buffer_uart[i+7]=='0')&&(buffer_uart[i+8]==',')&&(buffer_uart[i+9]==',')&&(buffer_uart[i+10]=='0')) 
      {
         st=0;
         i=200;
         del_buf(100,MSG);
         del_buf(16,mobile_number);
         count_spam=0;
         //output_toggle(pin_a5);
      }
         //else st=2;
     temp = buffer_uart[i];
     if((a==3)) 
     {
       if((temp>47)&&(temp<58)||temp=='+') mobile_number[i-b] = temp;
     }
     if(d==2)
     {
         MSG[i-b] = temp;
         st=1;
     }
     if(u==2)
     {
         datetime[i-v]=temp;
     }
      //else st=2;
     if(temp==34) 
     {
         if(a<6)a++;
         if(a==3) b = i+1;
     }
     if(a==6)
     {
         if((temp==13)||(temp==10)) 
         {
            d++;
            if(d==2) b = i+1;
         }
         if(temp==34) 
         {
            u++;
            if(u==2)v=i+1;
         }
     }
     //if((temp==',')&&(a==6))
    // {
         //if(c<2)c++;
     //}
   }
   return(st);
}
//================================
int8 ascii2int(int8 num1,int8 num2)
{
   int8 temp;
   temp=(num1-48)*10 + (num2-48);
   return temp;
}
//================================
void updatetime2ds1307()
{
   int8 temp;
   year=ascii2int(datetime[0],datetime[1]);
   mon=ascii2int(datetime[3],datetime[4]);
   date=ascii2int(datetime[6],datetime[7]);
   h=ascii2int(datetime[9],datetime[10]);
   min=ascii2int(datetime[12],datetime[13])+1;
   //h=ascii2int(datetime[9],datetime[10]);
   rtc_set_datetime(date,mon,year,day,h,min);
}
//================================
