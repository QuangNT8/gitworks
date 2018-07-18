#define    sda   PIN_B7
#define    scl   PIN_B6
 //=============================================
void delay_i2c (unsigned long d)  
{
   unsigned long i;
   for (i=0;i<=d;i++);
   return;
}
//=========================================
void i2c_start()
{
   output_high (sda); //sda=1;
   output_high (scl); //scl=1;
   output_low  (sda); //sda=0;
   delay_us(500);
   output_low  (scl); //scl=0;
   delay_us(500);
   return;
}
//=========================================
void i2c_stop()
{
   output_low  (sda); //sda=0;
   output_high (scl); //scl=1;
   delay_us(50);
   output_high (sda); //sda=1;
   delay_us(50);
   return;
}
//=====================================
int1 i2c_write(unsigned char daa)
{
   unsigned char i,buf; 
   int1 ack=1;
   buf=daa;
   for (i=0;i<8;i++)
   {
      output_bit(sda,((buf&0x80)>>7));   //sda=(buf&0x80)>>7;
      //delay_us(20);
      output_high (scl); //scl=1;
      delay_us(50);
      output_low  (scl); //scl=0;
      buf<<=1;
      delay_us(50);
   }
   output_high (scl);   //scl=1; 
   delay_us(5);
   ack = input(sda);    //read ack from slave
   delay_us(50);
   output_low  (scl);   //scl=0;
   delay_us(50);
   return ack;
}
//=====================================
unsigned char i2c_read()
{
unsigned char i,dat=0;
for (i=0;i<=7;i++)
{
   output_high (scl); //scl=1;
   output_high (sda); //sda=1;
   dat<<=1;
   dat=input(sda)|dat;
   delay_us(50);
   output_low (scl); //scl=0;
   delay_us(50);
}
return dat;
}
//====================================
void writedata2_485(unsigned int8 dat1,unsigned int8 dat2,unsigned int8 dat3)
{
   unsigned int8  checksum;
   unsigned int16 buffer;   
   int i;
   buffer=dat1+dat2+dat3;
   checksum = buffer;
   for(i=0;i<10;i++)
   {
       I2C_Start();
       i2c_write(dat1);
       i2c_write(dat2);
       i2c_write(dat3);
       i2c_write(checksum);         
       i2c_stop();
       delay_ms(50);
   }
}
//====================================
