
/*#define MCR_CARD_PRESENT   PIN_B0
#define MCR_DATA1          PIN_B1
#define MCR_STROBE1        PIN_B2
#define MCR_DATA2          PIN_B4
#define MCR_STROBE2        PIN_B5*/


#byte   RPINR1 =   getenv("SFR:RPINR1")
#byte   RPINR2 =   getenv("SFR:RPINR2")

#define MCR_CARD_PRESENT   PIN_B0
#define MCR_DATA1          PIN_B1
#define MCR_STROBE1        PIN_B2
#define MCR_DATA2          PIN_C5
#define MCR_STROBE2        PIN_B3

// The following define the error codes.
// To check for a specific error, check
// the cooresponding bit in the value
// returned by mcr_read().
#define MCR_ERR_PARITY1 1
#define MCR_ERR_PARITY2 2
#define MCR_ERR_LRC1    4
#define MCR_ERR_LRC2    8

#define numbyteoftrack1 79
#define numbyteoftrack2 40
#define numbyteofbuffer1    850
#define numbyteofbuffer2    300
//#define numbyteofbuffer    100
//=====================================
unsigned int8 day,year=14,mon=1,date=1,h=0,min=0,sec=0;
//=====================================
char Track1[numbyteoftrack1];
char Track2[numbyteoftrack2];

char buffertrack1[numbyteofbuffer1];
char buffertrack2[numbyteofbuffer2];
//char temptrack1[numbyteofbuffer1];
//char temptrack2[numbyteofbuffer2];

