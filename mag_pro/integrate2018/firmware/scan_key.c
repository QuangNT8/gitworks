
//#include <aes.c>
//#include <aes_enc.c>
#include <TI_aes_128.c>

#define  COL0      PIN_A5
#define  COL1      PIN_A3
#define  COL2      PIN_A2
#define  COL3      PIN_A1
#define  COL4      PIN_A0

#define  ROW0      PIN_B4
#define  ROW1      PIN_B5
#define  ROW2      PIN_B6
#define  ROW3      PIN_B7

#define  key       PIN_A7


//#define  en_getpin      enable_interrupts(INT_TIMER1)
//#define  dis_getpin     disable_interrupts(INT_TIMER1)
//#define  ROW4      PIN_B1
#define  turnoff_bgn_af30s  enable_interrupts(INT_RTCC);
//#define ALL_ROWS (ROW0|ROW1|ROW2|ROW3|ROW3)
//#define ALL_COLS (COL0|COL1|COL2|COL3|COL4)

// Keypad layout:
char const KEYS[4][5] = {{'1','2','3','A','B'},
                         {'4','5','6','C','D'},
                         {'7','8','9','E','F'},
                         {'*','0','#','G','H'}
                         };
                       
char const newKEYS[4][6] = {{'1','2','3','A','B','C'},
                            {'4','5','6','D','E','F'},
                            {'7','8','9','G','H','I'},
                            {'*','0','#','J','K','M'}
                           };      
                         
/*char const newKEYS[4][6] = {{'J','#','0','*','K','M'},
                            {'G','9','8','7','H','I'},
                            {'D','6','5','4','E','F'},
                            {'A','3','2','1','B','C'}
                           };    
                           */

#define KBD_DEBOUNCE_FACTOR  1    // Set this number to apx n/333 where
                                

#define     wideofkeystore      16
#define     key_numbyte         wideofkeystore-4

//unsigned int32 count_timer0=0;
int8 delaykey=0;
int8 keydebug_en=0;
int8 enable_getpin=0;
int8 count_strobe_kb=100;
int8 key_count_ms=0;
int8 kp_st=0;
int8 type_KB=0;

int8 key_data[wideofkeystore];
int8 delaycharaction=0;
void kbd_init() {
}
//====================
void delay_kb(int32 tim)
{
   int32 i;
   for(i=0;i<tim;i++);
}
//====================

int8 kbd_getc() {
   static BYTE kbd_call_count;
   static int1 kbd_down;
   static char last_key;
   static BYTE col;
   BYTE kchar;
   BYTE row;
   //kchar='t';//'\0';
   if(++kbd_call_count>KBD_DEBOUNCE_FACTOR) {
       switch (col) {
         case 0   : 
                    output_low(COL0);
                    output_high(COL1);
                    output_high(COL2);
                    output_high(COL3);
                    output_high(COL4);
                    break;
         case 1   : 
                    output_low(COL1);
                    output_high(COL0);
                    output_high(COL2);
                    output_high(COL3);
                    output_high(COL4);
                    break;
         case 2   : 
                    output_low(COL2);
                    output_high(COL1);
                    output_high(COL0);
                    output_high(COL3);
                    output_high(COL4);
                    break;
         case 3   : 
                    output_low(COL3);
                    output_high(COL1);
                    output_high(COL2);
                    output_high(COL0);
                    output_high(COL4);
                    break;
         case 4   : 
                    output_low(COL4);
                    output_high(COL1);
                    output_high(COL2);
                    output_high(COL3);
                    output_high(COL0);
                    break;
       }

       if(kbd_down) 
       {
         if(input(ROW0)&input(ROW1)&input(ROW2)&input(ROW3))
         {
            kchar=last_key;
            if(keydebug_en==0)
            {
                //count_timer0=0;
                charac_timeout=0;
                if(key_count_ms<wideofkeystore)
                {
                    key_data[key_count_ms++]=kchar;
                    fputc(kchar,COM2);
                }
            }
               else fputc(kchar,COM2);
           kbd_down=FALSE;
           last_key=0;
         }
       } else
         {
             if(!(input(ROW0)&input(ROW1)&input(ROW2)&input(ROW3)))
             {
                //set_tris_a(0xff);
                //delay_ms(100);
                if(input(ROW0)==0)
                  row=0;
                else if(input(ROW1)==0)
                  row=1;
                else if(input(ROW2)==0)
                  row=2;
                else if(input(ROW3)==0)
                  row=3;
                /*else if(input(ROW4)==0) 
                  row=4;*/
                last_key =KEYS[row][col];
                kbd_down = TRUE;
                set_tris_a(0xff);
                //lcd_gotoxy(0,1);
                //lcd_putc(last_key);
             } else 
               {
                  ++col;
                  if(col==5) col=0;
               }
         }
      kbd_call_count=0;
   }
  return(kchar);
}
//===========================
int8 read_col(int8 col)
{
   int8 adc;
   setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN4|sAN11|VSS_VDD);
   switch(col)
   {
      case 0:
        set_adc_channel(4);
        adc=read_adc();
        return adc;
      break;
      case 1:
        set_adc_channel(3);
        adc=read_adc();
        return adc;
      break;
      case 2:
        set_adc_channel(2);
        adc=read_adc();
        return adc;
      break;
      case 3:
        set_adc_channel(1);
        adc=read_adc();
        return adc;
      break;
      case 4:
        set_adc_channel(0);
        adc=read_adc();
        return adc;
      break;     
      case 5:
        set_adc_channel(11);
        adc=read_adc();
        return adc;
      break;     
   }
}

