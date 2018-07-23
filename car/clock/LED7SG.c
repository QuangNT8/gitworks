


#define  COL0      PIN_A5
#define  COL1      PIN_A3
#define  dot       PIN_A2
#define  KEY       PIN_A1
//#define  COL4      PIN_A0

#define  ROW0      PIN_B4
#define  ROW1      PIN_B5
#define  ROW2      PIN_B6
#define  ROW3      PIN_B7

#define  relay     PIN_A7

//#define  en_getpin      enable_interrupts(INT_TIMER1)
//#define  dis_getpin     disable_interrupts(INT_TIMER1)
//#define  ROW4      PIN_B1

//=================================================
void offled()
{
      output_low(PIN_B1);
      output_low(PIN_B3);  
      output_low(PIN_B2);
      output_low(PIN_C5);
      
      output_low(COL0);
      output_low(COL1);  
      output_low(dot);// dau cham
      output_low(ROW0);
      output_low(ROW1);
      output_low(PIN_C7);
      output_low(PIN_C6);
      output_low(PIN_B0);    
}
//=================================================
void dis_number(int8 num,int8 dott)
{
   switch(num)
   {
      case 0:
         output_low(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_low(ROW1);
         output_low(PIN_C7);
         output_high(PIN_C6);
         output_low(PIN_B0);
      break;
      case 1:
         output_high(COL0);
         output_high(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_high(ROW1);
         output_high(PIN_C7);
         output_high(PIN_C6);
         output_low(PIN_B0);
      break;
      case 2:
         output_low(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_high(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_high(PIN_B0);
      break;
      case 3:
         output_high(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_high(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);         
      break;
      case 4:
         output_high(COL0);
         output_high(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_low(ROW1);
         output_high(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);                  
      break;
      case 5:
         output_high(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_high(ROW0);
         output_low(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);
      break;
      case 6:
         output_low(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_high(ROW0);
         output_low(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);     
      break;
      case 7:
         output_high(COL0);
         output_high(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_high(ROW1);
         output_low(PIN_C7);
         output_high(PIN_C6);
         output_low(PIN_B0);
      break;
      case 8:
         output_low(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_low(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);
      break;
      case 9:
         output_high(COL0);
         output_low(COL1);
         if(dott==0) output_high(dot);// dau cham
            else output_low(dot);// dau cham
         output_low(ROW0);
         output_low(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);                        
      break;    
   }
}
//============================================
void dis_vol(int16 num)
{
   int16 temp;
   offled();
   delay_us(1000);
   temp=num/1000;
   dis_number(temp,1);
   output_high(PIN_B1);
   output_low(PIN_B3);  
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(200);
   temp=num%1000;
   offled();
   dis_number(temp/100,0);
   output_low(PIN_B1);
   output_high(PIN_B3);  
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(200);
   temp=num%100;
   temp=temp/10;
   offled();
   dis_number(temp,0);
   output_low(PIN_B1);
   output_low(PIN_B3);  
   output_high(PIN_B2);
   output_low(PIN_C5);
   delay_us(100);
   temp=num%100;
   temp=temp%10;
   offled();
   dis_number(temp,0);
   output_low(PIN_B1);
   output_low(PIN_B3);  
   output_low(PIN_B2);
   output_high(PIN_C5);
   delay_us(400);
   offled();

}
//============================================
void dis_time(int8 h,int8 min,int8 sec,int8 h_blink,int8 m_blink)
{
   output_low(PIN_B1);
   output_low(PIN_B3);
   output_low(PIN_B2);
   output_low(PIN_C5);
   
   offled();
   dis_number(h/10,0);
   if(h_blink==1)output_high(PIN_B1);
      else output_low(PIN_B1);
   output_low(PIN_B3);  
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(600);
   
   output_low(PIN_B1);
   output_low(PIN_B3);
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(500);
   
   offled();
   dis_number(h%10,sec);
   output_low(PIN_B1);
   if(h_blink==1)output_high(PIN_B3);
      else output_low(PIN_B3);
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(400);

   output_low(PIN_B1);
   output_low(PIN_B3);
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(500);
   //int8 temp;
   offled();
   dis_number(min/10,0);
   output_low(PIN_B1);
   output_low(PIN_B3);  
   
   if(m_blink==1)output_high(PIN_B2);
      else output_low(PIN_B2);
   output_low(PIN_C5);
   delay_us(300);
   
   offled();
   dis_number(min%10,0);
   output_low(PIN_B2);
   if(m_blink==1) output_high(PIN_C5);
      else output_low(PIN_C5);
   delay_us(500);
   
   output_low(PIN_B1);
   output_low(PIN_B3);  
   output_low(PIN_B2);
   output_low(PIN_C5);
   delay_ms(1);
}
//==================================================
void dis_autolight(int8 blk,int8 st)
{
   offled();
   delay_us(1000);
   offled();
   if(blk==1)output_high(PIN_B1);
      else output_low(PIN_B1);
   output_low(PIN_B3);  
   output_low(PIN_B2);
   output_low(PIN_C5);
   
   output_low(COL0);
   output_high(COL1);  
   output_high(dot);// dau cham
   output_low(ROW0);
   output_low(ROW1);
   output_low(PIN_C7);
   output_low(PIN_C6);
   output_low(PIN_B0);
   delay_us(500);
   offled();
   output_low(PIN_B1);
   if(blk==1)output_high(PIN_B3);
      else output_low(PIN_B3);
   output_low(PIN_B2);
   output_low(PIN_C5);
   
   output_low(COL0);
   output_low(COL1);  
   output_high(dot);// dau cham
   output_high(ROW0);
   output_low(ROW1);
   output_high(PIN_C7);
   output_high(PIN_C6);
   output_high(PIN_B0);
   delay_us(500);
   if(st==1)
   {
      offled();
      output_low(PIN_B1);
      output_low(PIN_B3);  
      output_high(PIN_B2);
      output_low(PIN_C5);
      
      output_low(COL0);
      output_low(COL1);  
      output_high(dot);// dau cham
      output_high(ROW0);
      output_high(ROW1);
      output_high(PIN_C7);
      output_low(PIN_C6);
      output_low(PIN_B0);   
      delay_us(500);
      offled();      
      output_low(PIN_B1);
      output_low(PIN_B3);  
      output_low(PIN_B2);
      output_high(PIN_C5);
      
      output_low(COL0);
      output_high(COL1);  
      output_high(dot);// dau cham
      output_high(ROW0);
      output_high(ROW1);
      output_high(PIN_C7);
      output_low(PIN_C6);
      output_low(PIN_B0);   

   }
      else 
      {
         offled();
         output_low(PIN_B1);
         output_low(PIN_B3);  
         output_high(PIN_B2);
         output_low(PIN_C5);
         
         output_low(COL0);
         output_low(COL1);  
         output_high(dot);// dau cham
         output_high(ROW0);
         output_high(ROW1);
         output_high(PIN_C7);
         output_low(PIN_C6);
         output_low(PIN_B0);   
         delay_us(400);
         offled();
         output_low(PIN_B1);
         output_low(PIN_B3);  
         output_low(PIN_B2);
         output_high(PIN_C5);
         
         output_low(COL0);
         output_high(COL1);  
         output_high(dot);// dau cham
         output_high(ROW0);
         output_low(ROW1);
         output_low(PIN_C7);
         output_low(PIN_C6);
         output_high(PIN_B0);  
      }
   delay_us(500);
   offled();
   delay_us(400);
}
//==================================================