int32 mcr_timeout=0;
int8 dir=0;
int16 countbit_T1=0,countbit_T2=0;
int16 bug_countbit_T1=0,bug_countbit_T2=0;
int16 card_timeout=65000;
int16 countbit=0;
int8 datinbuf=0;
int8 debugmode=0;
int8 cardread_st=0;
int8 saving_flag=0;
int8 data_avai=0;
int8 reading_fg=0;
int8 count_reading_error=0;
//============================
int8 makebyte(int8 bit7,int8 bit6,int8 bit5,int8 bit4,int8 bit3,int8 bit2,int8 bit1,int8 bit0)
{
   int8 temp=0;
   temp=bit7<<7|bit6<<6|bit5<<5|bit4<<4|bit3<<3|bit2<<2|bit1<<1|bit0<<0;
   return temp;
}
//=========================
int8 check_lrc_T1(int8 direct,int16 offset,int8 *datain)
{
   int8 temp=0;
   if(direct==0)
   {
      temp=!(datain[offset]^datain[offset+1]^datain[offset+2]^datain[offset+3]^datain[offset+4]^datain[offset+5]);
      if(temp==datain[offset+6]) return 1;
         else return 0;
   }
      else 
      {
         temp=!(datain[offset]^datain[offset-1]^datain[offset-2]^datain[offset-3]^datain[offset-4]^datain[offset-5]);
         if(temp==datain[offset-6]) return 1;
            else return 0;         
      }
}
//=========================
int8 check_lrc_T2(int8 direct,int16 offset,int8 *datain)
{
   int8 temp=0;
   if(direct==0)
   {
      temp=!(datain[offset]^datain[offset+1]^datain[offset+2]^datain[offset+3]);
      if(temp==datain[offset+4]) return 1;
         else return 0;
   }
   else
   {
      temp=!(datain[offset]^datain[offset-1]^datain[offset-2]^datain[offset-3]);
      if(temp==datain[offset-4]) return 1;
         else return 0;               
   }
}
//=========================
int16 detect_pos_st(int8 track,int16 end_point,int8 *datin)
{
   int16 i,j;
   int8 temp=0,temp1=0,temp2=0;
   unsigned int8 CRC=0;
   for(i=0;i<end_point/4;i++)
   {
      if(track==0)
      {
         temp = makebyte(0,datin[i+6],datin[i+5],datin[i+4],datin[i+3],datin[i+2],datin[i+1],datin[i]);
         temp1= makebyte(0,datin[i+13],datin[i+12],datin[i+11],datin[i+10],datin[i+9],datin[i+8],datin[i+7]);
         /*CRC = !(datin[i+19]^datin[i+18]^datin[i+17]^datin[i+16]^datin[i+15]^datin[i+14]);
         if(datin[i+20]== CRC)check_crc=1;
            else check_crc=0;*/
         if((temp==0b01000101)&&(temp1!=0b01111100)&&(temp1!=0b00111110)&&(temp1!=0b00011111))//(temp==0b01000101)&&(temp1!=0b01111100)&&(temp1!=0b00111110)&&(temp1!=0b00011111)
         {
            j=i;
            CRC=1;
            temp2=temp;
            //fprintf(COM2,"temp,j=%d,%lu\r\n",temp,j);
            while((temp2!=0b00011111)&&(j<1000))
            {
               //CRC=CRC^temp2;
               CRC=check_lrc_T1(0,j,datin)&CRC;
               temp2 = makebyte(0,datin[j+6],datin[j+5],datin[j+4],datin[j+3],datin[j+2],datin[j+1],datin[j]);
               j=j+7;
            }
            //fprintf(COM2,"CRC,j=%d,%lu\r\n",CRC,j);
            //CRC=CRC^temp2;
            //j=j-7;
            //j=j+7;
            //temp2 = makebyte(0,datin[j+6],datin[j+5],datin[j+4],datin[j+3],datin[j+2],datin[j+1],datin[j]);
            //fprintf(COM2,"CRC=%d,%d\r\n",CRC,temp2); 
            if(CRC==1)
            {
               dir=0;
               return i;
            }
            //else return 0;
         }
         //check_crc=1;
         //fprintf(COM2,"%d\r\n",check_crc);   
         /*if((temp==0b01000101)&&(temp1!=0b01111100)&&(temp1!=0b00111110)&&(temp1!=0b00011111)&&(check_crc==1))
         {
            //fprintf(COM2,"\r\ndir=%d,detect_pos=%lu\r\n",0,i);
            dir=0;
            return i;
         }*/
      }
         else 
         {
            temp = makebyte(0,0,0,datin[i+4],datin[i+3],datin[i+2],datin[i+1],datin[i]);
            temp1= makebyte(0,0,0,datin[i+9],datin[i+8],datin[i+7],datin[i+6],datin[i+5]); 
            //CRC=!(datin[i+13]^datin[i+12]^datin[i+11]^datin[i+10]);
            /*if(datin[i+14]== CRC)check_crc=1;
            else check_crc=0;*/
           // fprintf(COM2,"%d\r\n",check_crc); 
            if((temp==0b00001011)&&(temp1!=0b00011111)&&(temp1!=0b00001111))//(temp==0b00001011)&&(temp1!=0b00011111)&&(temp1!=0b00001111)
            {
               j=i;
               CRC=1;
               temp2=temp;
               while((temp2!=0b00011111)&&(j<500))
               {
                  CRC=check_lrc_T2(0,j,datin)&CRC;
                  temp2=makebyte(0,0,0,datin[j+4],datin[j+3],datin[j+2],datin[j+1],datin[j]);
                  j=j+5;
               }
               //fprintf(COM2,"CRC_T2,j=%d,%lu\r\n",CRC,j);
               if(CRC==1)
               {
                  dir=0;
                  return i;
               }
                  //else return 0;
            }
           /* if((temp==0b00001011)&&(temp1!=0b00011111)&&(temp1!=0b00001111)&&(check_crc==1))//&&(temp1!=0b00011111))
            {
               //fprintf(COM2,"\r\ndir=%d,detect_pos=%lu\r\n",0,i);
               dir=0;
               return i;
            }  */          
         }
   }
   for(i=end_point;i>end_point/4;i--)
   {
      if(track==0)
      {
         temp = makebyte(0,datin[i-6],datin[i-5],datin[i-4],datin[i-3],datin[i-2],datin[i-1],datin[i]);
         temp1= makebyte(0,datin[i-13],datin[i-12],datin[i-11],datin[i-10],datin[i-9],datin[i-8],datin[i-7]); 
         /*CRC = !(datin[i-19]^datin[i-18]^datin[i-17]^datin[i-16]^datin[i-15]^datin[i-14]);
         if(datin[i-20] == CRC)check_crc=1;
           else check_crc=0;*/
          // fprintf(COM2,"%d\r\n",check_crc); 
         if((temp==0b01000101)&&(temp1!=0b01111100)&&(temp1!=0b00111110)&&(temp1!=0b00011111)&&(temp1!=0b00001111)&&(temp1!=0b01111000))
         {
            j=i;
            CRC=1;
            temp2=temp;
             while((temp2!=0b00011111)&&(j>1))
             {
               CRC=check_lrc_T1(1,j,datin)&CRC;
               temp2 = makebyte(0,datin[j-6],datin[j-5],datin[j-4],datin[j-3],datin[j-2],datin[j-1],datin[j]);
               j=j-7;            
             }
             //fprintf(COM2,"CRC_T1,j=%d,%lu\r\n",CRC,j);
            if(CRC==1)
            {
               dir=1;
               return i;
            }
            else return 0;
         }
         /*if((temp==0b01000101)&&(temp1!=0b01111100)&&(temp1!=0b00111110)&&(temp1!=0b00011111)&&(temp1!=0b00001111)&&(temp1!=0b01111000)&&(check_crc==1))
         {
            //fprintf(COM2,"\r\ndir=%d,detect_pos=%lu\r\n",1,i);
            dir=1;
            return i;
         }*/
      }
         else
         {
            temp = makebyte(0,0,0,datin[i-4],datin[i-3],datin[i-2],datin[i-1],datin[i]);
            temp1= makebyte(0,0,0,datin[i-9],datin[i-8],datin[i-7],datin[i-6],datin[i-5]);
            /*CRC = !(datin[i-8]^datin[i-7]^datin[i-6]^datin[i-5]);
            if(datin[i-9]== CRC)check_crc=1;
            else check_crc=0;*/
            //fprintf(COM2,"%d\r\n",check_crc); 
            if((temp==0b00001011)&&(temp1!=0b00011111)&&(temp1!=0b00001111))
            {
               j=i;
               CRC=1;
               temp2=temp;
               while((temp2!=0b00011111)&&(j>1))
               {
                  CRC=check_lrc_T2(1,j,datin)&CRC;
                  temp2=makebyte(0,0,0,datin[j-4],datin[j-3],datin[j-2],datin[j-1],datin[j]);
                  j=j-5;
               }
               //fprintf(COM2,"CRC_T2,j=%d,%lu\r\n",CRC,j);
               if(CRC==1)
               {
                  dir=1;
                  return i;
               }
                  else return 0;
            }
           /* if((temp==0b00001011)&&(temp1!=0b00011111)&&(temp1!=0b00001111)&&(check_crc==1))//&&(temp1!=0b00011111))
            {
               //fprintf(COM2,"\r\ndir=%d,detect_pos=%lu\r\n",1,i);
               dir=1;
               return i;
            } */           
         }
   }
   return 0;
}
//=========================
int8 con_data_track(int8 track,int16 countbit ,int8 *datin,int8 *datout)
{
   int8 i;
   int16 j,temp=1;
   temp = detect_pos_st(track,countbit,datin);
   if(temp==0) return 0;
   j=temp;
   if(dir==0)
   {
      if(track==0)
      {
            for(i=0;i<79;i++)
            {
               datout[i]=makebyte(0,0,datin[j+5],datin[j+4],datin[j+3],datin[j+2],datin[j+1],datin[j]);
               j=j+7;
            }
      }
         else 
         {
            for(i=0;i<40;i++)
            {
               datout[i]=makebyte(0,0,0,0,datin[j+3],datin[j+2],datin[j+1],datin[j]);
               j=j+5;
            }         
         }
   }
      else 
      {
         if(track==0)
         {
            for(i=0;i<79;i++)
            {
               datout[i]=makebyte(0,0,datin[j-5],datin[j-4],datin[j-3],datin[j-2],datin[j-1],datin[j]);
               j=j-7;
            }
         }
            else
            {
               for(i=0;i<40;i++)
               {
                  datout[i]=makebyte(0,0,0,0,datin[j-3],datin[j-2],datin[j-1],datin[j]);
                  j=j-5;
               }            
            }
      }
      memset(datin,0,sizeof(datin));
}
//=========================
void debug_reader()
{
   int16 i;
   fprintf(COM2,"\r\nBIT_Track1 Data:\r\n");
   for(i=0;i<numbyteofbuffer1;i++)fprintf(COM2,"%d",buffertrack1[i]);
   fprintf(COM2,"\r\nBIT_Track2 Data:\r\n");
   for(i=0;i<numbyteofbuffer2;i++)fprintf(COM2,"%d",buffertrack2[i]);
}
//=========================
/*void copybuf2temp()
{
   int16 i;
   for(i=0;i<numbyteofbuffer1;i++)temptrack1[i]=buffertrack1[i];
   for(i=0;i<numbyteofbuffer2;i++)temptrack2[i]=buffertrack2[i];
   datinbuf=1;
}*/
//=========================
void saving_card()
{
   int8 temp;
   int16 tempcount;
   //fprintf(COM2,"\r\n");
   saving_flag=1;
   ptr_card=(int32)((get_countcard()*numdata)+ptr_start);
   if(datinbuf==0) 
   {
      countbit_T1=0;
      countbit_T2=0;
      bug_countbit_T1=0;
      bug_countbit_T2=0;
      if(data_avai==0)charac_timeout=0xffffffff;
      saving_flag=0;
      return;
   }//*/
   //disable_interrupts(INT_EXT1_H2L);
   fprintf(COM2,"\r\nSaving Card Data\r\n");
   //key_count=0;
   enable_getpin=1;
   //en_getpin;
   tempcount=countbit_T1;
   con_data_track(0,tempcount,buffertrack1,Track1);
   countbit_T1=0;
   /*   fprintf(COM2,"\r\n");
      for(countbit=0;countbit<numbyteoftrack1;countbit++)
         fputc(Track1[countbit]+0x20,COM2);
      fprintf(COM2,"\r\n");//*/
   tempcount=countbit_T2;   
   con_data_track(1,tempcount,buffertrack2,Track2);
   countbit_T2=0;
   saving_flag=0;
  /*   for(countbit=0;countbit<numbyteoftrack2;countbit++)
         fputc(Track2[countbit]+0x30,COM2);
      fprintf(COM2,"\r\n");//*/
      temp=Track1[0]+0x20;
   if((ptr_card<EEPROM_SIZE_stofkey)&&(temp=='%'))
   {
         write_ext_eeprom((long int)ptr_card++,date);
         write_ext_eeprom((long int)ptr_card++,mon);
         write_ext_eeprom((long int)ptr_card++,h);
         write_ext_eeprom((long int)ptr_card++,min);
         write_ext_eeprom((long int)ptr_card++,sec);          
         for(countbit=0;countbit<numbyteoftrack1;countbit++)
         {
            //fputc(Track1[countbit]+0x20,COM2);
            write_ext_eeprom((long int)ptr_card++,((Track1[countbit]&0b00111111)+0x20)); //(((Track1[countbit]&0b00111111)+0x20))
            
         }
         memset(Track1,0,sizeof(Track1));
         for(countbit=0;countbit<numbyteoftrack2;countbit++)
         {
            //fputc(Track2[countbit]+0x30,COM2);
            write_ext_eeprom((long int)ptr_card++,((Track2[countbit]&0b00001111)+0x30));//putc(((Track2[countbit]&0b00001111)+0x30));    
         } 
         memset(Track2,0,sizeof(Track2));
        fprintf(COM2,"\r\n");
        fprintf(COM2,"Done");
            
        fprintf(COM2,"\r\n");
        fprintf(COM2,"Waiting for PIN number");
        memset(key_data,0,sizeof(key_data)); 
        fprintf(COM2,"\r\n");
        charac_timeout=0;
      //===========================
      //enable_interrupts(INT_EXT1_H2L);
      save_ptrcard(ptr_card,strobe_ptrcard_addr);
      datinbuf=0;
      saving_flag=0;
      data_avai=1;
      enable_getpin=1;
      charac_timeout=0;
      key_count_ms=0;
      //en_getpin;
//      countcard=countcard+1;
//      save_coutcard(countcard);
      //fprintf(COM2,"  countcard=%lu\n\r",get_countcard());
   }//*/
   //fprintf(COM2," save_ptrcard=%lu\n\r",ptr_card);
}
//=========================
void saving_card_encrypt()
{
   int8 temp,i,j,u;
   int8 countbyte=0;
   int16 tempcount;
   int8 carddata[numdataofonecard];
   int8 encryptblock[16];
   int8 tempkey[16];
   saving_flag=1;
   ptr_card=(int32)((get_countcard()*numdataofonecard)+ptr_start);
   if(datinbuf==0) 
   {
      countbit_T1=0;
      countbit_T2=0;
      bug_countbit_T1=0;
      bug_countbit_T2=0;
      if(data_avai==0)charac_timeout=0xffffffff;
      saving_flag=0;
      return;
   }//*/
   fprintf(COM2,"\r\nSaving Card Data\r\n");
   //key_count=0;
   enable_getpin=1;
   //en_getpin;
   tempcount=countbit_T1;
   con_data_track(0,tempcount,buffertrack1,Track1);
   countbit_T1=0;
   tempcount=countbit_T2;   
   con_data_track(1,tempcount,buffertrack2,Track2);
   countbit_T2=0;
   saving_flag=0;
   temp=Track1[0]+0x20;
   if((ptr_card<EEPROM_SIZE_stofkey))
   {
         countbyte = 0;
         carddata[countbyte++]=date;   
         carddata[countbyte++]=mon;   
         carddata[countbyte++]=h;   
         carddata[countbyte++]=min;   
         carddata[countbyte++]=sec;   
         for(countbit=0;countbit<numbyteoftrack1;countbit++)
         {
            carddata[countbyte++] = (INT8)((Track1[countbit]&0b00111111)+0x20);      
         }
         memset(Track1,0,sizeof(Track1));
         for(countbit=0;countbit<numbyteoftrack2;countbit++)
         {
            carddata[countbyte++] = (INT8)((Track2[countbit]&0b00001111)+0x30);           
         } 
         for(i=countbyte;i<numdataofonecard;i++)
         {
             carddata[i] = 0;
         }
         for(i=0;i<8;i++)
         {
             for(j=0;j<16;j++)
             {
                encryptblock[j] = carddata[i*16+j];
             }
             EEPROM_read(strobe_crypto_key,CRYPTO_KEY_SIZE,crypto_key);
             aes_enc_dec((unsigned int8 *)&encryptblock[0], (unsigned int8 *)&crypto_key[0],0);
             /*for(u=0;u<16;u++)
             {
                encryptblock[u]; 
                fprintf(COM2,"%x",encryptblock[u]);
             }*/
             EEPROM_write(ptr_card,16,encryptblock);
             ptr_card=ptr_card+16;
         }
         fprintf(COM2,"\r\n");
         fprintf(COM2,"card data: %lu",ptr_card);
         fprintf(COM2,"\r\n");
         EEPROM_read(ptr_card-128,128,&carddata);
         for(i=0;i<numdataofonecard;i++)
         {
             fprintf(COM2,"%x",carddata[i]);
         }
         memset(Track2,0,sizeof(Track2));
        fprintf(COM2,"\r\n");
        fprintf(COM2,"Done");
            
        fprintf(COM2,"\r\n");
        fprintf(COM2,"Waiting for PIN number");
        memset(key_data,0,sizeof(key_data)); 
        fprintf(COM2,"\r\n");
        charac_timeout=0;
      //===========================
      //enable_interrupts(INT_EXT1_H2L);
      //ptr_card = ptr_card+numdataofonecard;
      save_ptrcard(ptr_card,strobe_ptrcard_addr);
      datinbuf=0;
      saving_flag=0;
      data_avai=1;
      enable_getpin=1;
      charac_timeout=0;
      key_count_ms=0;
      //en_getpin;
//      countcard=countcard+1;
//      save_coutcard(countcard);
      //fprintf(COM2,"  countcard=%lu\n\r",get_countcard());
   }//*/
   //fprintf(COM2," save_ptrcard=%lu\n\r",ptr_card);
}
//=========================
int8 mcr_read()
{
   int8  ST1      = 1;
   int8  ST1_old  = 1;
   int8  ST2      = 1;
   int8  ST2_old  = 1;
   int8  Bit_t2  = 0;
   int8  error_T1=1;
   int8  error_T2=1;
   int16 i;
   // Loop until a card is not present
   card_timeout=0;
   mcr_timeout=0;
   
   //charac_timeout=0; 
   while((card_timeout<65000)&&(input(MCR_DATA1)==1)&&(input(MCR_DATA2)==1))
   {
      card_timeout++;
   }
   card_timeout=0;
   buffertrack1[bug_countbit_T1++]=0;
   buffertrack1[bug_countbit_T1++]=1;
   buffertrack2[bug_countbit_T2++]=0;
   buffertrack2[bug_countbit_T2++]=1;//*/
   while(((bug_countbit_T1<numbyteofbuffer1)&&(bug_countbit_T2<numbyteofbuffer2))&&(card_timeout<65500))
   {
      // Check for NULL pointer and an index less than 79
         // Get strobe number one
         if(count_reading_error>0)
         {
            bug_countbit_T1=0;
            bug_countbit_T2=0;
         }
         card_timeout++;
         mcr_timeout=0;
         //charac_timeout=0;
         ST1 = input(MCR_STROBE1);
         // If the strobe was high and is now low (falling edge),
         // then data is present
        
         if(ST1 == 0 && ST1_old == 1)
         {
            ST1_old = 0;
            buffertrack1[bug_countbit_T1] = !input(MCR_DATA1);
            buffertrack2[bug_countbit_T2] = Bit_t2;
            /*if(countbit_T1<100)
            {
               if(buffertrack1[countbit_T1]==1) countbit_zero_t1=0;
               else countbit_zero_t1++;
            }
            //*/
            bug_countbit_T1++;
            card_timeout=0;
         }
         else if(ST1 == 1)ST1_old = 1;
         // Get strobe number 2
         ST2 = input(MCR_STROBE2);
         if(ST2 == 0 && ST2_old == 1)
         {
            ST2_old = 0;
            Bit_t2 = !input(MCR_DATA2);
            //buffertrack2[countbit_T2]=!input(MCR_DATA2);
            /*if(countbit_T2<100)
            {
               if(Bit_t2==1)countbit_zero_t2=0;
               else countbit_zero_t2++;
            }
            //*/
            bug_countbit_T2++;
            card_timeout=0;
         }
         else if(ST2 == 1)ST2_old = 1;//*/
         if(input(MCR_CARD_PRESENT)==1)//break;
         {
            //fprintf(COM2,"end card=%lu\r\n",card_timeout);
            count_reading_error=0;
            card_timeout=65500;
            break;           
         }
      }
      
      //fprintf(COM2,"\r\nreading done\r\n");
      card_timeout=0;
      error_T1 = detect_pos_st(0,bug_countbit_T1,buffertrack1);
      error_T2 = detect_pos_st(1,bug_countbit_T2,buffertrack2);
      //#if defined(debug)
      if(debugmode==1)
      {
         fprintf(COM2,"\r\ncountbit_T1:%lu\r\n",bug_countbit_T1);
         fprintf(COM2,"\r\ncountbit_T2:%lu\r\n",bug_countbit_T2);
         debug_reader();      
         if(error_T1==0)fprintf(COM2,"\r\nreading error T1\r\n");
         if(error_T2==0)fprintf(COM2,"\r\nreading error T2\r\n");
      }
         else
         {
            //for(i=0;i<3000;i++);
            delay_ms(15);
         }
      //#endif     
      if((error_T1==0)||(error_T2==0))
      {
         //charac_timeout=5000;
        // memset(buffertrack1, 0, sizeof(buffertrack1));   
         //memset(buffertrack2, 0, sizeof(buffertrack2));
         bug_countbit_T2=0;
         bug_countbit_T1=0;
         reading_fg=0;
         count_reading_error++;
         //fprintf(COM2,"\r\nreading error=%d\r\n",count_reading_error);
         mcr_timeout=0;
         //charac_timeout=0xffffffff; // can kiem tra them
         return 0;      
      }
         else 
         {
            //copybuf2temp();
            datinbuf=1;
            reading_fg=1;
            //saving_card();
            countbit_T2=bug_countbit_T2;
            countbit_T1=bug_countbit_T1;
            
            //memset(buffertrack1, 0, sizeof(buffertrack1));   
            //memset(buffertrack2, 0, sizeof(buffertrack2));
            bug_countbit_T1=0;
            bug_countbit_T2=0; 
            mcr_timeout=0;
            charac_timeout=0;
            return 1;
         }
  
// Return any errors
   return 0;
}
//=========================
#INT_EXT1
void read_card_T1()
{
      delay_us(500);
      if(saving_flag==1) return;     
      cardread_st=mcr_read();
      mcr_timeout=0;
      card_timeout=0;     
}
//====================================
#if 0
void debug_card()
{
   int8 i;
   int16 j;
   con_data_track(0,countbit_T1,buffertrack1,Track1);
   countbit_T1=0;
   con_data_track(1,countbit_T2,buffertrack2,Track2);
   countbit_T2=0;
   for(j=0;j<700;j++) buffertrack1[j]=0;
   for(j=0;j<300;j++) buffertrack2[j]=0;
   fprintf(COM2,"\r\nTrack1 Data:\r\n");
   for(i=0;i<numbyteoftrack1;i++)
   {
      fputc(Track1[i]+0x20,COM2);
   }
   fprintf(COM2,"\r\nTrack2 Data:\r\n");
   for(i=0;i<numbyteoftrack2;i++)
   {
      fputc(Track2[i]+0x30,COM2);
   }
}
#endif
//====================================