//===========================
int8 check_col()
{
   int16 i;
   int32 col_0=0,col_1=0,col_2=0,col_3=0,col_4=0;
   ///for(i=0;i<50;i++)col[i]=0;
   
   for(i=0;i<500;i++)
   {
      col_0=read_col(0)+col_0;
      col_1=read_col(1)+col_1;
      col_2=read_col(2)+col_2;
      col_3=read_col(3)+col_3;
      col_4=read_col(4)+col_4;
   }
   col_0=col_0/500;
   col_1=col_1/500;
   col_2=col_2/500;
   col_3=col_3/500;
   col_4=col_4/500;
   if(keydebug_en)
   {
      fprintf(COM2,"\r\n");     
      fprintf(COM2,"col0 = %lu\r\n",col_0);     
      fprintf(COM2,"col1 = %lu\r\n",col_1);      
      fprintf(COM2,"col2 = %lu\r\n",col_2);     
      fprintf(COM2,"col3 = %lu\r\n",col_3);     
      fprintf(COM2,"col4 = %lu\r\n",col_4);
      fprintf(COM2,"\r\n");
   }
   //*/
   if((col_0>5)&&(col_0<60)&&(col_1>100)&&(col_2>100)&&(col_3>100)&&(col_4>100)) return 0;
   if((col_1>5)&&(col_1<60)&&(col_0>100)&&(col_2>100)&&(col_3>100)&&(col_4>100)) return 1; 
   if((col_2>5)&&(col_2<60)&&(col_1>100)&&(col_0>100)&&(col_3>100)&&(col_4>100)) return 2; 
   if((col_3>5)&&(col_3<60)&&(col_1>100)&&(col_2>100)&&(col_0>100)&&(col_4>100)) return 3;
   if((col_4>5)&&(col_4<60)&&(col_1>100)&&(col_2>100)&&(col_3>100)&&(col_0>100)) return 4; 
   
   if((col_4>200)&&(col_0<180)&&(col_0>100)&&(col_1<180)&&(col_1>100)&&(col_2<180)&&(col_2>100)&&(col_3<180)&&(col_3>100)) return 4;
   /*// thong so cho mach o nha
   if((col_0>25)&&(col_0<50)&&(col_1>50)&&(col_2>50)&&(col_3>50)&&(col_4>50)) return 0;
   if((col_1>25)&&(col_1<50)&&(col_0>50)&&(col_2>50)&&(col_3>50)&&(col_4>50)) return 1; 
   if((col_2>25)&&(col_2<50)&&(col_1>50)&&(col_0>50)&&(col_3>50)&&(col_4>50)) return 2; 
   if((col_3>25)&&(col_3<50)&&(col_1>50)&&(col_2>50)&&(col_0>50)&&(col_4>50)) return 3;
   if((col_4>25)&&(col_4<50)&&(col_1>50)&&(col_2>50)&&(col_3>50)&&(col_0>50)) return 4; */
   
//!   if((col_0<35)&&(col_0>col_1)&&(col_0>col_2)&&(col_0>col_3)&&(col_0>col_4)) return 0;
//!   if((col_1<35)&&(col_1>col_0)&&(col_1>col_2)&&(col_1>col_3)&&(col_1>col_4)) return 1;
//!   if((col_2<35)&&(col_2>col_0)&&(col_2>col_1)&&(col_2>col_3)&&(col_2>col_4)) return 2;
//!   if((col_3<35)&&(col_3>col_0)&&(col_3>col_1)&&(col_3>col_2)&&(col_3>col_4)) return 3;
//!   if((col_4<35)&&(col_4>col_0)&&(col_4>col_1)&&(col_4>col_3)&&(col_4>col_2)) return 4;
   else return 0xff;
}
//===========================================
int8 new_check_col()
{
   int16 i;
   int32 col_0=0,col_1=0,col_2=0,col_3=0,col_4=0,col_5=0;
   ///for(i=0;i<50;i++)col[i]=0;
   
   for(i=0;i<500;i++)
   {
      col_0=read_col(0)+col_0;
      col_1=read_col(1)+col_1;
      col_2=read_col(2)+col_2;
      col_3=read_col(3)+col_3;
      col_4=read_col(4)+col_4;
      col_5=read_col(5)+col_5;
   }
   col_0=col_0/500;
   col_1=col_1/500;
   col_2=col_2/500;
   col_3=col_3/500;
   col_4=col_4/500;
   col_5=col_5/500;
   if(keydebug_en)
   {
      fprintf(COM2,"\r\n");     
      fprintf(COM2,"col0 = %lu\r\n",col_0);     
      fprintf(COM2,"col1 = %lu\r\n",col_1);      
      fprintf(COM2,"col2 = %lu\r\n",col_2);     
      fprintf(COM2,"col3 = %lu\r\n",col_3);     
      fprintf(COM2,"col4 = %lu\r\n",col_4);
      fprintf(COM2,"col5 = %lu\r\n",col_5);
      fprintf(COM2,"\r\n");
   }
   //*/
   if((col_0>5)&&(col_0<60)&&(col_1>100)&&(col_2>100)&&(col_3>100)&&(col_4>100)&&(col_5>100)) return 0;
   if((col_1>5)&&(col_1<60)&&(col_0>100)&&(col_2>100)&&(col_3>100)&&(col_4>100)&&(col_5>100)) return 1; 
   if((col_2>5)&&(col_2<60)&&(col_1>100)&&(col_0>100)&&(col_3>100)&&(col_4>100)&&(col_5>100)) return 2; 
   if((col_3>5)&&(col_3<60)&&(col_1>100)&&(col_2>100)&&(col_0>100)&&(col_4>100)&&(col_5>100)) return 3;
   if((col_4>5)&&(col_4<60)&&(col_1>100)&&(col_2>100)&&(col_3>100)&&(col_0>100)&&(col_5>100)) return 4; 
   if((col_5>5)&&(col_5<60)&&(col_1>100)&&(col_2>100)&&(col_3>100)&&(col_4>100)&&(col_0>100)) return 5; 
   
   if((col_5>200)&&(col_0<180)&&(col_0>100)&&(col_1<180)&&(col_1>100)&&(col_2<180)&&(col_2>100)&&(col_3<180)&&(col_3>100)&&(col_4<180)&&(col_4>100)) return 5;
   else return 0xff;
}

