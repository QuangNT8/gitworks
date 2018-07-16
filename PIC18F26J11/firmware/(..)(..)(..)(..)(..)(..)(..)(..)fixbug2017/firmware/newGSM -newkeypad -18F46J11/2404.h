 void eep_write(unsigned char add,unsigned char dat)
{
   I2C_Start();
   I2C_Write(0xA0);
   I2C_Write(add);
   I2C_Write(dat);
   I2C_Stop();
   Delay_ms(10);
   return;
}
//=====================================
unsigned char eep_read(unsigned char add)
{
   unsigned char dat;
   I2C_Start();
   I2C_Write(0xA0);
   I2C_Write(add);
   I2C_Start();
   I2C_Write(0xa1);
   dat=I2C_Read();
   I2C_Stop();
   return (dat);
}