//==========================
int8 KB_count=0;

int8 kbd_getc_slv(){
   static BYTE kbd_call_count;
   static int1 kbd_down;
   static char last_key;
   BYTE col;
   BYTE kchar=' ';
   BYTE row;
    if((input(ROW0)==0)||(input(ROW1)==0)||(input(ROW2)==0)||(input(ROW3)==0))
    {
       if(input(ROW0)==0)
         row=0;
       else if(input(ROW1)==0)
         row=1;
       else if(input(ROW2)==0)
         row=2;
       else if(input(ROW3)==0)
         row=3;     
       if(type_KB==0)col=check_col();
         else col=new_check_col();
       //fprintf(COM2,"\r\nrow = %d\r\n",row);
       //fprintf(COM2,"col = %d\r\n",col);
       if(col!=0xff)
       {
         if(type_KB==0)kchar = KEYS[row][col];
           else kchar =newKEYS[row][col];
         //fprintf(COM2,"kchar %c\r\n",kchar);
         if(keydebug_en==0)
            {
              charac_timeout=0;
               if(key_count_ms<wideofkeystore)
               {
                   key_data[key_count_ms++]=kchar;
                   fprintf(COM2,"key data[%d] %c\r\n",key_count_ms,kchar);
               }
            }
               else fputc(kchar,COM2);
       }
       while((input(ROW0)==0)||(input(ROW1)==0)||(input(ROW2)==0)||(input(ROW3)==0));
       while((input(ROW0)==0)||(input(ROW1)==0)||(input(ROW2)==0)||(input(ROW3)==0));
    }
  set_tris_a(0xff);    
  return(kchar);
}//*/

//============================================
void save_key_new()
{
   int8 i,tempdata;
   int16 temp;
   if(ptr_card_key<EEPROM_SIZE_endofkey)
   {
      if((key_data[0]!=0)||(key_count_ms>0))
      {
         temp=get_countcard();
         //fprintf(COM2," get_countcard=%lu\n\r",temp);
         ptr_card_key=(int32)(((temp)*wideofkeystore)+EEPROM_KEY_ST);
         //fprintf(COM2," ptr_card_key=%lu\n\r",ptr_card_key);
         for(i=0;i<wideofkeystore;i++)
         {
            write_ext_eeprom((int32)(ptr_card_key+i),0);
         }
         for(i=0;i<wideofkeystore;i++)
         {
            tempdata=key_data[i];
            write_ext_eeprom((int32)ptr_card_key++,tempdata);
            //delay_us(100);
            fprintf(COM2,"%c",tempdata);
         }
          //fprintf(COM2," ptr_card_key=%lu\n\r",ptr_card_key);
          /*fprintf(COM2,"\n\r");
          fprintf(COM2,"read key=\n\r");
          for(i=0;i<wideofkeystore;i++)
          {
            fprintf(COM2,"%c",read_ext_eeprom(ptr_card_key-i));
          }*/
         save_ptrcard(ptr_card_key,strobe_ptrcard_key);
         //fprintf(COM2," save_ptrcard_key=%lu\n\r",get_ptrcard(strobe_ptrcard_key));
      }
      //key_count=0;
      key_count_ms=0;
      memset(key_data,0,sizeof(key_data)); 
      fprintf(COM2,"\r\n");
      fprintf(COM2,"Done save_key_new");
      fprintf(COM2,"\r\n");
   }
}
//============================================
void save_key_encrypt()
{
   int i;
   int16 temp;
   int8 retval;
   int8 rec[wideofkeystore];
   
   if(ptr_card_key<EEPROM_SIZE_endofkey)
   {
      if((key_data[0]!=0)||(key_count_ms>0))
      {   
         for(i=0;i<key_numbyte;i++)
         {
            temp=key_data[i];
            fputc(temp,COM2);
            rec[i]=key_data[i];
         }
         rec[key_numbyte] = 0;
         rec[key_numbyte+1] = 0;         
         rec[key_numbyte+2] = 0;         
         rec[key_numbyte+3] = 0;
    #if 0
         fprintf(COM2,"\n\r");
         fprintf(COM2,"test_key:\n\r");
         for(i=0;i<16;i++) fprintf(COM2,"%x",test_key[i]);
         fprintf(COM2,"\n\r");
         fprintf(COM2,"\n\rdatain:\n\r");
         for(i=0;i<16;i++) fprintf(COM2," %x",rec[i]);
         fprintf(COM2,"\n\r");
    #endif
         aes_enc_dec((unsigned int8 *)&rec[0], (unsigned int8 *)&crypto_key[0],0);
         //aes_enc_dec((unsigned int8 *)&test_data[0], (unsigned int8 *)&test_key[0],1);
         
         /* get the pointer of keyboard data */
         temp=get_countcard();
         ptr_card_key=(int32)(((temp)*wideofkeystore)+EEPROM_KEY_ST);
         /* delete the old data of key store area  */
         for(i=0;i<wideofkeystore;i++)
         {
            write_ext_eeprom((int32)(ptr_card_key+i),0);
         }
         
         for(i=0;i<wideofkeystore;i++) fprintf(COM2,"%x",rec[i]);
         retval = EEPROM_write(ptr_card_key,wideofkeystore,rec); 
         if(retval!=0)
         {
            ptr_card_key+=wideofkeystore;
            save_ptrcard(ptr_card_key,strobe_ptrcard_key);
         }         
      }
      key_count_ms=0;
      memset(key_data,0,sizeof(key_data)); 
      fprintf(COM2,"\r\n");
      fprintf(COM2,"Done Encrypted PIN");
      fprintf(COM2,"\r\n");
   }
}
//===========================================
